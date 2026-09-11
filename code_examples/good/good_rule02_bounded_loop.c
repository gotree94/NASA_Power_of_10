/*
 * Power of 10 준수 예시: 규칙 2 (고정된 반복문 경계)
 * 
 * 이 코드는 모든 루프에 명확한 최대 반복 횟수를 지정합니다.
 * 제어 불능 코드를 방지합니다.
 */

#include <stdio.h>
#include <string.h>

#define MAX_ITERATIONS 10000
#define MAX_STRING_LENGTH 10000

int find_character(const char *text, char target, int *position) {
    int len;
    int i;
    
    if (text == NULL || position == NULL) {
        return -1;
    }
    
    len = strlen(text);
    if (len > MAX_STRING_LENGTH) {
        len = MAX_STRING_LENGTH;
    }
    
    *position = -1;
    
    for (i = 0; i < len; i++) {
        if (text[i] == target) {
            *position = i;
            return 0;
        }
    }
    
    return -1;
}

int sum_until_negative(int *array, int size, int *sum) {
    int i;
    int total = 0;
    
    if (array == NULL || sum == NULL || size <= 0) {
        return -1;
    }
    
    if (size > MAX_ITERATIONS) {
        size = MAX_ITERATIONS;
    }
    
    for (i = 0; i < size; i++) {
        if (array[i] < 0) {
            break;
        }
        total += array[i];
    }
    
    *sum = total;
    return 0;
}

int process_stream(int max_iterations) {
    int count = 0;
    int limit;
    
    if (max_iterations <= 0 || max_iterations > MAX_ITERATIONS) {
        limit = MAX_ITERATIONS;
    } else {
        limit = max_iterations;
    }
    
    for (count = 0; count < limit; count++) {
        // 처리 로직
    }
    
    return count;
}

int main(void) {
    char text[] = "Hello, World!";
    int position;
    
    if (find_character(text, 'W', &position) == 0) {
        printf("Found 'W' at index %d\n", position);
    } else {
        printf("Character not found\n");
    }
    
    int numbers[] = {1, 2, 3, 4, 5, -1};
    int sum;
    int size = sizeof(numbers) / sizeof(numbers[0]);
    
    if (sum_until_negative(numbers, size, &sum) == 0) {
        printf("Sum: %d\n", sum);
    }
    
    int processed = process_stream(500);
    printf("Processed %d iterations\n", processed);
    
    return 0;
}
