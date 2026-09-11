/*
 * Power of 10 위반 예시: 규칙 8 (포인터 사용 제한)
 * 
 * 이 코드는 포인터 산술과 unsafe한 포인터 사용을 합니다.
 * 메모리 안전성 문제가 발생할 수 있습니다.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ❌ 포인터 산술
void bad_pointer_arithmetic(void) {
    int arr[] = {1, 2, 3, 4, 5};
    int *ptr = arr;
    
    for (int i = 0; i < 5; i++) {
        printf("%d ", *ptr);  // ❌ 포인터 역참조
        ptr++;  // ❌ 포인터 증가
    }
    printf("\n");
}

// ❌ 문자열 처리에서 포인터 사용
void bad_string_copy(char *dest, const char *src) {
    while (*src != '\0') {  // ❌ 포인터 역참조
        *dest = *src;  // ❌ 포인터 역참조
        dest++;  // ❌ 포인터 증가
        src++;  // ❌ 포인터 증가
    }
    *dest = '\0';
}

// ❌ 연결 리스트 순회
struct Node {
    int data;
    struct Node *next;
};

void bad_traverse(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {  // ❌ 포인터 비교
        printf("%d ", current->data);
        current = current->next;  // ❌ 포인터 할당
    }
}

// ❌ 함수 포-pointer
typedef int (*operation)(int, int);
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

void bad_use_function_pointer(void) {
    operation ops[] = {add, multiply};  // ❌ 함수 포인터 배열
    for (int i = 0; i < 2; i++) {
        int result = ops[i](3, 4);  // ❌ 함수 포인터 호출
        printf("Result: %d\n", result);
    }
}

// ❌ void 포인터 사용
void bad_void_pointer(void *data, int type) {
    if (type == 1) {
        int *int_ptr = (int*)data;
        printf("Int: %d\n", *int_ptr);
    } else if (type == 2) {
        float *float_ptr = (float*)data;
        printf("Float: %f\n", *float_ptr);
    }
}

// ❌ 다중 포인터
void bad_multi_pointer(void) {
    int x = 42;
    int *p = &x;
    int **pp = &p;
    
    printf("Value: %d\n", **pp);
    **pp = 100;
    printf("New value: %d\n", x);
}

// ❌ 포인터 연산
void bad_pointer_comparison(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int *start = arr;
    int *end = arr + 4;
    
    while (start <= end) {  // ❌ 포인터 비교
        printf("%d ", *start);
        start++;  // ❌ 포인터 증가
    }
    printf("\n");
}

// ❌ realloc으로 인한 포인터 문제
void bad_realloc(void) {
    int *arr = malloc(5 * sizeof(int));
    if (arr == NULL) return;
    
    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
    
    int *new_arr = realloc(arr, 10 * sizeof(int));
    if (new_arr != NULL) {
        arr = new_arr;  // 이전 포인터 무효화
    }
    
    // arr이 여전히 유효하다고 가정하면 문제 발생
    free(new_arr);
}

int main(void) {
    bad_pointer_arithmetic();
    
    char src[] = "Hello, World!";
    char dest[20];
    bad_string_copy(dest, src);
    printf("%s\n", dest);
    
    struct Node nodes[3];
    nodes[0].data = 1; nodes[0].next = &nodes[1];
    nodes[1].data = 2; nodes[1].next = &nodes[2];
    nodes[2].data = 3; nodes[2].next = NULL;
    bad_traverse(nodes);
    printf("\n");
    
    bad_use_function_pointer();
    
    int int_val = 42;
    float float_val = 3.14f;
    bad_void_pointer(&int_val, 1);
    bad_void_pointer(&float_val, 2);
    
    bad_multi_pointer();
    bad_pointer_comparison();
    bad_realloc();
    
    return 0;
}
