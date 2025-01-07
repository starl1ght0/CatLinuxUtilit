#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
    printf("Usage: s21_cat [OPTION] [FILE]...\n");
    printf("Options:\n");
    printf("  -b, --number-nonblank    Number non-blank lines\n");
    printf("  -e                       Display end-of-line characters as $\n");
    printf("  -E                       Display end-of-line characters as $ (without -v)\n");
    printf("  -n, --number             Number all lines\n");
    printf("  -s, --squeeze-blank      Squeeze multiple blank lines into one\n");
    printf("  -t                       Display tabs as ^I\n");
    printf("  -T                       Display tabs as ^I (without -v)\n");
    printf("  -v                       Display non-printing characters\n");
}

void print_non_printable(char c) {
    if (c < 32 && c != '\n' && c != '\t') {
        printf("^%c", c + 64);  // Отображение символов управления как ^X
    } else if (c == 127) {
        printf("^?");  // Отображение символа DEL
    } else if ((unsigned char)c >= 128) {
        printf("M-^%c", c - 128 + 64);
    } else {
        putchar(c);
    }
}

void process_file(FILE *file, int b_flag, int e_flag, int n_flag, int s_flag, int t_flag, int v_flag) {
    char line[1024];
    int line_number = 1;
    int prev_blank = 0;

    while (fgets(line, sizeof(line), file)) {
        int is_blank = (line[0] == '\n');

        if (s_flag && is_blank && prev_blank) {
            continue;
        }
        prev_blank = is_blank;

        if (b_flag && !is_blank) {
            printf("%6d\t", line_number++);
        }

        else if (n_flag) {
            printf("%6d\t", line_number++);
        }

        for (char *p = line; *p; p++) {
            if (t_flag && *p == '\t') {
                printf("^I");
            } else if (v_flag && *p != '\n') {
                print_non_printable(*p);
            } else if (*p == '\n') {
                if (e_flag) {
                    printf("$");
                }
                putchar('\n');
            } else {
                putchar(*p);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int b_flag = 0, e_flag = 0, n_flag = 0, s_flag = 0, t_flag = 0, v_flag = 0;
    int file_start = 1;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            file_start++;
            if (strcmp(argv[i], "--number") == 0) {
                n_flag = 1;
            } else if (strcmp(argv[i], "--number-nonblank") == 0) {
                b_flag = 1;
            } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
                s_flag = 1;
            } else {
                for (int j = 1; argv[i][j]; j++) {
                    switch (argv[i][j]) {
                        case 'b':
                            b_flag = 1;
                            break;
                        case 'e':
                            e_flag = 1;
                            v_flag = 1;
                            break;
                        case 'E':
                            e_flag = 1;
                            break;
                        case 'n':
                            n_flag = 1;
                            break;
                        case 's':
                            s_flag = 1;
                            break;
                        case 't':
                            t_flag = 1;
                            v_flag = 1;
                            break;
                        case 'T':
                            t_flag = 1;
                            break;
                        case 'v':
                            v_flag = 1;
                            break;
                        default:
                            print_usage();
                            return 1;
                    }
                }
            }
        } else {
            break;
        }
    }

    // If no files are provided, read from stdin
    if (file_start == argc) {
        process_file(stdin, b_flag, e_flag, n_flag, s_flag, t_flag, v_flag);
    } else {
        // Process each file
        for (int i = file_start; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror(argv[i]);
                continue;
            }
            process_file(file, b_flag, e_flag, n_flag, s_flag, t_flag, v_flag);
            fclose(file);
        }
    }

    return 0;
}
