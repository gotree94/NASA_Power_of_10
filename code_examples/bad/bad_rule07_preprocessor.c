/*
 * Power of 10 위반 예시: 규칙 7 (전처리기 매크로)
 * 
 * 이 코드는 함수형 매크로를 사용합니다.
 * 타입 안전성 부족 및 예상치 못한 부작용이 있습니다.
 */

#include <stdio.h>
#include <math.h>

// ❌ 함수형 매크로
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

// ❌ 복잡한 매크로
#define PRINT_ARRAY(arr, size) \
    do { \
        for (int i = 0; i < size; i++) { \
            printf("%d ", arr[i]); \
        } \
        printf("\n"); \
    } while(0)

// ❌ 조건문 매크로
#define CHECK_AND_PRINT(val) \
    if (val > 0) { \
        printf("Positive: %d\n", val); \
    } else if (val < 0) { \
        printf("Negative: %d\n", val); \
    } else { \
        printf("Zero\n"); \
    }

// ❌ 코드 생성 매크로
#define DECLARE_ARRAY(type, name, size) \
    type name[size]; \
    for (int i = 0; i < size; i++) name[i] = 0

// ❌ 안전하지 않은 매크로
#define SWAP(a, b) \
    do { \
        int temp = a; \
        a = b; \
        b = temp; \
    } while(0)

// ❌ 타입을 고려하지 않는 매크로
#define MULTIPLY(a, b) ((a) * (b))

// ❌ 부작용이 있는 매크로
#define NEXT_VALUE(x) (++x)

int main(void) {
    int a = 5, b = 3;
    
    // SQUARE 매크로 문제점
    printf("Square of %d: %d\n", a, SQUARE(a));
    printf("Square of %d: %d\n", a+1, SQUARE(a+1));  // 의도와 다름!
    // SQUARE(a+1)은 (a+1)*(a+1)이 아니라 a+1*a+1 = a+1+a = 2a+1
    
    // MAX 매크로 문제점
    int x = 10, y = 20;
    printf("Max: %d\n", MAX(x++, y++));  // x와 y가 2번 증가!
    printf("x=%d, y=%d\n", x, y);
    
    // ABS 매크로 문제점
    int z = -5;
    printf("ABS of %d: %d\n", z, ABS(z++));  // z가 2번 증가!
    printf("z=%d\n", z);
    
    // PRINT_ARRAY 매크로
    int arr[] = {1, 2, 3, 4, 5};
    PRINT_ARRAY(arr, 5);
    
    // CHECK_AND_PRINT 매크로
    CHECK_AND_PRINT(42);
    CHECK_AND_PRINT(-10);
    CHECK_AND_PRINT(0);
    
    // DECLARE_ARRAY 매크로
    DECLARE_ARRAY(int, numbers, 5);
    printf("Declared array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // SWAP 매크로
    int p = 10, q = 20;
    printf("Before swap: p=%d, q=%d\n", p, q);
    SWAP(p, q);
    printf("After swap: p=%d, q=%d\n", p, q);
    
    // MULTIPLY 매크로 타입 문제
    int int_result = MULTIPLY(3, 4);
    double double_result = MULTIPLY(3.14, 2.0);
    printf("Int multiply: %d\n", int_result);
    printf("Double multiply: %.2f\n", double_result);
    
    // NEXT_VALUE 매크로 부작용
    int counter = 0;
    printf("Next: %d\n", NEXT_VALUE(counter));
    printf("Next: %d\n", NEXT_VALUE(counter));
    printf("Counter: %d\n", counter);
    
    return 0;
}
