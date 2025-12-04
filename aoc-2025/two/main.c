#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int parse_int(char arr[], int count) {
    int res = 0;
    for (int i = 0; i < count; i += 1) {
        res = res * 10 + (int)arr[i];
    }
    return res;
}

int handle_turn(int direction, char nums[], int num_count, int arrow_pos, int *password) {
    int count = parse_int(nums, num_count);
    int zeros = 0;

    for (int i = 0; i < count; i += 1) {
        if (direction == 0) {
            arrow_pos -= 1;
        } else {
            arrow_pos += 1;
        }

        arrow_pos = arrow_pos % 100;
        if (arrow_pos == 0) {
            zeros += 1;
        }
    }

    *password = (*password) + zeros;
    return arrow_pos;
}

void print_chars(char arr[], int arr_count) {
    for (int i = 0; i < arr_count; i += 1) {
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main() {
    int input_handle = open("./input.txt", O_RDONLY);

    if (input_handle == -1) {
        perror("unable to open input.txt");
        return 1;
    }

    size_t byte_count = 1000;
    char buffer[byte_count];

    int arrow_position = 50;
    // 0 -> left
    // 1 -> right
    int direction = 0;
    // 0 -> direction
    // 1 -> count
    int read_state = 0;
    char nums[10];
    int nums_count = 0;
    int flushed = 0;
    int password = 0;

    while (1) {
        ssize_t bytes_read = read(input_handle, buffer, byte_count);

        for (int i = 0; i < bytes_read; i += 1) {
            char c = buffer[i];

            if (c == '\n') {
                arrow_position = handle_turn(
                        direction, nums, nums_count, arrow_position, &password
                );
                
                flushed = 1;
                read_state = 0;
                nums_count = 0;
            } else {
                switch (read_state) {
                    case 0:
                        if (c == 'L') {
                            direction = 0;
                        } else {
                            direction = 1;
                        }
                        flushed = 0;
                        read_state = 1;
                        break;
                    case 1:
                        nums[nums_count] = c - 48;
                        nums_count += 1;
                        break;
                }
            }
        }

        if (bytes_read < byte_count) {
            break;
        }
    }

    if (flushed == 0) {
        arrow_position = handle_turn(
            direction, nums, nums_count, arrow_position, &password
        );
    }

    printf("Password is: %d\n", password);

    return 0;
}
