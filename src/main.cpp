#include <fstream>
#include <iostream>
#include <libriscv/machine.hpp>
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

int main(int argc, char **argv) {
  std::cout << "Hello World!\n";

  if (argc < 2) {
    std::cout << "no input file specified\n";
    return -1;
  }

  const std::vector<uint8_t> binary = read_binary_file(argv[1]);
  using namespace riscv;

  Machine<RISCV64> machine{binary};

  machine.install_syscall_handler(1, [](auto &m) -> void {
    auto exit_code = m.cpu.reg(10);
    std::cout << "process stopped with exit code " << exit_code << "\n";
    m.stop();
  });

  machine.simulate();
}
