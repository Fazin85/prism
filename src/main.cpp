#include <atomic>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <libriscv/machine.hpp>
#include <libriscv/rv32i_instr.hpp>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

std::vector<uint8_t> read_binary_file(const std::string &file_path) {
  std::ifstream file(file_path,
                     std::ios::in | std::ios::binary |
                         std::ios::ate); // ate for seeking to end immediately
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << file_path << std::endl;
    return {}; // Return empty vector on error
  }

  std::streampos file_size = file.tellg(); // Get current position (end of file)
  file.seekg(0, std::ios::beg);            // Seek back to beginning

  std::vector<uint8_t> buffer(file_size);
  file.read(reinterpret_cast<char *>(buffer.data()), file_size);

  file.close();
  return buffer;
}

struct process {
  uint64_t pid;
  riscv::Machine<riscv::RISCV64> machine;
  std::mutex machine_mutex;
};

struct process_initializer {
  std::string proc_name; // the name of the process
  std::vector<uint8_t> program_binary;
  uint64_t memory_max;
  uint64_t instructions_per_lock;
};

constexpr uint64_t memory_max = 256 << 20; // 256mb
static uint64_t next_pid = 0;
static std::unordered_map<uint64_t, process> processes;

void setup_machine(riscv::Machine<riscv::RISCV64> &machine,
                   const process_initializer &initializer) {
  using namespace riscv;

  machine.setup_linux({initializer.proc_name});
  machine.setup_linux_syscalls();

  Machine<RISCV64>::install_syscall_handler(
      94, // exit_group
      [](Machine<RISCV64> &machine) {
        const auto code = machine.cpu.reg(0);
        printf("program exited, exit code = %ld\n", code);
        machine.stop();
      });
}

void run_process(const process_initializer &initializer) {
  process p{
      next_pid++,
      {initializer.program_binary, {.memory_max = initializer.memory_max}}};

  {
    std::lock_guard<std::mutex> lock(p.machine_mutex);
    setup_machine(p.machine, initializer);
    p.machine.set_max_instructions(UINT64_MAX);
  }

  uint64_t locks = 0;
  while (true) {
    std::lock_guard<std::mutex> lock(p.machine_mutex);
    locks++;
    for (auto i = 0; i < initializer.instructions_per_lock; i++) {
      if (p.machine.stopped()) {
        goto done;
      }
      auto &cpu = p.machine.cpu;
      const auto instruction = cpu.read_next_instruction();

      cpu.execute(instruction);
      cpu.increment_pc(instruction.length());
      p.machine.increment_counter(1);
    }
  }
done:
  printf("locks: %lu\n", locks);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "no input file specified\n";
    return -1;
  }

  const std::vector<uint8_t> binary = read_binary_file(argv[1]);

  if (binary.empty()) {
    std::cout << "failed to load " << argv[1] << "\n";
    return -1;
  }

  process_initializer initializer{"test_program", binary, memory_max, 1000};
  std::thread t(run_process, initializer);
  t.join();
}
