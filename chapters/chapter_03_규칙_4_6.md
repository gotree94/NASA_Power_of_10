# Chapter 03: 규칙 4-6 (메모리, 스코프, 반환값 검증)

## 규칙 4: 초기화 이후 동적 메모리 할당 금지

### 왜 필요한가?

동적 메모리 할당(malloc, calloc, realloc)은:
- **메모리 누수(memory leak)** 위험: 할당된 메모리를 해제하지 못할 수 있음
- **파편화(fragmentation)**: 장시간 실행 시 메모리가 조각남
- **예측 불가능한 동작**: 할당 실패 시 NULL 반환으로 인한 크래시
- **정적 분석 어려움**: 메모리 라이프타임 추적 어려움

### 대안: 정적 메모리 할당

```c
// ❌ 동적 할당 (금지)
void process(int n) {
    int *buffer = malloc(n * sizeof(int));  // ❌
    if (buffer == NULL) return;
    
    // ... 사용 ...
    
    free(buffer);  // 잊어버리면 메모리 누수!
}

// ✅ 정적 할당 (허용)
#define MAX_BUFFER_SIZE 1024

void process(int n) {
    int buffer[MAX_BUFFER_SIZE];  // ✅ 스택 할당
    
    if (n > MAX_BUFFER_SIZE) return;
    
    // ... 사용 ...
    // free() 필요 없음 - 자동 관리
}
```

### 메모리 할당 전략 비교

| 전략 | 장점 | 단점 | Power of 10 |
|------|------|------|-------------|
| 스택 할당 | 빠름, 자동 관리 | 크기 제한 | ✅ 허용 |
| 정적 전역 | 큰 크기 가능 | 전역 상태 | ✅ 허용 |
| 동적 할당 | 유연한 크기 | 누수 위험 | ❌ 금지 |

### 코드 비교

**나쁜 예시** (`bad/bad_rule04_dynamic_alloc.c`):
```c
#include <stdio.h>
#include <stdlib.h>

// ❌ 동적 메모리 할당 사용
int* create_array(int size) {
    int *arr = malloc(size * sizeof(int));  // ❌ 동적 할당
    if (arr == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        arr[i] = i * 2;
    }
    
    return arr;
}

// ❌ 동적 할당을 사용한 문자열 처리
char* process_string(const char *input) {
    int len = strlen(input);
    char *buffer = malloc(len + 1);  // ❌ 동적 할당
    if (buffer == NULL) return NULL;
    
    strcpy(buffer, input);
    // ... 처리 ...
    return buffer;  // 호출자가 free()해야 함!
}
```

**좋은 예시** (`good/good_rule04_static_alloc.c`):
```c
#include <stdio.h>
#include <string.h>

#define MAX_ARRAY_SIZE 1024
#define MAX_STRING_SIZE 256

// ✅ 정적 배열 사용
int create_array(int *arr, int size) {
    if (size > MAX_ARRAY_SIZE) return -1;
    
    for (int i = 0; i < size; i++) {
        arr[i] = i * 2;
    }
    
    return 0;  // 성공
}

// ✅ 정적 버퍼 사용
int process_string(char *output, int output_size, const char *input) {
    int len = strlen(input);
    
    if (len >= output_size) return -1;
    
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    return 0;  // 성공
}
```

---

## 규칙 5: 데이터 객체는 가장 좁은 범위(scope)에서 선언할 것

### 왜 필요한가?

- **가시성 제한**: 변수의 영향 범위를 최소화
- **변경 추적 용이성**: 변수가 어디서 변경되는지 파악 쉬움
- **이름 충돌 방지**: 같은 이름의 변수가 다른 곳에서 사용되지 않도록
- **정적 분석 효과적**: 변수의 라이프타임이 명확함

### 범위 비교

```c
// ❌ 넓은 범위
int i;  // 전역
int result;

void function1(void) {
    result = 0;
    for (i = 0; i < 100; i++) {
        result += i;
    }
}

void function2(void) {
    // i와 result가 여전히 유효 - 의도치 않은 사용 가능
    result += i;  // 실수로 이전 상태 사용
}

// ✅ 좁은 범위
void function1(void) {
    int result = 0;
    for (int i = 0; i < 100; i++) {
        result += i;
    }
    // i와 result는 여기서 소멸
}

void function2(void) {
    // i와 result를 사용할 수 없음 - 안전!
    int local_var = 42;  // 이 함수에서만 유효
}
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule05_wide_scope.c`):
```c
#include <stdio.h>

// ❌ 전역 변수
int global_counter = 0;
int global_result = 0;
char global_buffer[256];

void process_data(int *data, int size) {
    int i, j;  // ✅ 함수 내이지만 for 루프 밖에서 선언
    
    // ❌ i가 for 루프 밖에서 선언됨
    for (i = 0; i < size; i++) {
        global_result += data[i];
    }
    
    // ❌ 전역 변수 사용
    global_counter++;
    
    // ❌ 전역 버퍼 사용
    sprintf(global_buffer, "Processed %d items", size);
}

void another_function(void) {
    // ❌ 이전 함수의 i를 사용할 수 있음 (의도치 않게)
    // global_result도 여전히 유효
    printf("Counter: %d\n", global_counter);
}
```

**좋은 예시** (`good/good_rule05_narrow_scope.c`):
```c
#include <stdio.h>

#define MAX_BUFFER_SIZE 256

// ✅ 전역 변수 최소화
static int process_counter = 0;

void process_data(int *data, int size) {
    int local_result = 0;  // ✅ 함수 내에서 선언
    char local_buffer[MAX_BUFFER_SIZE];  // ✅ 함수 내에서 선언
    
    // ✅ for 루프 내에서 i 선언
    for (int i = 0; i < size; i++) {
        local_result += data[i];
    }
    
    process_counter++;
    
    // ✅ 로컬 버퍼 사용
    snprintf(local_buffer, MAX_BUFFER_SIZE, "Processed %d items", size);
    printf("%s\n", local_buffer);
    
    // local_result와 local_buffer는 함수 종료 시 소멸
}

void another_function(void) {
    // ✅ 이전 함수의 변수에 접근 불가
    int local_data = 42;  // 이 함수에서만 유효
    
    printf("Counter: %d\n", process_counter);  // 전역은 허용
}
```

---

## 규칙 6: 반환값 확인 및 매개변수 검증

### 왜 필요한가?

- **오류 처리**: 함수 호출 실패를 무시하면 예상치 못한 동작 발생
- **보안**: 잘못된 입력으로 인한 버퍼 오버플로우 방지
- **안정성**: NULL 포인터 역참조 방지
- **디버깅**: 오류 원인을 빠르게 파악

### 검증 체크리스트

```c
// 1. 함수 반환값 확인
FILE *fp = fopen("file.txt", "r");
if (fp == NULL) {
    // 오류 처리
    return -1;
}

// 2. 매개변수 검증
void process(int *data, int size) {
    if (data == NULL) return;
    if (size <= 0) return;
    if (size > MAX_SIZE) return;
    
    // 안전한 처리
}

// 3. 메모리 할당 결과 확인
int *buffer = malloc(size * sizeof(int));
if (buffer == NULL) {
    return -1;
}

// 4. 문자열 함수 결과 확인
int result = snprintf(buffer, size, "format", args);
if (result < 0 || result >= size) {
    // 오버플로우 발생
}
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule06_no_return_check.c`):
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ❌ 반환값 무시
void bad_file_processing(void) {
    FILE *fp = fopen("data.txt", "r");  // 반환값 확인 안 함
    char buffer[256];
    
    fgets(buffer, sizeof(buffer), fp);  // fp가 NULL일 수 있음!
    fclose(fp);  // fp가 NULL이면 크래시!
    
    printf("Data: %s\n", buffer);
}

// ❌ 매개변수 검증 안 함
int bad_array_sum(int *data, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];  // data가 NULL이거나 size가 잘못되면?
    }
    return sum;
}

// ❌ 동적 할당 결과 무시
void bad_dynamic_example(void) {
    char *str = malloc(100);  // NULL일 수 있음
    strcpy(str, "Hello");     // 크래시!
    free(str);
}
```

**좋은 예시** (`good/good_rule06_return_check.c`):
```c
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

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
int good_array_sum(int *data, int size) {
    if (data == NULL) return -1;
    if (size <= 0) return -1;
    if (size > MAX_ARRAY_SIZE) return -1;
    
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum;
}

// ✅ 모든 함수 호출 검증
int good_string_copy(char *dest, int dest_size, const char *src) {
    if (dest == NULL || src == NULL) return -1;
    if (dest_size <= 0) return -1;
    
    int result = snprintf(dest, dest_size, "%s", src);
    if (result < 0) return -1;
    
    return 0;
}
```

---

## 실습 문제

### 실습 1: 동적 할당을 정적 할당으로 변환

```c
// 이 코드를 Power of 10 규칙을 준수하도록 변환하세요
#include <stdlib.h>

int* process_data(int n) {
    int *result = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        result[i] = i * i;
    }
    return result;
}
```

### 실습 2: 반환값 검증 추가

```c
// 이 코드에 반환값 검증을 추가하세요
void read_and_process(void) {
    FILE *fp = fopen("data.txt", "r");
    char buffer[100];
    fgets(buffer, 100, fp);
    fclose(fp);
    printf("%s", buffer);
}
```

## 다음 챕터로

[Chapter 04: 규칙 7-9 - 전처리기, 포인터, 경고](chapter_04_규칙_7_9.md)
