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

int main() {
  std::cout << "Hello World!\n";

  const std::vector<uint8_t> binary = read_binary_file("test.elf");
  using namespace riscv;

  Machine<RISCV64> machine{binary};

  machine.simulate();
}
