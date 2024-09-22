#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[34m"
#define GREEN_COLOR "\033[32m"
#define CYAN_COLOR "\033[36m"

// Функция для обработки файлов и получения информации о них (например, для опции -l)
void print_file_info(const char *filename, struct stat *file_stat, int long_format) {
    if (long_format) {
        // Вывод прав доступа
        printf((S_ISDIR(file_stat->st_mode)) ? "d" : "-");
        printf((file_stat->st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat->st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat->st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat->st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat->st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat->st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat->st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat->st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat->st_mode & S_IXOTH) ? "x" : "-");

        // Вывод количества ссылок, владельца и группы
        printf(" %ld", file_stat->st_nlink);
        printf(" %s", getpwuid(file_stat->st_uid)->pw_name);
        printf(" %s", getgrgid(file_stat->st_gid)->gr_name);

        // Вывод размера файла
        printf(" %ld", file_stat->st_size);

        // Вывод времени последней модификации
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_stat->st_mtime));
        printf(" %s", time_str);
    }

    // Цветной вывод имени файла в зависимости от типа
    if (S_ISDIR(file_stat->st_mode)) {
        printf(" %s%s%s\n", BLUE_COLOR, filename, RESET_COLOR);
    } else if (file_stat->st_mode & S_IXUSR) {
        printf(" %s%s%s\n", GREEN_COLOR, filename, RESET_COLOR);
    } else if (S_ISLNK(file_stat->st_mode)) {
        printf(" %s%s%s\n", CYAN_COLOR, filename, RESET_COLOR);
    } else {
        printf(" %s\n", filename);
    }
}

// Функция для сравнения строк (для сортировки)
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char *argv[]) {
    int opt;
    int long_format = 0, all_files = 0;

    // Обработка опций командной строки с помощью getopt()
    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch (opt) {
            case 'l':
                long_format = 1;
                break;
            case 'a':
                all_files = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-a] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Путь к директории, если указан
    const char *dir_path = (optind < argc) ? argv[optind] : ".";

    // Открытие директории
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    char **files = NULL;
    size_t file_count = 0;

    // Чтение содержимого директории
    while ((entry = readdir(dir)) != NULL) {
        // Пропуск скрытых файлов, если опция -a не указана
        if (!all_files && entry->d_name[0] == '.') {
            continue;
        }

        files = realloc(files, sizeof(char *) * (file_count + 1));
        files[file_count] = strdup(entry->d_name);
        file_count++;
    }
    closedir(dir);

    // Сортировка файлов в алфавитном порядке
    qsort(files, file_count, sizeof(char *), compare);

    // Вывод файлов
    for (size_t i = 0; i < file_count; i++) {
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, files[i]);

        struct stat file_stat;
        if (stat(filepath, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        print_file_info(files[i], &file_stat, long_format);
        free(files[i]);
    }
    free(files);

    return 0;
}
