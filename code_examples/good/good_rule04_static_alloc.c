/*
 * Power of 10 준수 예시: 규칙 4 (정적 메모리 할당)
 * 
 * 이 코드는 스택 할당과 정적 배열을 사용합니다.
 * 메모리 누수 위험이 없습니다.
 */

#include <stdio.h>
#include <string.h>

#define MAX_ARRAY_SIZE 1024
#define MAX_STRING_SIZE 256
#define MAX_MATRIX_ROWS 100
#define MAX_MATRIX_COLS 100

// ✅ 정적 배열 사용
int create_array(int *arr, int size) {
    if (arr == NULL || size <= 0 || size > MAX_ARRAY_SIZE) {
        return -1;
    }
    
    for (int i = 0; i < size; i++) {
        arr[i] = i * 2;
    }
    
    return 0;
}

// ✅ 정적 버퍼 사용
int process_string(char *output, int output_size, const char *input) {
    int len;
    int i;
    
    if (output == NULL || input == NULL || output_size <= 0) {
        return -1;
    }
    
    len = strlen(input);
    if (len >= output_size) {
        return -1;
    }
    
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    // 문자열 변환
    for (i = 0; i < len; i++) {
        if (output[i] >= 'a' && output[i] <= 'z') {
            output[i] = output[i] - 'a' + 'A';
        }
    }
    
    return 0;
}

// ✅ 정적 행렬 사용
int create_matrix(int matrix[MAX_MATRIX_ROWS][MAX_MATRIX_COLS], 
                  int rows, int cols) {
    if (matrix == NULL || rows <= 0 || rows > MAX_MATRIX_ROWS ||
        cols <= 0 || cols > MAX_MATRIX_COLS) {
        return -1;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }
    
    return 0;
}

// ✅ 스택 할당 사용
void safe_function(void) {
    char buffer[MAX_STRING_SIZE];
    int local_array[MAX_ARRAY_SIZE];
    
    strncpy(buffer, "Hello", MAX_STRING_SIZE - 1);
    buffer[MAX_STRING_SIZE - 1] = '\0';
    
    int local_sum = 0;
    for (int i = 0; i < 10; i++) {
        local_array[i] = i;
        local_sum += local_array[i];
    }
    
    printf("%s (sum=%d)\n", buffer, local_sum);
    // buffer와 local_array는 함수 종료 시 자동 해제
}

// ✅ 구조체 내 정적 배열
struct DataBuffer {
    int data[MAX_ARRAY_SIZE];
    int size;
    char name[MAX_STRING_SIZE];
};

int init_buffer(struct DataBuffer *buf, const char *name) {
    if (buf == NULL || name == NULL) {
        return -1;
    }
    
    buf->size = 0;
    strncpy(buf->name, name, MAX_STRING_SIZE - 1);
    buf->name[MAX_STRING_SIZE - 1] = '\0';
    
    return 0;
}

int add_to_buffer(struct DataBuffer *buf, int value) {
    if (buf == NULL) {
        return -1;
    }
    
    if (buf->size >= MAX_ARRAY_SIZE) {
        return -1;
    }
    
    buf->data[buf->size] = value;
    buf->size++;
    
    return 0;
}

int main(void) {
    // 테스트 1: 배열 생성
    int arr[MAX_ARRAY_SIZE];
    if (create_array(arr, 5) == 0) {
        for (int i = 0; i < 5; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    
    // 테스트 2: 문자열 처리
    char processed[MAX_STRING_SIZE];
    if (process_string(processed, sizeof(processed), "hello world") == 0) {
        printf("%s\n", processed);
    }
    
    // 테스트 3: 행렬 생성
    int matrix[MAX_MATRIX_ROWS][MAX_MATRIX_COLS];
    if (create_matrix(matrix, 3, 3) == 0) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }
    
    safe_function();
    
    // 테스트 4: 구조체 사용
    struct DataBuffer buffer;
    init_buffer(&buffer, "Test Buffer");
    add_to_buffer(&buffer, 10);
    add_to_buffer(&buffer, 20);
    add_to_buffer(&buffer, 30);
    
    printf("Buffer '%s' contains %d items: ", buffer.name, buffer.size);
    for (int i = 0; i < buffer.size; i++) {
        printf("%d ", buffer.data[i]);
    }
    printf("\n");
    
    return 0;
}
