/*
 * Power of 10 준수 예시: 규칙 8 (안전한 포인터 사용)
 * 
 * 이 코드는 포인터 사용을 최소화하고 안전한 접근 방식을 사용합니다.
 */

#include <stdio.h>
#include <string.h>

#define MAX_ARRAY_SIZE 1000
#define MAX_STRING_SIZE 256
#define MAX_NODES 100

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
    if (dest == NULL || src == NULL) {
        return -1;
    }
    if (dest_size <= 0) {
        return -1;
    }
    
    int result = snprintf(dest, dest_size, "%s", src);
    if (result < 0) {
        return -1;
    }
    
    return 0;
}

// ✅ 배열 기반 연결 리스트 시뮬레이션
struct NodeList {
    int data[MAX_NODES];
    int next[MAX_NODES];
    int head;
    int free_list;
    int count;
};

void init_node_list(struct NodeList *list) {
    if (list == NULL) return;
    
    list->head = -1;
    list->count = 0;
    list->free_list = 0;
    
    for (int i = 0; i < MAX_NODES - 1; i++) {
        list->next[i] = i + 1;
    }
    list->next[MAX_NODES - 1] = -1;
}

int allocate_node(struct NodeList *list) {
    if (list == NULL) return -1;
    if (list->free_list == -1) return -1;
    
    int node = list->free_list;
    list->free_list = list->next[node];
    list->count++;
    
    return node;
}

void free_node(struct NodeList *list, int node) {
    if (list == NULL || node < 0 || node >= MAX_NODES) return;
    
    list->next[node] = list->free_list;
    list->free_list = node;
    list->count--;
}

void add_to_list(struct NodeList *list, int value) {
    if (list == NULL) return;
    
    int new_node = allocate_node(list);
    if (new_node == -1) return;
    
    list->data[new_node] = value;
    list->next[new_node] = list->head;
    list->head = new_node;
}

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

typedef int (*OperationFunc)(int, int);

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

void good_use_operation(void) {
    OperationFunc ops[OP_COUNT] = {add, multiply};
    
    for (int i = 0; i < OP_COUNT; i++) {
        int result = ops[i](3, 4);
        printf("Result: %d\n", result);
    }
}

// ✅ 구조체 배열 사용
struct Student {
    char name[50];
    int age;
    float gpa;
};

void process_students(struct Student *students, int count) {
    if (students == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        printf("Name: %s, Age: %d, GPA: %.2f\n",
               students[i].name,
               students[i].age,
               students[i].gpa);
    }
}

// ✅ 안전한 메모리 레이아웃
void safe_memory_layout(void) {
    struct {
        int x;
        int y;
        char buffer[MAX_STRING_SIZE];
    } data;
    
    data.x = 10;
    data.y = 20;
    strncpy(data.buffer, "Hello", MAX_STRING_SIZE - 1);
    data.buffer[MAX_STRING_SIZE - 1] = '\0';
    
    printf("x=%d, y=%d, buffer=%s\n", data.x, data.y, data.buffer);
}

// ✅ 포인터 사용을 최소화하는 알고리즘
int binary_search(const int *arr, int size, int target) {
    if (arr == NULL || size <= 0) {
        return -1;
    }
    
    int left = 0;
    int right = size - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

int main(void) {
    good_array_access();
    
    char dest[MAX_STRING_SIZE];
    if (good_string_copy(dest, sizeof(dest), "Hello, World!") == 0) {
        printf("%s\n", dest);
    }
    
    struct NodeList list;
    init_node_list(&list);
    add_to_list(&list, 10);
    add_to_list(&list, 20);
    add_to_list(&list, 30);
    good_traverse(&list);
    
    good_use_operation();
    
    struct Student students[] = {
        {"Alice", 20, 3.5f},
        {"Bob", 22, 3.8f},
        {"Charlie", 21, 3.2f}
    };
    process_students(students, 3);
    
    safe_memory_layout();
    
    int sorted_arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int index = binary_search(sorted_arr, 10, 7);
    if (index >= 0) {
        printf("Found at index: %d\n", index);
    }
    
    return 0;
}
