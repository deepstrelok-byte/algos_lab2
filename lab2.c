#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 20
#define INITIAL_CAPACITY 20

// Структура (Вариант 8): Комбинированный ключ (строка + целое), хранение вместе
typedef struct {
    char keyStr[MAX_STR_LEN];
    int keyInt;
} TableItem;

// Функция сравнения: сначала по строке, потом по числу
int compareItems(const TableItem *a, const TableItem *b) {
    int strRes = strcmp(a->keyStr, b->keyStr);
    if (strRes != 0) {
        return strRes;
    }
    return a->keyInt - b->keyInt;
}

// Вывод таблицы
void printTable(TableItem *table, int size, const char *message) {
    printf("\n--- %s ---\n", message);
    printf("| %-15s | %-10s |\n", "String Key", "Int Key");
    printf("|-----------------|------------|\n");
    for (int i = 0; i < size; i++) {
        printf("| %-15s | %-10d |\n", table[i].keyStr, table[i].keyInt);
    }
    printf("--------------------------------\n");
}

// Сортировка "Линейный выбор с подсчетом" (Comparison Counting Sort)
void countingSort(TableItem *table, int size) {
    TableItem *result = (TableItem *)malloc(size * sizeof(TableItem));
    int *count = (int *)malloc(size * sizeof(int));
    
    // Обнуляем счетчики
    for (int i = 0; i < size; i++) count[i] = 0;

    // Считаем, сколько элементов меньше текущего
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (compareItems(&table[i], &table[j]) > 0) {
                count[i]++;
            } else {
                count[j]++;
            }
        }
    }

    // Расставляем по местам
    for (int i = 0; i < size; i++) {
        result[count[i]] = table[i];
    }

    // Копируем обратно
    for (int i = 0; i < size; i++) {
        table[i] = result[i];
    }

    free(result);
    free(count);
}

// Двоичный поиск
int binarySearch(TableItem *table, int size, TableItem key) {
    int left = 0;
    int right = size - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = compareItems(&table[mid], &key);
        
        if (cmp == 0) return mid;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main() {
    int capacity = INITIAL_CAPACITY;
    int size = 0;
    TableItem *table = (TableItem *)malloc(capacity * sizeof(TableItem));
    
    char bufferStr[MAX_STR_LEN];
    int bufferInt;

    printf("Reading table data from input (stop with 'END_TABLE' string)...\n");

    // ЧТЕНИЕ ТАБЛИЦЫ
    while (scanf("%s", bufferStr) == 1) {
        // Проверка на разделитель конца таблицы
        if (strcmp(bufferStr, "END_TABLE") == 0) {
            break;
        }
        
        // Считываем числовую часть ключа
        if (scanf("%d", &bufferInt) != 1) {
            break; // Ошибка формата или конец файла
        }

        // Динамическое расширение массива
        if (size >= capacity) {
            capacity *= 2;
            table = (TableItem *)realloc(table, capacity * sizeof(TableItem));
        }

        strcpy(table[size].keyStr, bufferStr);
        table[size].keyInt = bufferInt;
        size++;
    }

    // 1. Вывод исходной
    printTable(table, size, "Source Table");

    // 2. Сортировка
    countingSort(table, size);

    // 3. Вывод отсортированной
    printTable(table, size, "Sorted Table");

    // 4. Поиск (продолжаем читать из того же потока)
    printf("\nStarting Search Phase (reading remaining input)...\n");
    
    while (scanf("%s", bufferStr) == 1) {
        if (scanf("%d", &bufferInt) != 1) break;

        TableItem searchKey;
        strcpy(searchKey.keyStr, bufferStr);
        searchKey.keyInt = bufferInt;

        int index = binarySearch(table, size, searchKey);
        
        if (index != -1) {
            printf("[FOUND] Key '%s %d' at index %d\n", bufferStr, bufferInt, index);
        } else {
            printf("[NOT FOUND] Key '%s %d'\n", bufferStr, bufferInt);
        }
    }

    free(table);
    return 0;
}