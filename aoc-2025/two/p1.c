#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

typedef uint64_t u64;

typedef struct {
    // u64 max => 18446744073709551615 => 20 chars
    char digits[20];
    int len;
} NumStr;

void NumStr_append_digit(NumStr *np, char digit) {
    np->digits[np->len] = digit - '0';
    np->len += 1;
}

u64 NumStr_flush(NumStr *np) {
    u64 n = 0;
    for (int i = 0; i < np->len; i += 1) {
        n = n * 10 + (int)(np->digits[i]);
    }
    np->len = 0;
    return n;
}

void NumStr_from_u64(NumStr *ns, u64 num) {
    u64 n = num;
    while (n > 0) {
        char rem = ((char)(n % 10)) + '0';
        NumStr_append_digit(ns, rem);
        n = n / 10;
    }
}

void sum_invalid_ids(u64 start, u64 end, u64 *sum) {
    for (; start <= end; start += 1) {
        NumStr n = {};
        NumStr_from_u64(&n, start);

        int l = n.len;
        if (l % 2 != 0) {
            continue;
        }

        int mid = l / 2;
        int eq = 1;
        for (int i = 0; i < mid; i += 1) {
            char d1 = n.digits[i];
            char d2 = n.digits[i + mid];

            if (d1 != d2) {
                eq = 0;
                break;
            }
        }

        if (eq == 1) {
            *sum += start;
        }
    }
}

int main() {
    int input_handle = open("./input.txt", O_RDONLY);
    if (input_handle == -1) {
        perror("unable to open input.txt");
        return 1;
    }

    size_t byte_count = 1000;
    char buffer[byte_count];

    NumStr num_accum = {}; 
    u64 range_start = 0;
    u64 invalid_ids_sum = 0;

    while (1) {
        ssize_t bytes_read = read(input_handle, buffer, byte_count);

        for (int i = 0; i < bytes_read; i += 1) {
            char c = buffer[i];
            if (c == '\n') {
                continue;
            }

            switch (c) {
                case ',':
                    u64 range_end = NumStr_flush(&num_accum);
                    printf("start %ld - end %ld\n", range_start, range_end);
                    sum_invalid_ids(range_start, range_end, &invalid_ids_sum);
                    break;
                case '-':
                    range_start = NumStr_flush(&num_accum);
                    break;
                default:
                    NumStr_append_digit(&num_accum, c);
            }
        }

        u64 range_end = NumStr_flush(&num_accum);
        printf("start %ld - end %ld\n", range_start, range_end);
        sum_invalid_ids(range_start, range_end, &invalid_ids_sum);

        if (bytes_read < byte_count) {
            break;
        }
    }

    printf("Sum: %ld\n", invalid_ids_sum);

    return 0;
}
