/*
 * Power of 10 위반 예시: 규칙 1 (goto 사용)
 * 
 * 이 코드는 goto문을 사용하여 복잡한 제어 흐름을 만듭니다.
 * Power of 10에서는 goto 사용을 금지합니다.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

int process_data(int *data, int size) {
    int result = 0;
    int i;
    
    if (data == NULL || size <= 0 || size > MAX_SIZE) {
        goto error_invalid;
    }
    
    for (i = 0; i < size; i++) {
        if (data[i] < 0) {
            goto error_negative;
        }
        if (data[i] > 1000) {
            goto error_too_large;
        }
        result += data[i];
    }
    
    printf("Processing complete. Result: %d\n", result);
    return result;

error_invalid:
    printf("Error: Invalid input parameters\n");
    return -1;

error_negative:
    printf("Error: Negative value at index %d\n", i);
    return -2;

error_too_large:
    printf("Error: Value too large at index %d\n", i);
    return -3;
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
