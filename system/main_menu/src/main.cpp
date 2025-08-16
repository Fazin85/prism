extern "C" void _start();

static long syscall(long number, long arg0 = 0, long arg1 = 0, long arg2 = 0) {
  register long a0 asm("a0") = arg0;
  register long a1 asm("a1") = arg1;
  register long a2 asm("a2") = arg2;
  register long a7 asm("a7") = number;
  register long result asm("a0");

  asm volatile("ecall"
               : "=r"(result)
               : "r"(a0), "r"(a1), "r"(a2), "r"(a7)
               : "memory");
  return result;
}

void _start() { syscall(1); }
