/*
 * Power of 10 위반 예시: 규칙 5 (넓은 범위)
 * 
 * 이 코드는 변수를不必要的 넓은 범위에서 선언합니다.
 */

#include <stdio.h>
#include <string.h>

// ❌不必要的 전역 변수
int global_counter = 0;
int global_result = 0;
char global_buffer[256];
int global_temp;

void process_data1(int *data, int size) {
    int i, j, k;  // ❌不必要的广阔的范围
    
    // ❌ i가 for 루프 밖에서 선언됨
    for (i = 0; i < size; i++) {
        global_result += data[i];
    }
    
    // ❌ 전역 변수 사용
    global_counter++;
    
    // ❌ 전역 버퍼 사용
    sprintf(global_buffer, "Processed %d items", size);
}

void process_data2(int *data, int size) {
    int i;  // ❌不必要的广阔的范围
    
    // ❌ i가 for 루프 밖에서 선언됨
    for (i = 0; i < size; i++) {
        global_result -= data[i];
    }
    
    // ❌ 전역 변수 사용
    global_counter--;
}

void another_function(void) {
    // ❌ 이전 함수의 i를 사용할 수 있음 (의도치 않게)
    // global_result도 여전히 유효
    printf("Counter: %d\n", global_counter);
    
    // ❌不必要的 전역 임시 변수
    global_temp = 42;
}

void function_with_unnecessary_scope(void) {
    int x = 10;
    int y = 20;
    int z = 30;
    int temp;
    
    // ❌不必要的广阔的范围
    if (x > 5) {
        int a = 100;
        int b = 200;
        // a와 b는 if 블록 안에서만 사용됨
        //但是函数的开头就声明了
        printf("%d\n", a + b);
    }
    
    // ❌不必要的广阔的范围
    for (int i = 0; i < 10; i++) {
        temp = i * 2;
        printf("%d ", temp);
    }
    printf("\n");
    
    // ❌不必要的广阔的范围
    switch (x) {
        case 10:
            {
                int case_value = 1;
                printf("Case 1: %d\n", case_value);
            }
            break;
        case 20:
            {
                int case_value = 2;
                printf("Case 2: %d\n", case_value);
            }
            break;
        default:
            printf("Default\n");
            break;
    }
}

int main(void) {
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {10, 20, 30, 40, 50};
    
    process_data1(data1, 5);
    process_data2(data2, 5);
    another_function();
    function_with_unnecessary_scope();
    
    return 0;
}
