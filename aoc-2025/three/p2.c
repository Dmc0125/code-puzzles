#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

typedef uint64_t u64;

typedef struct {
  int joltages[256];
  int count;
} Batteries;

void Batteries_append(Batteries *b, int joltage) {
  b->joltages[b->count] = joltage;
  b->count += 1;
}

#define WINDOW_SIZE 12

u64 Batteries_flush(Batteries *b) {
  if (b->count == 0) {
    return 0;
  }

  // u64 max = 0;

  for (int i = 0; i < b->count - WINDOW_SIZE; i += 1) {
    // now need to find 12 highest
    // and update overall max if it is greater
    int count = 0;
    while (count < 12) {
      int max = 0;
      int indexes[12]

      for (int j = i + count; j < b->count; j += 1) {
        int digit = b->joltages[j];
        if (digit > max) {

        }
      }
    }
  }

  b->count = 0;
  return 0;
}

int main() {
  int file_handle = open("./example.txt", O_RDONLY);
  if (file_handle == -1) {
    perror("unable to read file");
    return 1;
  }

  size_t byte_count = 1000;
  char buffer[byte_count];

  Batteries batteries = {};
  u64 joltage_sum = 0;

  while (1) {
    ssize_t bytes_read = read(file_handle, buffer, byte_count);

    for (int i = 0; i < bytes_read; i += 1) {
      char joltage = buffer[i];

      if (joltage == '\n') {
        joltage_sum += Batteries_flush(&batteries);
      } else {
        Batteries_append(&batteries, (int)(joltage - '0'));
      }
    }

    if (bytes_read < byte_count) {
      joltage_sum += Batteries_flush(&batteries);
      break;
    }
  }

  printf("Sum is: %ld\n", joltage_sum);

  return 0;
}
