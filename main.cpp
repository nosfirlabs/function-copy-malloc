#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// This is the function we want to duplicate at runtime
int original_function(int x, int y) {
  int result = 0;
  for (int i = 0; i < x; i++) {
    result += y;
  }
  return result;
}

int main() {
  // Open a file descriptor for the executable
  int fd = open("/proc/self/exe", O_RDONLY);
  if (fd < 0) {
    perror("Failed to open executable");
    return 1;
  }

  // Get the size of the original function
  size_t size = (char*)original_function - (char*)main;

  // Map the machine code for the original function into memory using mmap
  void* original_code = mmap(NULL, size, PROT_READ | PROT_EXEC, MAP_PRIVATE, fd, (char*)main - (char*)main);
  if (original_code == MAP_FAILED) {
    fprintf(stderr, "Failed to map original function: %s\n", strerror(errno));
    return 1;
  }

  // Allocate memory for the duplicate function using malloc
  void* duplicate = malloc(size);

  // Copy the machine code for the original function to the duplicate function
  memcpy(duplicate, original_code, size);

  // Cast the duplicate function to a function pointer and call it
  int (*duplicate_function)(int, int) = (int (*)(int, int))duplicate;
  printf("Result: %d\n", duplicate_function(5, 7));

  // Unmap the original code and free the allocated memory
  munmap(original_code, size);
  free(duplicate);

  return 0;
}

