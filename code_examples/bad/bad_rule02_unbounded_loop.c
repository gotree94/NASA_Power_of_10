/*
 * Power of 10 위반 예시: 규칙 2 (반복문 경계 없음)
 * 
 * 이 코드는 명확한 종료 조건이 없는 루프를 사용합니다.
 * 제어 불능 코드(runway code)의 위험이 있습니다.
 */

#include <stdio.h>
#include <string.h>

void find_character(const char *text, char target) {
    int i = 0;
    
    // ❌ 명확한 종료 조건 없음 - 문자열 끝까지 반복
    while (text[i] != target) {
        i++;
        // text[i]가 널 문자인지 검사하지 않음!
        // 무한 루프 위험
    }
    
    printf("Found '%c' at index %d\n", target, i);
}

int sum_until_negative(int *array) {
    int sum = 0;
    int i = 0;
    
    // ❌ 배열 크기를 모르므로 무한 루프 위험
    while (array[i] >= 0) {
        sum += array[i];
        i++;
    }
    
    return sum;
}

void process_stream(void) {
    int count = 0;
    
    // ❌ 종료 조건이 불확실한 루프
    while (1) {
        count++;
        if (count > 1000000) {
            break;  // 매우 큰 값에서만 종료
        }
    }
    
    printf("Processed %d iterations\n", count);
}

int main(void) {
    char text[] = "Hello, World!";
    find_character(text, 'W');
    
    int numbers[] = {1, 2, 3, 4, 5, -1};
    printf("Sum: %d\n", sum_until_negative(numbers));
    
    process_stream();
    
    return 0;
}
