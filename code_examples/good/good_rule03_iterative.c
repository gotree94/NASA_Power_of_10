/*
 * Power of 10 준수 예시: 규칙 3 (반복문 사용)
 * 
 * 이 코드는 재귀 대신 반복문을 사용하여 스택 사용을 최소화합니다.
 */

#include <stdio.h>

#define MAX_FACT_VALUE 20
#define MAX_FIB_VALUE 40
#define MAX_STACK_SIZE 100

// ✅ 반복문을 사용한 팩토리얼
long long factorial(int n) {
    long long result = 1;
    int i;
    
    if (n < 0 || n > MAX_FACT_VALUE) {
        return -1;
    }
    
    for (i = 2; i <= n; i++) {
        result *= i;
    }
    
    return result;
}

// ✅ 반복문을 사용한 피보나치
int fibonacci(int n) {
    int prev = 0;
    int curr = 1;
    int next;
    int i;
    
    if (n < 0 || n > MAX_FIB_VALUE) {
        return -1;
    }
    
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    for (i = 2; i <= n; i++) {
        next = prev + curr;
        prev = curr;
        curr = next;
    }
    
    return curr;
}

// ✅ 반복문을 사용한 거듭제곱
double power(double base, int exp) {
    double result = 1.0;
    int i;
    int abs_exp;
    
    if (exp < 0) {
        abs_exp = -exp;
    } else {
        abs_exp = exp;
    }
    
    for (i = 0; i < abs_exp; i++) {
        result *= base;
    }
    
    if (exp < 0) {
        result = 1.0 / result;
    }
    
    return result;
}

// ✅ 스택을 사용한 반복문 연결 리스트 역순 출력
struct Node {
    int data;
    struct Node *next;
};

void print_reverse(const struct Node *head) {
    const struct Node *stack[MAX_STACK_SIZE];
    const struct Node *current = head;
    int top = -1;
    int i;
    
    // 스택에 모든 노드 저장
    while (current != NULL && top < MAX_STACK_SIZE - 1) {
        stack[++top] = current;
        current = current->next;
    }
    
    // 스택에서 꺼내며 출력
    for (i = top; i >= 0; i--) {
        printf("%d ", stack[i]->data);
    }
}

// ✅ 반복문을 사용한 하노이의 탑
void hanoi(int n, char from, char to, char aux) {
    struct Move {
        int n;
        char from;
        char to;
        char aux;
        int state;
    };
    
    struct Move stack[MAX_STACK_SIZE];
    int top = -1;
    
    // 첫 번째 호출 push
    stack[++top].n = n;
    stack[top].from = from;
    stack[top].to = to;
    stack[top].aux = aux;
    stack[top].state = 0;
    
    while (top >= 0) {
        struct Move *current = &stack[top];
        
        if (current->n == 1) {
            printf("Move disk 1 from %c to %c\n", current->from, current->to);
            top--;
        } else if (current->state == 0) {
            current->state = 1;
            stack[++top].n = current->n - 1;
            stack[top].from = current->from;
            stack[top].to = current->aux;
            stack[top].aux = current->to;
            stack[top].state = 0;
        } else if (current->state == 1) {
            printf("Move disk %d from %c to %c\n", current->n, current->from, current->to);
            current->state = 2;
            stack[++top].n = current->n - 1;
            stack[top].from = current->aux;
            stack[top].to = current->to;
            stack[top].aux = current->from;
            stack[top].state = 0;
        } else {
            top--;
        }
    }
}

int main(void) {
    printf("Factorial of 20: %lld\n", factorial(20));
    
    printf("Fibonacci of 10: %d\n", fibonacci(10));
    
    printf("2^10: %.0f\n", power(2, 10));
    
    // 연결 리스트 생성 (동적 할당 없이)
    struct Node nodes[3];
    nodes[0].data = 1;
    nodes[0].next = &nodes[1];
    nodes[1].data = 2;
    nodes[1].next = &nodes[2];
    nodes[2].data = 3;
    nodes[2].next = NULL;
    
    printf("Reverse: ");
    print_reverse(&nodes[0]);
    printf("\n");
    
    printf("Hanoi with 3 disks:\n");
    hanoi(3, 'A', 'C', 'B');
    
    return 0;
}
