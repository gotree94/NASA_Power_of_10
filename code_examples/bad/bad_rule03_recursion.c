/*
 * Power of 10 위반 예시: 규칙 3 (재귀 사용)
 * 
 * 이 코드는 재귀를 사용하여 스택 오버플로우 위험이 있습니다.
 * Power of 10에서는 재귀 사용을 금지합니다.
 */

#include <stdio.h>
#include <stdlib.h>

// ❌ 재귀를 사용한 팩토리얼
long long factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);  // 재귀 호출
}

// ❌ 재귀를 사용한 피보나치
int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);  // 이중 재귀
}

// ❌ 재귀를 사용한 거듭제곱
double power(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp < 0) return 1.0 / power(base, -exp);
    return base * power(base, exp - 1);  // 재귀 호출
}

// ❌ 재귀를 사용한 연결 리스트 역순 출력
struct Node {
    int data;
    struct Node *next;
};

void print_reverse(struct Node *node) {
    if (node == NULL) return;
    print_reverse(node->next);  // 재귀 호출
    printf("%d ", node->data);
}

// ❌ 재귀를 사용한 하노이의 탑
void hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        printf("Move disk 1 from %c to %c\n", from, to);
        return;
    }
    hanoi(n - 1, from, aux, to);
    printf("Move disk %d from %c to %c\n", n, from, to);
    hanoi(n - 1, aux, to, from);
}

int main(void) {
    printf("Factorial of 20: %lld\n", factorial(20));
    
    printf("Fibonacci of 10: %d\n", fibonacci(10));
    
    printf("2^10: %.0f\n", power(2, 10));
    
    // 연결 리스트 생성
    struct Node *head = malloc(sizeof(struct Node));
    head->data = 1;
    head->next = malloc(sizeof(struct Node));
    head->next->data = 2;
    head->next->next = malloc(sizeof(struct Node));
    head->next->next->data = 3;
    head->next->next->next = NULL;
    
    printf("Reverse: ");
    print_reverse(head);
    printf("\n");
    
    printf("Hanoi with 3 disks:\n");
    hanoi(3, 'A', 'C', 'B');
    
    // 메모리 해제
    while (head != NULL) {
        struct Node *temp = head;
        head = head->next;
        free(temp);
    }
    
    return 0;
}
