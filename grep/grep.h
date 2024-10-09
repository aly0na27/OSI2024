typedef struct arguments {
    int e, i, v, c, l, n, h, s, f, o;
    char pattern[1024];
    int len_pattern;
    char output_file[1024]; // Добавляем поле для имени выходного файла
} arguments;
