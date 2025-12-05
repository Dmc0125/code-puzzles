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

    for (int i = 0; i < ns->len; i += 1) {
        int l = i;
        int r = (ns->len) - i - 1;
        if (l > r) {
            break;
        }

        int temp = ns->digits[r];
        ns->digits[r] = ns->digits[l];
        ns->digits[l] = temp;
    }
}

void sum_invalid_ids(u64 start, u64 end, u64 *sum) {
    for (; start <= end; start += 1) {
        NumStr n = {};
        NumStr_from_u64(&n, start);

        int len = 0;

outer:
        while (1) {
            len += 1;
            // string needs to be at least 2x the length of the substring
            if (n.len < ((len * 2))) {
                break;
            }
            if (n.len % len != 0) {
                continue;
            }

            // count of possible repetitions in string
            int count = n.len / len;

            // 123123123
            // len = 3
            // word = 0
            //
            // 0 == 3
            // 1 == 4
            // 2 == 5
            //
            // 3 == 6
            // 4 == 7
            // 5 == 8
            for (int word = 0; word < count - 1; word += 1) {
                int word_start = word * len;
                int word_end = (word * len) + len;
                // printf("num %ld len %d word1 %d word2 %d word1start %d word2start %d\n",
                //         start, len, word, word +1, word_start, word_end);

                for (int c = 0; c < len; c += 1) {
                    char d1 = n.digits[word_start + c];
                    char d2 = n.digits[word_end + c];

                    // printf("d1 %d d2 %d\n", d1, d2);

                    if (d1 != d2) {
                        goto outer;
                    }
                }
            }

            printf("repeated: %ld len %d\n", start, len);
            *sum += start;
            break;
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
