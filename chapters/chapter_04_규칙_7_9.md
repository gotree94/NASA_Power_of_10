# Chapter 04: 규칙 7-9 (전처리기, 포인터, 컴파일 경고)

## 규칙 7: C언어 전처리기(preprocessor) 사용 제한

### 왜 필요한가?

전처리기 매크로는:
- **정적 분석 방해**: 매크로 치환 후 코드가 달라져서 분석 어려움
- **디버깅 어려움**: 매크로 확장 후의 코드를 추적하기 어려움
- **타입 안전성 부족**: 매크로는 타입 검사가 없음
- **예상치 못한 부작용**: 매개변수가 여러 번 평가될 수 있음

### 허용되는 전처리기 사용

```c
// ✅ 허용: 단순 상수 정의
#define MAX_SIZE 1024
#define PI 3.14159

// ✅ 허용: 파일 포함
#include <stdio.h>
#include "myheader.h"

// ✅ 허용: 조건부 컴파일 (디버깅용)
#ifdef DEBUG
    printf("Debug: %s\n", message);
#endif
```

### 금지되는 전처리기 사용

```c
// ❌ 함수형 매크로
#define SQUARE(x) ((x) * (x))  // ❌ 타입 안전성 부족
#define MAX(a, b) ((a) > (b) ? (a) : (b))  // ❌ 매개변수 다중 평가

// ❌ 코드 생성 매크로
#define DECLARE_ARRAY(type, name, size) \
    type name[size]; \
    for (int i = 0; i < size; i++) name[i] = 0

// ❌ 복잡한 매크로
#define COMPLEX_MACRO(a, b, c) \
    do { \
        if ((a) > 0) { \
            (b) = (c) + (a); \
        } \
    } while(0)
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule07_preprocessor.c`):
```c
#include <stdio.h>

// ❌ 함수형 매크로
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// ❌ 코드 생성 매크로
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

int main(void) {
    int a = 5, b = 3;
    
    // SQUARE 매크로 문제점
    printf("Square of %d: %d\n", a, SQUARE(a));
    printf("Square of %d: %d\n", a+1, SQUARE(a+1));  // 의도와 다름!
    
    // MAX 매크로 문제점
    int x = 10, y = 20;
    printf("Max: %d\n", MAX(x++, y++));  // x와 y가 2번 증가!
    
    int arr[] = {1, 2, 3, 4, 5};
    PRINT_ARRAY(arr, 5);
    
    CHECK_AND_PRINT(42);
    
    return 0;
}
```

**좋은 예시** (`good/good_rule07_no_preprocessor.c`):
```c
#include <stdio.h>

// ✅ const 상수 사용
const int MAX_SIZE = 1024;
const double PI = 3.14159;

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

// ✅ 함수로 분리
static void print_array(const int *arr, int size) {
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

int main(void) {
    int a = 5, b = 3;
    
    // 인라인 함수 사용
    printf("Square of %d: %d\n", a, square(a));
    printf("Square of %d: %d\n", a+1, square(a+1));  // 의도대로 동작
    
    int x = 10, y = 20;
    printf("Max: %d\n", max_int(x++, y++));  // x와 y가 1번만 증가
    
    int arr[] = {1, 2, 3, 4, 5};
    print_array(arr, 5);
    
    check_and_print(42);
    
    return 0;
}
```

---

## 규칙 8: 포인터 사용 제한

### 왜 필요한가?

포인터는:
- **메모리 안전성 문제**: 잘못된 포인터 사용으로 세그멘테이션 폴트 발생
- **정적 분석 어려움**: 포인터가 가리키는 대상을 추적하기 어려움
- **알리어싱(aliasing)**: 같은 메모리를 가리키는 여러 포인터로 인한 문제
- **버퍼 오버플로우**: 포인터 연산으로 인한 메모리 접근 위반

### 포인터 사용 제한 지침

```c
// ❌ 포인터 산술
int arr[10];
int *ptr = arr;
ptr++;  // ❌ 포인터 증가
ptr += 5;  // ❌ 포인터 더하기

// ❌ 복합 리터럴로 동적 크기 배열
void func(int n) {
    int arr[n];  // ❌ VLA (Variable Length Array)
}

// ✅ 안전한 포인터 사용
void func(int *arr, int size) {
    if (arr == NULL) return;
    if (size <= 0) return;
    
    for (int i = 0; i < size; i++) {
        printf("%d\n", arr[i]);  // ✅ 배열 인덱싱
    }
}

// ✅ 포인터 대신 배열 인덱싱
int sum_array(const int *arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];  // ✅ arr[i]는 *(arr + i)와 동일
    }
    return sum;
}
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule08_pointer_arith.c`):
```c
#include <stdio.h>
#include <string.h>

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

// ❌ 함수 포인터
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
```

**좋은 예시** (`good/good_rule08_safe_pointer.c`):
```c
#include <stdio.h>
#include <string.h>

#define MAX_ARRAY_SIZE 1000
#define MAX_STRING_SIZE 256

// ✅ 배열 인덱싱 사용
void good_array_access(void) {
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;
    
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);  // ✅ 배열 인덱싱
    }
    printf("\n");
}

// ✅ 안전한 문자열 처리
int good_string_copy(char *dest, int dest_size, const char *src) {
    if (dest == NULL || src == NULL) return -1;
    if (dest_size <= 0) return -1;
    
    int result = snprintf(dest, dest_size, "%s", src);
    if (result < 0) return -1;
    
    return 0;
}

// ✅ 배열 기반 연결 리스트 시뮬레이션
#define MAX_NODES 100

struct NodeList {
    int data[MAX_NODES];
    int next[MAX_NODES];
    int head;
    int free_list;
};

void good_traverse(const struct NodeList *list) {
    if (list == NULL) return;
    
    int current = list->head;
    int count = 0;
    
    while (current != -1 && count < MAX_NODES) {
        printf("%d ", list->data[current]);
        current = list->next[current];
        count++;
    }
    printf("\n");
}

// ✅ 열거형(enum)으로 대체
typedef enum {
    OP_ADD = 0,
    OP_MULTIPLY = 1,
    OP_COUNT = 2
} OperationType;

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

typedef int (*OperationFunc)(int, int);

void good_use_operation(void) {
    OperationFunc ops[OP_COUNT] = {add, multiply};
    
    for (int i = 0; i < OP_COUNT; i++) {
        int result = ops[i](3, 4);
        printf("Result: %d\n", result);
    }
}
```

---

## 규칙 9: 모든 가능한 컴파일 경고를 활성화하고 해결할 것

### 왜 필요한가?

컴파일 경고는:
- **잠재적 버그 감지**: 실제 오류로 이어질 수 있는 문제점 표시
- **코드 품질 향상**: 컴파일러가 발견한 문제점 해결
- **이식성 향상**: 플랫폼 간 호환성 문제 예방
- **정적 분석 보완**: 컴파일러의 추가 검사 활용

### GCC/Clang 경고 플래그

```bash
# 기본 경고
-Wall -Wextra

# 추가 권장 경고
-Werror          # 경고를 에러로 처리
-Wshadow         # 변수 섀도잉 경고
-Wformat=2       # 포맷 문자열 검증
-Wconversion     # 타입 변환 경고
-Wsign-conversion # 부호 변환 경고
-Wpedantic       # 표준 준수 경고
-Wstrict-prototypes # 함수 프로토타입 검증
-Wmissing-prototypes # 누락된 프로토타입 경고
-Wnull-dereference # NULL 역참조 경고
-Wdouble-promotion # double→float 암시적 변환
-Wwrite-strings  # 문자열 상수 할당 경고
```

### Makefile 예시

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wshadow -Wformat=2 \
         -Wconversion -Wsign-conversion -Wpedantic \
         -Wstrict-prototypes -Wmissing-prototypes \
         -Wnull-dereference -std=c11

all: program

program: main.o utils.o
	$(CC) $(CFLAGS) -o program main.o utils.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o program
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule09_no_warnings.c`):
```c
#include <stdio.h>
#include <string.h>

// ❌ 경고를 무시하는 코드
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wformat-security"

int main(void) {
    // ❌ 사용되지 않는 변수
    int unused_var = 42;
    
    // ❌ 사용 후 할당만 된 변수
    int only_assigned = 10;
    only_assigned = 20;
    
    // ❌ 포맷 문자열 경고
    char *format = "Hello, %s!\n";
    printf(format, "World");
    
    // ❌ 부호 없는 정수와의 비교
    unsigned int size = 10;
    int index = -1;
    if (index < size) {  // ❌ 경고: signed/unsigned comparison
        printf("Index is less than size\n");
    }
    
    // ❌ 반환값 무시
    char buffer[10];
    sprintf(buffer, "Test");  // ❌ 버퍼 오버플로우 위험
    
    // ❌ 빈 함수
    void empty_func(void) {
    }
    
    return 0;
}
```

**좋은 예시** (`good/good_rule09_clean_compile.c`):
```c
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 100

// ✅ 모든 경고를 활성화하고 해결
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
#pragma GCC diagnostic error "-Werror"
#pragma GCC diagnostic error "-Wshadow"
#pragma GCC diagnostic error "-Wformat=2"
#pragma GCC diagnostic error "-Wconversion"
#pragma GCC diagnostic error "-Wsign-conversion"
#pragma GCC diagnostic error "-Wpedantic"

int main(void) {
    // ✅ 사용되는 변수만 선언
    int used_var = 42;
    printf("Value: %d\n", used_var);
    
    // ✅ 포맷 문자열 직접 사용
    printf("Hello, %s!\n", "World");
    
    // ✅ 부호 일치
    unsigned int size = 10;
    unsigned int index = 0;
    if (index < size) {
        printf("Index is less than size\n");
    }
    
    // ✅ 안전한 문자열 처리
    char buffer[MAX_BUFFER_SIZE];
    int result = snprintf(buffer, sizeof(buffer), "Test");
    if (result < 0) {
        fprintf(stderr, "Error formatting string\n");
        return 1;
    }
    
    // ✅ 함수 구현 포함
    // void empty_func(void) {
    //     // 함수 본문이 필요하면 작성, 아니면 선언만
    // }
    
    return 0;
}

#pragma GCC diagnostic pop
```

---

## 실습 문제

### 실습 1: 매크로를 인라인 함수로 변환

```c
// 이 매크로를 인라인 함수로 변환하세요
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
```

### 실습 2: 경고 활성화 Makefile 작성

```bash
# 다음 요구사항을 만족하는 Makefile을 작성하세요
# 1. 모든 경고 플래그 활성화
# 2. C11 표준 사용
# 3. 경고를 에러로 처리
# 4. clean 타겟 포함
```

## 다음 챕터로

[Chapter 05: 실습 환경 세팅](chapter_05_실습_환경_세팅.md)
