#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

void show_board(uint64_t board) {
    uint64_t mask = 1;
    mask = mask << 63;
    int counter = 0, i;
    for (i = 64; i > 0; i--) {
        if ((board & mask) == 0) {
            printf(".");
        } else {
            printf("#");
        }
        mask = mask >> 1;
        counter++;
        if (counter % 8 == 0) printf("\n");
    }
    printf("\n");
}

int type_of_piece(uint64_t piece) {
    uint64_t mask = 1;
    int counter = 0, i;
    for (i = 0; i < 64; i++) {
        if ((mask & piece) != 0) {
            counter++;
        }
        mask <<= 1;
    }

    switch (counter) {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
    }
}

void move_piece(uint64_t *piece, int shift) {
    uint16_t max;
    if (*piece <= 255) {
        max = UINT8_MAX;
    } else {
        max = UINT16_MAX;
    }
    
    int mask = 1;

    int type = type_of_piece(*piece);
    while (shift < 0) {
        // safety check for type 3 pieces
        if (type == 3) {
            mask <<= 15;
            if ((mask & *piece) == 0 && (mask >> 8 & *piece) != 0) return;
        }

        // general case that doesn't work for some type 3 pieces
        if (max > (*piece << 1)) {
            *piece = *piece << 1;
            shift++;
        } else return;
    }

    mask = 1;
    while (shift > 0) {
        // safety check for type 3 pieces
        if (type == 3) {
            if ((mask & *piece) == 0 && (mask << 8 & *piece) != 0) return;
        }

        // general case that works for type 1 2 4 (and sometimes 3) pieces
        if ((*piece & mask) == 0) {
            *piece = *piece >> 1;
            shift--;
        } else return;
    }
}

uint64_t shift_down(uint64_t board, uint64_t *piece,
    int line, int shift) {
    uint64_t first_copy, second_copy = board, third_copy;

    if (line > 0) {
        second_copy = *piece << 56 - (line - 1) * 8;
        second_copy = ~second_copy;
    }
    board &= second_copy;

    first_copy = *piece << 56 - line * 8;
    if ((board & first_copy) != 0) return UINT64_MAX;

    if (shift != 0) {
        first_copy = *piece << 56 - line * 8;
        if (shift > 0) {
            while ((first_copy >> 1 & board) == 0 && shift != 0) {
                move_piece(piece, 1);
                shift--;
                first_copy = *piece << 56 - line * 8;
            }
        } else if (shift < 0) {
            while ((first_copy << 1 & board) == 0 && shift != 0) {
                move_piece(piece, -1);
                shift++;
                first_copy = *piece << 56 - line * 8;
            }
        }
    }

    board |= first_copy;
    return board;
}

int* check_full_lines(uint64_t board) {
    int *full_lines, counter = 0, i;
    full_lines = calloc(8, sizeof(int));
    uint64_t mask = 1;

    for (i = 0; i < 64; i++) {
        if (i % 8 == 0) counter = 0;
        if ((mask & board) != 0) counter++;
        if (counter % 8 == 0 && counter != 0) full_lines[i % 7] = 1;

        mask <<= 1;
    }

    return full_lines;
}

uint64_t delete_full_lines(uint64_t board, int *nr_of_deleted_lines) {
    int *full_lines = check_full_lines(board);
    int i;

    i = 1;
    while (i < 8) {
        if (full_lines[i] == 1) {
            (*nr_of_deleted_lines)++;
            uint64_t upper_board, lower_board;

            lower_board = board << (7 - i + 1) * 8;
            upper_board = board >> (i + 1) * 8;
 
            lower_board >>= (7 - i + 1) * 8;
            upper_board <<= i * 8;

            board = upper_board | lower_board;

            free(full_lines);
            full_lines = NULL;
            full_lines = check_full_lines(board);
            i = 0;
        }
        i++;    
    }

    if (full_lines[0] == 1) {
        (*nr_of_deleted_lines)++;
        board >>= 8;
    }

    free(full_lines);
    full_lines = NULL;
    return board;
}

float score(uint64_t board, int deleted_lines) {
    uint64_t mask = 1;
    int number_zeros = 0, i;
    for (i = 0; i < 64; i++) {
        if ((mask & board) == 0) number_zeros++;
        mask <<= 1;
    }

    float score = sqrt(number_zeros) + pow(1.25, deleted_lines);
    return score;
}

int main() {
    uint64_t board;
    scanf("%lu", &board);
    show_board(board);

    int movements;
    scanf("%d", &movements);

    int i, j;
    uint64_t copied_board = board;
    int nr_of_deleted_lines = 0;
    for (i = 0; i < movements; i++) {
        uint64_t piece;
        scanf("%lu", &piece);

        for (j = 0; j < 8; j++) {
            int shift;
            scanf("%d", &shift);
            copied_board = board;
            board = shift_down(board, &piece, j, shift);
            if (board == UINT64_MAX) {
                board = copied_board;
                break;
            }
            show_board(board);
        }

        uint64_t deleted_board = delete_full_lines(board, &nr_of_deleted_lines);
        if (deleted_board != board) {
            board = deleted_board;
            show_board(board);
        }
    }

    printf("GAME OVER!\n");
    printf("Score:%0.2f\n", score(board, nr_of_deleted_lines));

    return 0;
}