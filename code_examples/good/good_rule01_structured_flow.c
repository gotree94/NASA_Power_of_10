/*
 * Power of 10 준수 예시: 규칙 1 (구조화된 제어 흐름)
 * 
 * 이 코드는 goto문 없이 구조화된 제어 흐름을 사용합니다.
 * 모든 경로가 명확하게 정의되어 있습니다.
 */

#include <stdio.h>

#define MAX_SIZE 100
#define RESULT_SUCCESS 0
#define RESULT_ERROR_INVALID -1
#define RESULT_ERROR_NEGATIVE -2
#define RESULT_ERROR_TOO_LARGE -3

int process_data(int *data, int size) {
    int result = 0;
    int error_code = RESULT_SUCCESS;
    int error_index = -1;
    
    if (data == NULL || size <= 0 || size > MAX_SIZE) {
        error_code = RESULT_ERROR_INVALID;
    } else {
        int i;
        int found_error = 0;
        
        for (i = 0; i < size && !found_error; i++) {
            if (data[i] < 0) {
                error_code = RESULT_ERROR_NEGATIVE;
                error_index = i;
                found_error = 1;
            } else if (data[i] > 1000) {
                error_code = RESULT_ERROR_TOO_LARGE;
                error_index = i;
                found_error = 1;
            } else {
                result += data[i];
            }
        }
    }
    
    if (error_code == RESULT_SUCCESS) {
        printf("Processing complete. Result: %d\n", result);
        return result;
    } else if (error_code == RESULT_ERROR_INVALID) {
        printf("Error: Invalid input parameters\n");
    } else if (error_code == RESULT_ERROR_NEGATIVE) {
        printf("Error: Negative value at index %d\n", error_index);
    } else if (error_code == RESULT_ERROR_TOO_LARGE) {
        printf("Error: Value too large at index %d\n", error_index);
    }
    
    return error_code;
}

int main(void) {
    int data1[] = {10, 20, 30, 40, 50};
    int data2[] = {10, -5, 30, 40, 50};
    int data3[] = {10, 2000, 30, 40, 50};
    
    printf("Test 1: ");
    process_data(data1, 5);
    
    printf("Test 2: ");
    process_data(data2, 5);
    
    printf("Test 3: ");
    process_data(data3, 5);
    
    printf("Test 4: ");
    process_data(NULL, 5);
    
    return 0;
}
