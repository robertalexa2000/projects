// auxiliary functions
void add_endline(char (*text)[1000], int line_count);
void remove_endline(char (*text)[1000], int line_count);
void remove_trailing_whitespace(char (*text)[1000], int line_count);
void align_left_paragraphs(char (*text)[1000], int line_count);
void show(char (*text)[1000], int line_count);
int longest_word(char (*text)[1000], int line_count, int max_line_length);

// implementation functions
void wrap(char (*text)[1000], int *line_count, int max_line_length);
void center(char (*text)[1000], int line_count,
    int start_line, int end_line);
void align_left(char (*text)[1000], int line_count,
    int start_line, int end_line);
void align_right(char (*text)[1000], int line_count,
    int start_line, int end_line);
void paragraphs(char (*text)[1000], int line_count,
    int indent_length, int start_line, int end_line);
void lists(char (*text)[1000], int line_count, char list_type,
    char special_character, int start_line, int end_line);
void ordered_lists(char (*text)[1000], int line_count, char list_type,
    char special_character, char ordering, int start_line, int end_line);