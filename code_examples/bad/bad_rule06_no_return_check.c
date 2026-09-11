/*
 * Power of 10 위반 예시: 규칙 6 (반환값 검증 없음)
 * 
 * 이 코드는 함수의 반환값을 확인하지 않습니다.
 * 오류 처리가 부족하여 예상치 못한 동작이 발생할 수 있습니다.
 */

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

// ❌ 문자열 함수 결과 무시
void bad_string_operations(void) {
    char dest[10];
    char src[] = "Hello, World!";  // 13자 + 널 문자
    
    strcpy(dest, src);  // 버퍼 오버플로우!
    printf("%s\n", dest);
}

// ❌ scanf 반환값 무시
void bad_input_processing(void) {
    int value;
    scanf("%d", &value);  // 입력 실패 시 value는 초기화되지 않음
    printf("Value: %d\n", value);
}

// ❌ atoi 사용 (오류 검증 없음)
void bad_conversion(void) {
    char *str = "not_a_number";
    int value = atoi(str);  // 0 반환하지만 오류인지 검증 안 함
    printf("Value: %d\n", value);
}

// ❌ 모든 함수 호출 검증 안 함
void bad_comprehensive_example(void) {
    FILE *fp = fopen("test.txt", "w");
    char buffer[50];
    int arr[5];
    
    fprintf(fp, "Hello");  // fp가 NULL일 수 있음
    
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // 반환값은 확인했지만 fp는 확인 안 함
    }
    
    for (int i = 0; i < 10; i++) {  // 배열 크기 초과!
        arr[i] = i;
    }
    
    fclose(fp);
}

int main(void) {
    // bad_file_processing();  // 크래시 위험
    // bad_dynamic_example();  // 크래시 위험
    
    int data[] = {1, 2, 3};
    printf("Sum: %d\n", bad_array_sum(NULL, 3));  // 크래시 위험
    
    bad_string_operations();
    bad_input_processing();
    bad_conversion();
    bad_comprehensive_example();
    
    return 0;
}
