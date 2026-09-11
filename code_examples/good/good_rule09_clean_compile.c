/*
 * Power of 10 준수 예시: 규칙 9 (컴파일 경고 해결)
 * 
 * 이 코드는 모든 컴파일 경고를 활성화하고 완전히 해결합니다.
 * -Wall -Wextra -Werror로 컴파일 가능합니다.
 */

#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 100

// ✅ 사용되는 전역 변수만
static int used_global = 0;

int main(void) {
    // ✅ 사용되는 변수만 선언
    int used_var = 42;
    printf("Value: %d\n", used_var);
    
    // ✅ 포맷 문자열 직접 사용
    printf("Hello, %s!\n", "World");
    
    // ✅ 부호 일치
    unsigned int size = 10;
    unsigned int index = 1;
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
    
    // ✅ NULL 포인터 검사
    char *text = NULL;
    if (text != NULL) {
        printf("Text: %s\n", text);
    }
    
    // ✅ 명확한 조건식
    int x = 10;
    if (x == 10) {
        printf("x is 10\n");
    }
    
    // ✅ 명시적 형변환
    double d = 3.14159;
    int i = (int)d;
    printf("Converted: %d\n", i);
    
    // ✅ 안전한 문자열 복사
    char src[] = "Hello, World!";
    char dest[MAX_BUFFER_SIZE];
    int copy_result = snprintf(dest, sizeof(dest), "%s", src);
    if (copy_result < 0) {
        fprintf(stderr, "Error copying string\n");
        return 1;
    }
    
    used_global++;
    (void)used_global;
    
    return 0;
}