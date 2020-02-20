#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "functions.h"
#include "auxiliary.h"

void wrap(char (*text)[1000], int *line_count, int max_line_length) {
    int i, j;
    int new_line_count = 0, length, counter;

    char buffer[1000][1000];
    for (i = 0; i < 1000; i++) memset(buffer[i], '\0', max_line_length + 10);

    align_left_paragraphs(text, *line_count);
    remove_endline(text, *line_count);

    char *token = NULL;
    for (i = 0; i < *line_count; i++) {
        /* paragraphs beginning with whitespace */
        if (i == 0 || text[i - 1][0] == '\n') {
            counter = 0;

            for (j = 0; j < strlen(text[i]); j++) {
                if (text[i][j] == ' ') counter++;
                else break;
            }

            while (counter > max_line_length) {
                buffer[new_line_count][0] = '\n';
                buffer[new_line_count][1] = '\0';
                new_line_count++;

                counter -= max_line_length + 1;
            }

            memset(buffer[new_line_count], ' ', counter);
        }

        token = strtok(text[i], " ");

        while (token) {
            length = strlen(buffer[new_line_count]);

            if (token[0] == '\n') {
                buffer[new_line_count][length - 1] = '\0';
                new_line_count++;

                buffer[new_line_count][0] = '\n';
                buffer[new_line_count][1] = '\0';
                new_line_count++;

                break;
            }

            if (length + strlen(token) > max_line_length) {
                buffer[new_line_count][length - 1] = '\0';
                new_line_count++;
                length = 0;
            }

            memcpy(buffer[new_line_count] + length, token, strlen(token));
            length = strlen(buffer[new_line_count]);
            buffer[new_line_count][length] = ' ';

            token = strtok(NULL, " ");
        }
    }

    length = strlen(buffer[new_line_count]);
    buffer[new_line_count][length - 1] = '\0';
    new_line_count++;

    add_endline(buffer, new_line_count);

    for (i = 0; i < new_line_count; i++) memmove(text[i],
        buffer[i], strlen(buffer[i]) + 1);
    *line_count = new_line_count;
}

void center(char (*text)[1000], int line_count, int start_line, int end_line) {
    int i;
    int max_line_length = 0, length, spaces;

    remove_endline(text, line_count);

    for (i = 0; i < line_count; i++) {
        if (strlen(text[i]) > max_line_length)
            max_line_length = strlen(text[i]);
    }

    for (i = start_line; i < end_line; i++) {
        length = strlen(text[i]);

        if (text[i][0] == '\n') continue;
        if (length == max_line_length) continue;

        spaces = max_line_length - length;
        if (spaces % 2 == 0) {
            memmove(text[i] + spaces / 2, text[i], length + 1);
            memset(text[i], ' ', spaces / 2);
        } else {
            memmove(text[i] + spaces / 2  + 1, text[i], length + 1);
            memset(text[i], ' ', spaces / 2 + 1);
        }
    }

    add_endline(text, line_count);
}

void align_left(char (*text)[1000], int line_count, int start_line,
    int end_line) {
    int i, j;
    int length, counter;

    for (i = start_line; i < end_line; i++) {
        if (text[i][0] == '\n') continue;

        length = strlen(text[i]);
        counter = 0;

        for (j = 0; j < length; j++) {
            if (text[i][j] == ' ') counter++;
            else break;
        }

        memmove(text[i], text[i] + counter, length - counter + 1);
    }

    remove_trailing_whitespace(text, line_count);
    add_endline(text, line_count);
}

void align_right(char (*text)[1000], int line_count, int start_line,
    int end_line) {
    int i;
    int length, max_line_length = 0, spaces;

    remove_endline(text, line_count);

    for (i = 0; i < line_count; i++) {
        if (strlen(text[i]) > max_line_length)
            max_line_length = strlen(text[i]); 
    }

    for (i = start_line; i < end_line; i++) {
        if (text[i][0] == '\n') continue;

        length = strlen(text[i]);

        if (length == max_line_length) continue;

        spaces = max_line_length - length;
        memmove(text[i] + spaces, text[i], length + 1);
        memset(text[i], ' ', spaces);
    }

    remove_trailing_whitespace(text, line_count);
    add_endline(text, line_count);
}

void paragraphs(char (*text)[1000], int line_count,
    int indent_length, int start_line, int end_line) {
    int i;

    for (i = start_line; i < end_line; i++) {
        if (text[i][0] == '\n') continue;

        if (i == 0 || text[i - 1][0] == '\n') {
            memmove(text[i] + indent_length, text[i], strlen(text[i]) + 1);
            memset(text[i], ' ', indent_length);
        }
    }
}

void lists(char (*text)[1000], int line_count, char list_type,
    char special_character, int start_line, int end_line) {
    int i;

    align_left(text, line_count, start_line, end_line);

    if (list_type == 'n') {
        int counter = 1;

        for (i = start_line; i < end_line; i++) {
            if (counter < 10) {
                memmove(text[i] + 3, text[i], strlen(text[i]) + 1);
                text[i][0] = (counter++) + '0';
                text[i][1] = special_character;
                text[i][2] = ' ';
            } else {
                memmove(text[i] + 4, text[i], strlen(text[i]) + 1);
                text[i][0] = counter / 10 + '0';
                text[i][1] = counter % 10 + '0';
                text[i][2] = special_character;
                text[i][3] = ' ';

                counter++;
            }
        }
    }

    if (list_type == 'a' || list_type == 'A') {
        char idx = list_type;

        for (i = start_line; i < end_line; i++) {
            memmove(text[i] + 3, text[i], strlen(text[i]) + 1);
            text[i][0] = idx++;
            text[i][1] = special_character;
            text[i][2] = ' ';
        }
    }

    if (list_type == 'b') {
        for (i = start_line; i < end_line; i++) {
            memmove(text[i] + 2, text[i], strlen(text[i]) + 1);
            text[i][0] = special_character;
            text[i][1] = ' ';
        }
    }

    remove_trailing_whitespace(text, line_count);
    add_endline(text, line_count);
}

void ordered_lists(char (*text)[1000], int line_count, char list_type,
    char special_character, char ordering, int start_line, int end_line) {
    int i, j;

    for (i = start_line; i < end_line - 1; i++) {
        for (j = i; j < end_line; j++) {

            // alphabetically ordered
            if (ordering == 'a' && strcmp(text[i], text[j]) > 0) {
                char buffer[strlen(text[i]) + 1];
                strcpy(buffer, text[i]);
                strcpy(text[i], text[j]);
                strcpy(text[j], buffer);
            }

            if (ordering == 'z' && strcmp(text[i], text[j]) < 0) {
                char buffer[strlen(text[i]) + 1];
                strcpy(buffer, text[i]);
                strcpy(text[i], text[j]);
                strcpy(text[j], buffer);
            }
        }
    }

    lists(text, line_count, list_type, special_character,
        start_line, end_line);
}

int main(int argc, char *argv[]) {
    char buf[1000],               // buffer used for reading from the file
        original[1000][1000],     // original text, line by line
        result[1000][1000];       // text obtained after applying operations
    int original_line_count = 0,  // number of lines in the input file
        result_line_count = 0,    // number of lines in the output file
        i;

    if (argc != 4) {  // invalid number of arguments
        fprintf(stderr,
                "Usage: %s operations_string input_file output_file\n",
                argv[0]);
        return -1;
    }

    // Open input file for reading
    FILE *input_file = fopen(argv[2], "r");

    if (input_file == NULL) {
        fprintf(stderr, "File \"%s\" not found\n", argv[2]);
        return -2;
    }

    // Read data in file line by line
    while (fgets(buf, 1000, input_file)) {
        strcpy(original[original_line_count], buf);
        original_line_count++;
    }

    fclose(input_file);

    char arg[strlen(argv[1]) + 1];
    strcpy(arg, argv[1]);

    char operation;
    int arg1, arg2, arg3;
    char arg1_char, arg2_char, arg3_char;

    char *token = strtok(arg, ",");
    while (token) {

        int no_chars = 0, no_digits = 0;
        for (i = 0; i < strlen(token); i++) {
            if (isalpha(token[i])) {
                if (no_chars == 0) operation = tolower(token[i]);
                no_chars++;
            }

            if (isdigit(token[i]) && isspace(token[i - 1])) no_digits++;
        }

        switch (operation) {
        case 'w':
            if (no_digits > 1) {
                printf("Invalid operation!\n");
                break;
            }

            sscanf(token, " %c %d", &operation, &arg1);
            wrap(original, &original_line_count, arg1);
        break;
        case 'c':
            if (no_digits == 0) center(original, original_line_count,
                                        0, original_line_count);
            if (no_digits == 1) {
                sscanf(token, " %c %d", &operation, &arg1);
                center(original, original_line_count, arg1,
                    original_line_count);
            }
            if (no_digits == 2) {
                sscanf(token, " %c %d %d", &operation, &arg1, &arg2);

                if (arg1 > arg2) {
                    printf("Invalid operation!\n");
                    break;
                }

                if (arg2 < original_line_count) center(original,
                    original_line_count, arg1, arg2 + 1);
                else center(original, original_line_count, arg1,
                    original_line_count);
            }
        break;
        case 'l':
            if (no_digits == 0) align_left(original, original_line_count,
                                            0, original_line_count);
            if (no_digits == 1) {
                sscanf(token, " %c %d", &operation, &arg1);
                align_left(original, original_line_count,
                            arg1, original_line_count);
            }
            if (no_digits == 2) {
                sscanf(token, " %c %d %d", &operation, &arg1, &arg2);

                if (arg2 < original_line_count) align_left(original,
                    original_line_count, arg1, arg2 + 1);
                else align_left(original, original_line_count, arg1,
                    original_line_count);
            }
        break;
        case 'r':
            if (no_digits > 2) {
                printf("Invalid operation!\n");
                break;
            }

            if (no_digits == 0) align_right(original, original_line_count,
                                            0, original_line_count);
            if (no_digits == 1) {
                sscanf(token, " %c %d", &operation, &arg1);
                align_right(original, original_line_count,
                            arg1, original_line_count);
            }
            if (no_digits == 2) {
                sscanf(token, " %c %d %d", &operation, &arg1, &arg2);

                if (arg2 < original_line_count) {
                    align_right(original, original_line_count, arg1, arg2 + 1);
                }
                if (arg2 > original_line_count) {
                    align_right(original, original_line_count,
                                arg1, original_line_count);
                }
            }
        break;
        case 'p':
            if (no_digits == 1) {
                sscanf(token, " %c %d", &operation, &arg1);
                paragraphs(original, original_line_count, arg1,
                            0, original_line_count);
            }
            if (no_digits == 2) {
                sscanf(token, " %c %d %d", &operation, &arg1, &arg2);
                paragraphs(original, original_line_count, arg1,
                            arg2, original_line_count);
            }
            if (no_digits == 3) {
                sscanf(token, " %c %d %d %d", &operation, &arg1, &arg2, &arg3);

                if (arg3 < original_line_count) {
                    paragraphs(original, original_line_count,
                                arg1, arg2, arg3 + 1);
                }
                if (arg3 > original_line_count) {
                    paragraphs(original, original_line_count, arg1,
                                arg2, original_line_count);
                }
            }
        break;
        case 'i':
            sscanf(token, " %c %c %c ", &operation, &arg1_char, &arg2_char);

            if (arg1_char != 'a' && arg1_char != 'A' && arg1_char != 'n'
                && arg1_char != 'b') {
                printf("Invalid operation!\n");
                break;
            }

            if (no_digits == 0) {
                sscanf(token, " %c %c %c", &operation, &arg1_char, &arg2_char);
                lists(original, original_line_count, arg1_char, arg2_char,
                        0, original_line_count);
            }
            if (no_digits == 1) {
                sscanf(token, " %c %c %c %d", &operation,
                        &arg1_char, &arg2_char, &arg1);
                lists(original, original_line_count, arg1_char,
                        arg2_char, arg1, original_line_count);
            }
            if (no_digits == 2) {
                sscanf(token, " %c %c %c %d %d", &operation, &arg1_char,
                        &arg2_char, &arg1, &arg2);

                if (arg2 < original_line_count) {
                    lists(original, original_line_count, arg1_char,
                            arg2_char, arg1, arg2 + 1);
                }
                if (arg2 > original_line_count) {
                    lists(original, original_line_count, arg1_char,
                            arg2_char, arg1, original_line_count);
                }
            }
        break;
        case 'o':
            if (no_digits == 0) {
                sscanf(token, " %c %c %c %c", &operation, &arg1_char,
                        &arg2_char, &arg3_char);
                ordered_lists(original, original_line_count, arg1_char,
                                arg2_char, arg3_char, 0, original_line_count);
            }
            if (no_digits == 1) {
                sscanf(token, " %c %c %c %c %d", &operation, &arg1_char,
                        &arg2_char, &arg3_char, &arg1);
                ordered_lists(original, original_line_count, arg1_char,
                                arg2_char, arg3_char, arg1, original_line_count);
            }
            if (no_digits == 2) {
                sscanf(token, " %c %c %c %c %d %d", &operation,
                        &arg1_char, &arg2_char, &arg3_char, &arg1, &arg2);

                if (arg2 < original_line_count) {
                    ordered_lists(original, original_line_count,
                        arg1_char, arg2_char, arg3_char, arg1, arg2 + 1);
                }
                if (arg2 > original_line_count) {
                    ordered_lists(original, original_line_count, arg1_char,
                        arg2_char, arg3_char, arg1, original_line_count);
                }
            }
        break;
        }
        
        token = strtok(NULL, ",");
    }

    for (i = 0; i < original_line_count; i++) {
        strcpy(result[i], original[i]);
    }
    result_line_count = original_line_count;

    // Open output file for writing
    FILE *output_file = fopen(argv[3], "w");

    if (output_file == NULL) {
        fprintf(stderr, "Could not open or create file \"%s\"\n", argv[3]);
        return -2;
    }

    // Write result in output file
    for (i = 0; i < result_line_count; i++) {
        fputs(result[i], output_file);
    }

    fclose(output_file);
}