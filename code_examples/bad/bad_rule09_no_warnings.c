/*
 * Power of 10 위반 예시: 규칙 9 (컴파일 경고 무시)
 * 
 * 이 코드는 컴파일 경고를 무시하고 작성되었습니다.
 * 다양한 잠재적 문제가 포함되어 있습니다.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ❌ 경고를 무시하는 코드
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wformat-security"

// ❌ 사용되지 않는 전역 변수
int unused_global = 42;

// ❌ 사용 후 할당만 된 전역 변수
int only_assigned_global = 0;

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
    if (index < size) {
        printf("Index is less than size\n");
    }
    
    // ❌ 반환값 무시
    char buffer[10];
    sprintf(buffer, "Test");
    
    // ❌ NULL 포인터 위험
    char *null_ptr = NULL;
    if (null_ptr) {
        printf("%s\n", null_ptr);
    }
    
    // ❌ 표현식이 항상 참/거짓
    int x = 42;
    if (x = 10) {
        printf("x is 10\n");
    }
    
    // ❌ 죽은 코드
    if (1) {
        printf("Always executed\n");
    }
    
    // ❌ 암시적 형변환
    double d = 3.14159;
    int i = d;
    printf("Converted: %d\n", i);
    
    // ❌ 오래된 문자열 함수
    char src[] = "Hello, World!";
    char dest[5];
    strcpy(dest, src);
    
    // ❌ 사용되지 않는 리턴값
    int result = system("echo test");
    (void)result;
    
    return 0;
}