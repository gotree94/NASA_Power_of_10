/*
 * Power of 10 준수 예시: 규칙 5 (좁은 범위)
 * 
 * 이 코드는 변수를 사용하는 가장 좁은 범위에서 선언합니다.
 */

#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

// ✅最小한 전역 변수
static int process_counter = 0;

void process_data1(const int *data, int size) {
    int local_result = 0;  // ✅函数内声明
    
    if (data == NULL || size <= 0) {
        return;
    }
    
    // ✅ for 루프 내에서 i 선언
    for (int i = 0; i < size; i++) {
        local_result += data[i];
    }
    
    process_counter++;
    
    // ✅ 로컬 버퍼 사용
    char local_buffer[MAX_BUFFER_SIZE];
    snprintf(local_buffer, MAX_BUFFER_SIZE, "Processed %d items", size);
    printf("%s\n", local_buffer);
    
    // local_result와 local_buffer는 함수 종료 시 소멸
}

void process_data2(const int *data, int size) {
    int local_result = 0;  // ✅函数内声明
    
    if (data == NULL || size <= 0) {
        return;
    }
    
    // ✅ for 루프 내에서 i 선언
    for (int i = 0; i < size; i++) {
        local_result -= data[i];
    }
    
    process_counter--;
    
    printf("Subtracted total: %d\n", local_result);
}

void another_function(void) {
    // ✅ 이전 함수의 변수에 접근 불가
    int local_data = 42;  // 이 함수에서만 유효
    
    printf("Counter: %d\n", process_counter);
    printf("Local data: %d\n", local_data);
}

void function_with_narrow_scope(void) {
    int x = 10;
    
    // ✅ 필요한 곳에서만 선언
    if (x > 5) {
        int a = 100;  // ✅ if 블록 안에서만 선언
        int b = 200;  // ✅ if 블록 안에서만 선언
        printf("%d\n", a + b);
    }
    // a와 b는 여기서 접근 불가
    
    // ✅ for 루프 내에서만 필요한 변수 선언
    for (int i = 0; i < 10; i++) {
        int temp = i * 2;  // ✅ for 루프 안에서만 선언
        printf("%d ", temp);
    }
    printf("\n");
    // i와 temp는 여기서 접근 불가
    
    // ✅ switch case 내에서만 필요한 변수 선언
    switch (x) {
        case 10: {
            int case_value = 1;  // ✅ case 블록 안에서만 선언
            printf("Case 1: %d\n", case_value);
            break;
        }
        case 20: {
            int case_value = 2;  // ✅ 각 case마다 독립적
            printf("Case 2: %d\n", case_value);
            break;
        }
        default:
            printf("Default\n");
            break;
    }
}

// ✅nested blocks for scope control
void nested_scope_example(void) {
    int outer = 1;
    
    {
        int middle = 2;
        
        {
            int inner = 3;
            printf("%d %d %d\n", outer, middle, inner);
        }
        // inner는 여기서 접근 불가
    }
    // middle은 여기서 접근 불가
}

int main(void) {
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {10, 20, 30, 40, 50};
    
    process_data1(data1, 5);
    process_data2(data2, 5);
    another_function();
    function_with_narrow_scope();
    nested_scope_example();
    
    return 0;
}
