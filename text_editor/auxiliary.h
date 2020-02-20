void add_endline(char (*text)[1000], int line_count) {
    int i;
    int length;

    for (i = 0; i < line_count; i++) {
        if (text[i][0] == '\n') continue;

        length = strlen(text[i]);

        if (text[i][length - 1] != '\n') {
            text[i][length] = '\n';
            text[i][length + 1] = '\0'; 
        }
    }
}

void remove_endline(char (*text)[1000], int line_count) {
    int i;
    int length;

    for (i = 0; i < line_count; i++) {
        if (text[i][0] == '\n') continue;

        length = strlen(text[i]);
        if (text[i][length - 1] == '\n') text[i][length - 1] = '\0';
    }
}

void remove_trailing_whitespace(char (*text)[1000], int line_count) {
    int i, j;
    int length, counter, end_of_line;

    for (i = 0; i < line_count; i++) {
        if (text[i][0] == '\n') continue;

        length = strlen(text[i]);
        counter = 0;
        end_of_line = 0;

        for (j = length - 1; j >= 0; j--) {
            if (text[i][j] == '\n') {
                end_of_line++;
                continue;
            }

            if (text[i][j] == ' ' || text[i][j] == '\t') counter++;
            else break; 
        }

        text[i][length - counter - end_of_line] = '\0';
    }
}

void align_left_paragraphs(char (*text)[1000], int line_count) {
    int i;

    for (i = 0; i < line_count; i++) {
        if (i == 0 || text[i - 1][0] == '\n') continue;

        align_left(text, line_count, i, i + 1);
    }
}

void show(char (*text)[1000], int line_count) {
    int i;
    
    for (i = 0; i < line_count; i++) {
        printf("%s %ld", text[i], strlen(text[i]));
        int length = strlen(text[i]);
        if (text[i][length] == '\0') printf("$$$");
        else printf("&&&");
    }
}

int longest_word(char (*text)[1000], int line_count, int max_line_length) {
    int i;

    char buffer[1000][1000];
    for (i = 0; i < line_count; i++) strcpy(buffer[i], text[i]);

    remove_endline(buffer, line_count);

    int longest_word = 0;
    char *token = NULL;
    for (i = 0; i < line_count; i++) {
        token = strtok(buffer[i], " ");

        while (token) {
            if (strlen(token) < longest_word)
                longest_word = strlen(token);
        }
    }

    return longest_word;
}