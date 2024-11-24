#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>


typedef struct {
    int **mat;   // Двумерный массив для хранения судоку (9x9).
    int N;       // Размерность судоку (например, 9).
    int SRN;     // Квадратный корень из N (например, 3 для 9x9).
    int K;       // Количество чисел, которые нужно удалить (для задания "пустых клеток").
} Sudoku;



// Создание новой структуры Sudoku
Sudoku* createSudoku(int N, int K) {
    Sudoku* sudoku = (Sudoku*)malloc(sizeof(Sudoku)); // Выделяем память для структуры Sudoku
    sudoku->N = N;                                   // Устанавливаем размерность N
    sudoku->K = K;                                   // Устанавливаем количество удаляемых чисел K
    sudoku->SRN = (int)sqrt(N);                      // Вычисляем квадратный корень от N и сохраняем в SRN


    sudoku->mat = (int**)malloc(N * sizeof(int*)); // Выделяем память для строк матрицы
    for (int i = 0; i < N; i++) {
        sudoku->mat[i] = (int*)malloc(N * sizeof(int)); // Выделяем память для каждой строки
        memset(sudoku->mat[i], 0, N * sizeof(int)); // Инициализируем нулями
    }

    return sudoku;
}

// Проверка, можно ли вставить число в квадрат 3x3
int unUsedInBox(Sudoku* sudoku, int rowStart, int colStart, int num) {
    for (int i = 0; i < sudoku->SRN; i++) {
        for (int j = 0; j < sudoku->SRN; j++) {
            if (sudoku->mat[rowStart + i][colStart + j] == num) {
                return 0;
            }
        }
    }
    return 1;
}

// Генерация случайного числа
int randomGenerator(int num) {
    return rand() % num + 1;
}

// Проверка строки на наличие числа
int unUsedInRow(Sudoku* sudoku, int row, int num) {
    for (int j = 0; j < sudoku->N; j++) {
        if (sudoku->mat[row][j] == num) {
            return 0; // Число уже есть в строке
        }
    }
    return 1; // Число отсутствует в строке
}

// Проверка столбца на наличие числа
int unUsedInCol(Sudoku* sudoku, int col, int num) {
    for (int i = 0; i < sudoku->N; i++) {
        if (sudoku->mat[i][col] == num) {
            return 0; // Число уже есть в столбце
        }
    }
    return 1; // Число отсутствует в столбце
}

// Проверка, можно ли вставить число в ячейку
int CheckIfSafe(Sudoku* sudoku, int i, int j, int num) {
    return unUsedInRow(sudoku, i, num) &&
           unUsedInCol(sudoku, j, num) &&
           unUsedInBox(sudoku, i - i % sudoku->SRN, j - j % sudoku->SRN, num);
}

// Заполнение квадрата 3x3
void fillBox(Sudoku* sudoku, int row, int col) {
    int num;
    for (int i = 0; i < sudoku->SRN; i++) {
        for (int j = 0; j < sudoku->SRN; j++) {
            do {
                num = randomGenerator(sudoku->N); // Генерация случайного числа
            } while (!unUsedInBox(sudoku, row, col, num)); // Проверка на уникальность
            sudoku->mat[row + i][col + j] = num; // Вставка числа
        }
    }
}

// Заполнение диагональных квадратов 3x3
void fillDiagonal(Sudoku* sudoku) {
    for (int i = 0; i < sudoku->N; i += sudoku->SRN) {
        fillBox(sudoku, i, i);
    }
}

// Рекурсивное заполнение оставшейся части сетки
int fillRemaining(Sudoku* sudoku, int i, int j) {
    if (j >= sudoku->N && i < sudoku->N - 1) {
        i++;
        j = 0;
    }
    if (i >= sudoku->N && j >= sudoku->N) {
        return 1;
    }
    if (i < sudoku->SRN) {
        if (j < sudoku->SRN) {
            j = sudoku->SRN;
        }
    } else if (i < sudoku->N - sudoku->SRN) {
        if (j == (i / sudoku->SRN) * sudoku->SRN) {
            j += sudoku->SRN;
        }
    } else {
        if (j == sudoku->N - sudoku->SRN) {
            i++;
            j = 0;
            if (i >= sudoku->N) {
                return 1;
            }
        }
    }

    for (int num = 1; num <= sudoku->N; num++) {
        if (CheckIfSafe(sudoku, i, j, num)) {
            sudoku->mat[i][j] = num;
            if (fillRemaining(sudoku, i, j + 1)) {
                return 1;
            }
            sudoku->mat[i][j] = 0;
        }
    }
    return 0;
}

// Удаление K случайных цифр
void removeKDigits(Sudoku* sudoku) {
    int count = sudoku->K;
    while (count > 0) {
        int cellId = randomGenerator(sudoku->N * sudoku->N) - 1;
        int i = cellId / sudoku->N;
        int j = cellId % sudoku->N;
        if (sudoku->mat[i][j] != 0) {
            sudoku->mat[i][j] = 0;
            count--;
        }
    }
}

// Генерация судоку
void fillValues(Sudoku* sudoku) {
    fillDiagonal(sudoku);
    fillRemaining(sudoku, 0, sudoku->SRN);
    removeKDigits(sudoku);
}

// Печать судоку
void printSudoku(Sudoku* sudoku) 
{   
    //printf("---------------------------\n");
    //printf("------------------------------\n");

    for (int i = 0; i < sudoku->N; i++) {

        //printf("||");
        for (int j = 0; j < sudoku->N; j++) {
            if (sudoku->mat[i][j] == 0) {
                printf(". "); // Если клетка пустая, выводим точку
            } else {
                printf("%d ", sudoku->mat[i][j]); // Если клетка заполнена, выводим число
            }
            if ( ((j + 1) % 3) == 0)
            {
                // printf("||");
            }
        }
        printf("\n");
        if ((i + 1) % 3 == 0)
        {
        //printf("--------------------------\n");
        //printf("------------------------------\n");
        }
    }
}

// Освобождение памяти
void freeSudoku(Sudoku* sudoku) {
    for (int i = 0; i < sudoku->N; i++) {
        free(sudoku->mat[i]);
    }
    free(sudoku->mat);
    free(sudoku);
}

// Проверка, можно ли вставить число в ячейку
int isValid(Sudoku* sudoku, int row, int col, int num) {
    // Проверяем строку
    for (int x = 0; x < sudoku->N; x++) {
        if (sudoku->mat[row][x] == num) return 0;
    }

    // Проверяем столбец
    for (int x = 0; x < sudoku->N; x++) {
        if (sudoku->mat[x][col] == num) return 0;
    }

    // Проверяем блок 3x3
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku->mat[i + startRow][j + startCol] == num) return 0;
        }
    }

    return 1;
}

// Печать судоку с отображением текущей позиции курсора
void printSudokuWithCursor(Sudoku* sudoku, int cursorRow, int cursorCol) {
    system("clear"); // Очистка экрана
    for (int i = 0; i < sudoku->N; i++) {
        for (int j = 0; j < sudoku->N; j++) {
            if (i == cursorRow && j == cursorCol) {
                printf("[%c] ", sudoku->mat[i][j] ? sudoku->mat[i][j] + '0' : '.');
            } else {
                printf(" %c  ", sudoku->mat[i][j] ? sudoku->mat[i][j] + '0' : '.');
            }
        }
        printf("\n");
    }
    printf("\nUse W/A/S/D to move, E to edit, Q to quit.\n");
}

// функция автоматического решения судоку с наглядностью

void printSudokuProgress(Sudoku* sudoku) {
            system("clear"); // Очистка экрана для обновления отображения
            printSudoku(sudoku); // Печать текущего состояния
            Sleep(1500); // Небольшая задержка (50 мс) для визуализации процесса
        }

        int solveSudoku(Sudoku* sudoku, int row, int col) {
            // Если дошли до конца, решение найдено
            if (row == sudoku->N - 1 && col == sudoku->N) 
                return 1;
            
            // Если достигли конца строки, переходим на следующую
            if (col == sudoku->N) {
                row++;
                col = 0;
            }
            
            // Пропускаем уже заполненные клетки
            if (sudoku->mat[row][col] != 0)
                return solveSudoku(sudoku, row, col + 1);
            
            // Пробуем вставить числа от 1 до 9
            for (int num = 1; num <= sudoku->N; num++) {
                if (CheckIfSafe(sudoku, row, col, num)) {
                    sudoku->mat[row][col] = num; // Заполняем клетку
                    printSudokuProgress(sudoku); // Визуализация вставки
                    
                    // Рекурсивно переходим к следующей
                    if (solveSudoku(sudoku, row, col + 1))
                        return 1;
                    
                    sudoku->mat[row][col] = 0; // Откатываем, если решение не найдено
                    printSudokuProgress(sudoku); // Визуализация отката
                }
            }
            
            return 0; // Возврат на предыдущий шаг, если не удалось найти решение
        }

int main() {

    srand(time(0));
    int N = 9, K;

    printf("Hey! This sudoku always be 9x9 dimenshion\n");
    printf("How many misses do you want? Interval = [0, 81]\n");
    scanf(" %d", &K);

    Sudoku* sudoku = createSudoku(N, K);
    fillValues(sudoku);
    
    int flag; // 0 - вручную // 1 - auto
    printf("Put 0 or 1, where 0 -> by yourseld 1 -> auto\n");
    scanf(" %d", &flag);

    if (flag == 0) {

        int cursorRow = 0, cursorCol = 0; // Начальная позиция курсора
        char input;

        while (1) {
            printSudokuWithCursor(sudoku, cursorRow, cursorCol);

            // Получаем ввод пользователя
            printf("Enter a command: ");
            scanf(" %c", &input);
            input = toupper(input); // Приводим к верхнему регистру

            if (input == 'Q') {
                // Выход из программы
                break;
            } else if (input == 'W' && cursorRow > 0) {
                cursorRow--; // Вверх
            } else if (input == 'A' && cursorCol > 0) {
                cursorCol--; // Влево
            } else if (input == 'S' && cursorRow < N - 1) {
                cursorRow++; // Вниз
            } else if (input == 'D' && cursorCol < N - 1) {
                cursorCol++; // Вправо
            } else if (input == 'E') {
                // Редактирование текущей клетки
                int num;
                printf("Enter a number (1-9) to place at [%d][%d]: ", cursorRow + 1, cursorCol + 1);
                scanf("%d", &num);

                if (num >= 1 && num <= 9 && isValid(sudoku, cursorRow, cursorCol, num)) {
                    sudoku->mat[cursorRow][cursorCol] = num; // Устанавливаем значение
                } else {
                    printf("Invalid number for this position!\n");
                    getchar(); getchar(); // Пауза перед возвратом
                }
            }
        }

        printf("Exiting...\n");
    }
    if (flag == 1) {

        // Вызываем алгоритм решения
        if (solveSudoku(sudoku, 0, 0)) {
            printf("Solved Sudoku:\n");
            printSudoku(sudoku); // Печать решенной головоломки
        } else {
            printf("No solution exists for the given Sudoku.\n");
        }  
    }
     
    freeSudoku(sudoku);
    return 0;
}
// для комплиции в ручном режиме 
// gcc main.c 
// выбрать и запустить в консоли a.exe

// для компиляции с автоматикой
// gcc main.c -o sudoku_solver -std=c99
