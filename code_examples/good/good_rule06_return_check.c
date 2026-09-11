/*
 * Power of 10 준수 예시: 규칙 6 (반환값 검증)
 * 
 * 이 코드는 모든 함수의 반환값을 확인하고 매개변수를 검증합니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define MAX_ARRAY_SIZE 1000

// ✅ 반환값 및 매개변수 검증
int good_file_processing(void) {
    FILE *fp = fopen("data.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file\n");
        return -1;
    }
    
    char buffer[MAX_BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fprintf(stderr, "Error: Cannot read file\n");
        fclose(fp);
        return -1;
    }
    
    if (fclose(fp) != 0) {
        fprintf(stderr, "Error: Cannot close file\n");
        return -1;
    }
    
    printf("Data: %s\n", buffer);
    return 0;
}

// ✅ 매개변수 검증
int good_array_sum(const int *data, int size, int *sum) {
    if (data == NULL || sum == NULL) {
        return -1;
    }
    if (size <= 0 || size > MAX_ARRAY_SIZE) {
        return -1;
    }
    
    int total = 0;
    for (int i = 0; i < size; i++) {
        total += data[i];
    }
    
    *sum = total;
    return 0;
}

// ✅ 안전한 문자열 처리
int good_string_copy(char *dest, int dest_size, const char *src) {
    if (dest == NULL || src == NULL) {
        return -1;
    }
    if (dest_size <= 0) {
        return -1;
    }
    
    int result = snprintf(dest, dest_size, "%s", src);
    if (result < 0) {
        return -1;
    }
    
    return 0;
}

// ✅ 안전한 입력 처리
int good_input_processing(int *value) {
    if (value == NULL) {
        return -1;
    }
    
    printf("Enter a number: ");
    if (scanf("%d", value) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return -1;
    }
    
    return 0;
}

// ✅ 안전한 문자열 변환
int safe_string_to_int(const char *str, int *result) {
    long temp;
    char *endptr;
    
    if (str == NULL || result == NULL) {
        return -1;
    }
    
    temp = strtol(str, &endptr, 10);
    
    if (*endptr != '\0') {
        return -1;
    }
    
    if (temp < -MAX_ARRAY_SIZE || temp > MAX_ARRAY_SIZE) {
        return -1;
    }
    
    *result = (int)temp;
    return 0;
}

// ✅ 모든 함수 호출 검증
int good_comprehensive_example(void) {
    FILE *fp = fopen("test.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot create file\n");
        return -1;
    }
    
    if (fprintf(fp, "Hello") < 0) {
        fprintf(stderr, "Error: Cannot write to file\n");
        fclose(fp);
        return -1;
    }
    
    char buffer[MAX_BUFFER_SIZE];
    printf("Enter text: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error: Cannot read input\n");
        fclose(fp);
        return -1;
    }
    
    int arr[5];
    int arr_size = 5;
    int arr_sum = 0;
    for (int i = 0; i < arr_size; i++) {
        arr[i] = i;
        arr_sum += arr[i];
    }
    
    if (fclose(fp) != 0) {
        fprintf(stderr, "Error: Cannot close file\n");
        return -1;
    }
    
    printf("Operation completed successfully (arr_sum=%d)\n", arr_sum);
    return 0;
}

// ✅ 체이닝된 검증
int chained_validation(void) {
    const char *filename = "test.txt";
    FILE *fp;
    char line[MAX_BUFFER_SIZE];
    int line_count = 0;
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open %s\n", filename);
        return -1;
    }
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_count++;
        printf("Line %d: %s", line_count, line);
    }
    
    if (ferror(fp)) {
        fprintf(stderr, "Error reading file\n");
        fclose(fp);
        return -1;
    }
    
    if (fclose(fp) != 0) {
        fprintf(stderr, "Error closing file\n");
        return -1;
    }
    
    printf("Processed %d lines\n", line_count);
    return 0;
}

int main(void) {
    // good_file_processing();  // 파일이 없을 수 있으므로 주석 처리
    
    int data[] = {1, 2, 3, 4, 5};
    int sum;
    if (good_array_sum(data, 5, &sum) == 0) {
        printf("Sum: %d\n", sum);
    }
    
    char dest[MAX_BUFFER_SIZE];
    if (good_string_copy(dest, sizeof(dest), "Hello, World!") == 0) {
        printf("%s\n", dest);
    }
    
    // good_input_processing은 사용자 입력이 필요하므로 주석 처리
    
    int converted;
    if (safe_string_to_int("123", &converted) == 0) {
        printf("Converted: %d\n", converted);
    }
    
    // good_comprehensive_example();
    
    chained_validation();
    
    return 0;
}
