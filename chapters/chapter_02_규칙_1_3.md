# Chapter 02: 규칙 1-3 (제어흐름, 반복문, Goto/재귀)

## 규칙 1: 단순한 제어 흐름 구조만 사용할 것

### 왜 필요한가?

복잡한 제어 흐름은:
- 코드 리뷰를 어렵게 만든다
- 정적 분석 도구의 분석을 방해한다
- 테스트 커버리지를 달성하기 어렵게 만든다
- 예측 불가능한 동작의 가능성을 높인다

### 허용되는 구조

```c
// 단순 if-else
if (condition) {
    // ...
} else {
    // ...
}

// switch-case (각 case에 break 포함)
switch (value) {
    case 1:
        action1();
        break;
    case 2:
        action2();
        break;
    default:
        default_action();
        break;
}

// 단순 루프
for (int i = 0; i < n; i++) {
    // ...
}

while (condition) {
    // ...
}
```

### 금지되는 구조

```c
// goto 사용 금지
goto error_handler;  // ❌

// 재귀 호출 금지
int factorial(int n) {  // ❌
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// 중첩 break/continue가 필요한 복잡한 루프
for (...) {
    for (...) {
        if (...) break;  // ❌ 외부 루프로 영향
    }
}
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule01_goto.c`):
```c
#include <stdio.h>

int process_data(int *data, int size) {
    int result = 0;
    
    for (int i = 0; i < size; i++) {
        if (data[i] < 0) {
            goto error_negative;  // ❌ goto 사용
        }
        result += data[i];
    }
    
    return result;

error_negative:  // ❌ goto 레이블
    printf("Error: negative value at index %d\n", i);
    return -1;
}
```

**좋은 예시** (`good/good_rule01_structured_flow.c`):
```c
#include <stdio.h>

int process_data(int *data, int size) {
    int result = 0;
    int error_found = 0;
    int error_index = -1;
    
    for (int i = 0; i < size; i++) {
        if (data[i] < 0) {
            error_found = 1;
            error_index = i;
            break;  // ✅ 구조화된 탈출
        }
        result += data[i];
    }
    
    if (error_found) {
        printf("Error: negative value at index %d\n", error_index);
        return -1;
    }
    
    return result;
}
```

---

## 규칙 2: 모든 루프에는 고정된 반복 한계가 있어야 한다

### 왜 필요한가?

- **제어 불능 코드(runaway code) 방지**: 무한 루프나 예상치 못한 긴 실행 시간을 방지
- **예측 가능성**: 코드가 최대 몇 번 실행될지 미리 알 수 있음
- **테스트 용이성**: 테스트 케이스의 실행 시간을 예측 가능

### 구현 방법

```c
// ✅ 루프 카운터를 사용한 고정된 한계
#define MAX_ITERATIONS 1000

for (int i = 0; i < MAX_ITERATIONS; i++) {
    // 루프 작업
    if (condition_met) {
        break;
    }
}

// ✅ while 루프에도 카운터 적용
int count = 0;
while (condition && count < MAX_ITERATIONS) {
    // 루프 작업
    count++;
}
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule02_unbounded_loop.c`):
```c
#include <stdio.h>

void find_value(int *array, int size, int target) {
    int i = 0;
    
    // ❌ 명확한 종료 조건이 없는 루프
    while (array[i] != target) {
        i++;
        // size 검사 없음 - 메모리 초과 위험!
    }
    
    printf("Found at index: %d\n", i);
}
```

**좋은 예시** (`good/good_rule02_bounded_loop.c`):
```c
#include <stdio.h>

#define MAX_SEARCH_SIZE 10000

void find_value(int *array, int size, int target) {
    // ✅ size와 MAX_SEARCHSize 중 작은 값으로 한계 설정
    int limit = (size < MAX_SEARCH_SIZE) ? size : MAX_SEARCH_SIZE;
    int found = 0;
    int i = 0;
    
    for (i = 0; i < limit; i++) {
        if (array[i] == target) {
            found = 1;
            break;
        }
    }
    
    if (found) {
        printf("Found at index: %d\n", i);
    } else {
        printf("Value not found within %d iterations\n", limit);
    }
}
```

---

## 규칙 3: 재귀를 사용하지 말 것

### 왜 필요한가?

- **스택 오버플로우 위험**: 깊은 재귀는 시스템 스택을 초과시킬 수 있음
- **메모리 사용량 예측 불가**: 재귀 깊이에 따라 메모리 사용량이 달라짐
- **성능 불일치**: 동일한 입력에 대해 실행 시간이 다를 수 있음
- **테스트 어려움**: 재귀 깊이를 제어하기 어려움

### 대안: 반복문으로 변환

**재귀** (❌ 금지):
```c
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```

**반복문** (✅ 허용):
```c
int fibonacci(int n) {
    if (n <= 1) return n;
    
    int prev = 0;
    int curr = 1;
    
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    
    return curr;
}
```

### 코드 비교

**나쁜 예시** (`bad/bad_rule03_recursion.c`):
```c
#include <stdio.h>

// ❌ 재귀를 사용한 팩토리얼
long long factorial_recursive(int n) {
    if (n <= 1) return 1;
    return n * factorial_recursive(n - 1);
}

// ❌ 재귀를 사용한 트리 순회
struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

void traverse_tree(struct Node *node) {
    if (node == NULL) return;
    
    traverse_tree(node->left);   // ❌ 재귀
    printf("%d ", node->data);
    traverse_tree(node->right);  // ❌ 재귀
}
```

**좋은 예시** (`good/good_rule03_iterative.c`):
```c
#include <stdio.h>

// ✅ 반복문을 사용한 팩토리얼
long long factorial_iterative(int n) {
    long long result = 1;
    
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    
    return result;
}

// ✅ 스택을 사용한 반복문 트리 순회 (중위 순회)
struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

#define MAX_STACK_SIZE 1000

void traverse_tree_iterative(struct Node *root) {
    struct Node *stack[MAX_STACK_SIZE];
    struct Node *current = root;
    int top = -1;
    
    while (current != NULL || top >= 0) {
        while (current != NULL) {
            stack[++top] = current;
            current = current->left;
        }
        
        current = stack[top--];
        printf("%d ", current->data);
        current = current->right;
    }
}
```

---

## 실습 문제

### 실습 1: Goto를 구조화된 코드로 변환

다음 코드를 Power of 10 규칙을 준수하도록 변환하세요:

```c
int process(int *data, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] < 0) goto error;
        if (data[i] > 100) goto skip;
        sum += data[i];
    }
    return sum;
error:
    return -1;
skip:
    continue;
}
```

### 실습 2: 재귀를 반복문으로 변환

```c
int sum_list(struct Node *head) {
    if (head == NULL) return 0;
    return head->data + sum_list(head->next);
}
```

## 다음 챕터로

[Chapter 03: 규칙 4-6 - 메모리, 스코프, 반환값](chapter_03_규칙_4_6.md)
