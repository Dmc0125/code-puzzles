#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  int joltages[256];
  int count;
} Batteries;

void Batteries_append(Batteries *b, int joltage) {
  b->joltages[b->count] = joltage;
  b->count += 1;
}

int Batteries_flush(Batteries *b) {
  if (b->count == 0) {
    return 0;
  }

  int max = 0;

  for (int i = 0; i < b->count - 1; i += 1) {
    int joltage_a = b->joltages[i] * 10;

    for (int j = i + 1; j < b->count; j += 1) {
      int joltage_b = b->joltages[j];

      int sum = joltage_a + joltage_b;
      if (sum > max) {
        max = sum;
      }
    }
  }

  b->count = 0;
  return max;
}

int main() {
  int file_handle = open("./input.txt", O_RDONLY);
  if (file_handle == -1) {
    perror("unable to read file");
    return 1;
  }

  size_t byte_count = 1000;
  char buffer[byte_count];

  Batteries batteries = {};
  int joltage_sum = 0;

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

  printf("Sum is: %d\n", joltage_sum);

  return 0;
}
