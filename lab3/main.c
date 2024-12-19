#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Переменная для отслеживания состояния дочернего процесса
pid_t child_pid = -1;

// Обработчик сигналов
void signal_handler(int signal) {
    switch (signal) {
        case SIGINT:
            printf("\nПолучен сигнал SIGINT (Ctrl+C)\n");
            // Если дочерний процесс существует, убиваем его
            if (child_pid > 0) {
                kill(child_pid, SIGTERM);
                printf("Дочерний процесс (PID: %d) завершен.\n", child_pid);
            }
            exit(EXIT_SUCCESS); // Завершаем родительский процесс
            break;
        case SIGTERM:
            printf("\nПолучен сигнал SIGTERM\n");
            exit(EXIT_SUCCESS); // Завершаем программу
            break;
        default:
            printf("\nПолучен сигнал: %d\n", signal);
            break;
    }
}

// Обработчик выхода
void exit_handler() {
    printf("Программа завершает работу\n");
}

int main() {
    // Регистрация обработчиков сигналов
    signal(SIGINT, signal_handler); // Для SIGINT
    struct sigaction sa;
    sa.sa_handler = signal_handler; // Установка обработчика для SIGTERM
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);

    // Регистрация обработчика выхода
    atexit(exit_handler);

    // Создание нового процесса
    child_pid = fork();

    if (child_pid < 0) {
        // Ошибка при создании процесса
        perror("Ошибка fork()");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Код, выполняемый дочерним процессом
        printf("Дочерний процесс (PID: %d) создан.\n", getpid());
        for (int i = 0; i < 5; i++) {
            printf("Дочерний процесс: работа... (%d)\n", i + 1);
            sleep(5); // Имитация работы
        }
        exit(EXIT_SUCCESS); // Завершение дочернего процесса
    } else {
        // Код, выполняемый родительским процессом
        printf("Родительский процесс (PID: %d) создал дочерний процесс (PID: %d).\n", getpid(), child_pid);

        // Ожидание завершения дочернего процесса
        int status;
        waitpid(child_pid, &status, 0); // Ждем завершения дочернего процесса

        // Проверка статуса завершения
        if (WIFEXITED(status)) {
            printf("Дочерний процесс завершился с кодом: %d\n", WEXITSTATUS(status));
        } else {
            printf("Дочерний процесс завершился ненормально.\n");
        }
    }

    return 0;
}
