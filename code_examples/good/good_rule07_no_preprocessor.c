/*
 * Power of 10 준수 예시: 규칙 7 (전처리기 제한)
 * 
 * 이 코드는 함수형 매크로 대신 인라인 함수를 사용합니다.
 * 타입 안전성과 가독성이 향상됩니다.
 */

#include <stdio.h>

#define MAX_ARRAY_SIZE 1024
#define MAX_BUFFER_SIZE 256

// ✅ const 상수 사용
const int MAX_VALUE = 1000;
const double PI = 3.14159265358979;

// ✅ 인라인 함수 (C99 이상)
static inline int square(int x) {
    return x * x;
}

static inline int max_int(int a, int b) {
    return (a > b) ? a : b;
}

static inline int min_int(int a, int b) {
    return (a < b) ? a : b;
}

static inline int abs_int(int x) {
    return (x < 0) ? -x : x;
}

// ✅ 안전한 매크로 대체 함수
static void print_array(const int *arr, int size) {
    if (arr == NULL || size <= 0) {
        return;
    }
    
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// ✅ 함수로 분리
static void check_and_print(int val) {
    if (val > 0) {
        printf("Positive: %d\n", val);
    } else if (val < 0) {
        printf("Negative: %d\n", val);
    } else {
        printf("Zero\n");
    }
}

// ✅ 안전한 초기화 함수
static void init_array(int *arr, int size, int value) {
    if (arr == NULL || size <= 0) {
        return;
    }
    
    for (int i = 0; i < size; i++) {
        arr[i] = value;
    }
}

// ✅ 안전한 swap 함수
static void swap_int(int *a, int *b) {
    if (a == NULL || b == NULL) {
        return;
    }
    
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ✅ 안전한 곱셈 함수
static int multiply_int(int a, int b) {
    return a * b;
}

static double multiply_double(double a, double b) {
    return a * b;
}

// ✅ 값 반환 함수 (부작용 없음)
static int get_next_value(int current) {
    return current + 1;
}

// ✅ 열거형(enum) 사용
typedef enum {
    COLOR_RED = 0,
    COLOR_GREEN = 1,
    COLOR_BLUE = 2,
    COLOR_COUNT = 3
} Color;

static const char* color_names[] = {"Red", "Green", "Blue"};

static void print_color(Color color) {
    if (color >= 0 && color < COLOR_COUNT) {
        printf("Color: %s\n", color_names[color]);
    }
}

int main(void) {
    int a = 5;
    
    // 인라인 함수 사용
    printf("Square of %d: %d\n", a, square(a));
    printf("Square of %d: %d\n", a+1, square(a+1));  // 의도대로 동작
    
    int x = 10, y = 20;
    printf("Max: %d\n", max_int(x++, y++));  // x와 y가 1번만 증가
    printf("x=%d, y=%d\n", x, y);
    
    int z = -5;
    int abs_z = abs_int(z);
    z++;
    printf("ABS of %d: %d\n", z - 1, abs_z);
    printf("z=%d\n", z);
    
    // print_array 함수
    int arr[] = {1, 2, 3, 4, 5};
    print_array(arr, 5);
    
    // check_and_print 함수
    check_and_print(42);
    check_and_print(-10);
    check_and_print(0);
    
    // init_array 함수
    int numbers[MAX_ARRAY_SIZE];
    init_array(numbers, 5, 0);
    printf("Declared array: ");
    print_array(numbers, 5);
    
    // swap_int 함수
    int p = 10, q = 20;
    printf("Before swap: p=%d, q=%d\n", p, q);
    swap_int(&p, &q);
    printf("After swap: p=%d, q=%d\n", p, q);
    
    // multiply 함수
    int int_result = multiply_int(3, 4);
    double double_result = multiply_double(3.14, 2.0);
    printf("Int multiply: %d\n", int_result);
    printf("Double multiply: %.2f\n", double_result);
    
    // get_next_value 함수
    int counter = 0;
    counter = get_next_value(counter);
    printf("Next: %d\n", counter);
    counter = get_next_value(counter);
    printf("Next: %d\n", counter);
    printf("Counter: %d\n", counter);
    
    // 열거형 사용
    for (int i = 0; i < COLOR_COUNT; i++) {
        print_color((Color)i);
    }
    
    return 0;
}
