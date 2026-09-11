/*
 * Power of 10 위반 예시: 규칙 4 (동적 메모리 할당)
 * 
 * 이 코드는 malloc/free를 사용하여 동적 메모리 할당을 합니다.
 * 메모리 누수 및 파편화 위험이 있습니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ❌ 동적 할당을 사용한 배열 생성
int* create_array(int size) {
    int *arr = malloc(size * sizeof(int));
    if (arr == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        arr[i] = i * 2;
    }
    
    return arr;
    // 호출자가 free()하지 않으면 메모리 누수!
}

// ❌ 동적 할당을 사용한 문자열 처리
char* process_string(const char *input) {
    int len = strlen(input);
    char *buffer = malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }
    
    strcpy(buffer, input);
    
    // 문자열 변환
    for (int i = 0; i < len; i++) {
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            buffer[i] = buffer[i] - 'a' + 'A';
        }
    }
    
    return buffer;
    // 호출자가 free()하지 않으면 메모리 누수!
}

// ❌ 동적 할당을 사용한 2차원 배열
int** create_matrix(int rows, int cols) {
    int **matrix = malloc(rows * sizeof(int*));
    if (matrix == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            // 이전에 할당된 메모리 누수!
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    
    return matrix;
}

// ❌ 메모리 누수가 있는 함수
void leaky_function(void) {
    char *str = malloc(100);
    strcpy(str, "Hello");
    printf("%s\n", str);
    // free(str) 누락!
}

// ❌ 재할당으로 인한 메모리 누수
void reallocation_leak(void) {
    char *buffer = malloc(50);
    strcpy(buffer, "Initial");
    
    // 새 할당 후 이전 할당 해제하지 않음!
    buffer = realloc(buffer, 100);
    // realloc이 새 메모리를 반환하면 이전 메모리 누수
}

int main(void) {
    // 테스트 1: 배열 생성
    int *arr = create_array(5);
    if (arr != NULL) {
        for (int i = 0; i < 5; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        free(arr);
    }
    
    // 테스트 2: 문자열 처리
    char *processed = process_string("hello world");
    if (processed != NULL) {
        printf("%s\n", processed);
        free(processed);
    }
    
    // 테스트 3: 행렬 생성
    int **matrix = create_matrix(3, 3);
    if (matrix != NULL) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                matrix[i][j] = i * 3 + j;
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        
        for (int i = 0; i < 3; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
    
    leaky_function();
    reallocation_leak();
    
    return 0;
}
