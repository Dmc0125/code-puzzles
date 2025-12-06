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

u64 Batteries_flush(Batteries *b) {
  if (b->count == 0) {
    return 0;
  }

  // Not working correctly, im not checking different positions of same numbers
  int max_idxs[12] = {
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  };
  int maxes[256] = {};
  for (int i = 0; i < 256; i += 1) {
    maxes[i] = -1;
  }

  for (int iter = 0; iter < 12; iter += 1) {
    int max = 0;
    int max_idx = 0;

    for (int i = 0; i < b->count; i += 1) {
      int is_excluded = 0;
      for (int j = 0; j < iter; j += 1) {
        if (i == max_idxs[j]) {
          is_excluded = 1;
          break;
        }
      }

      if (is_excluded == 0) {
        int joltage = b->joltages[i];
        if (joltage > max) {
          max = joltage;
          max_idx = i;

          max_idxs[iter] = i;
          // printf("iter %d idx %d\n", iter, i);
        }
      }
    }

    maxes[max_idx] = max;

    printf("%d", max);
    // sum = (sum * 10) + max;
  }

  printf("\n");

  u64 sum = 0;

  for (int i = 0; i < 256; i += 1) {
    u64 m = (u64)(maxes[i]);
    if (m != -1) {
      printf("%ld", m);
      sum = (sum * 10) + m;
    }
  }
  printf("\n");

  b->count = 0;
  return sum;
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
