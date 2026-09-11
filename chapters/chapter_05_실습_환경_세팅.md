# Chapter 05: 실습 환경 세팅 및 검증 가이드

## 5.1 필요 도구 및 환경

### 필수 도구

| 도구 | 목적 | 설치 방법 |
|------|------|----------|
| GCC | C 컴파일러 | `sudo apt install gcc` (Linux) |
| Clang | 대체 컴파일러 | `sudo apt install clang` |
| Cppcheck | 정적 분석 | `sudo apt install cppcheck` |
| Valgrind | 메모리 검증 | `sudo apt install valgrind` |
| GDB | 디버거 | `sudo apt install gdb` |
| Make | 빌드 자동화 | `sudo apt install make` |

### Windows 환경 (MinGW/MSYS2)

```bash
# MSYS2 설치 후
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-clang
pacman -S mingw-w64-x86_64-cppcheck
pacman -S mingw-w64-x86_64-make
```

### macOS 환경

```bash
# Xcode Command Line Tools
xcode-select --install

# Homebrew로 추가 도구
brew install cppcheck valgrind
```

## 5.2 환경 세팅 스크립트

```bash
#!/bin/bash
# verify.sh - 환경 세팅 및 검증 스크립트

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "========================================="
echo " NASA Power of 10 실습 환경 세팅"
echo "========================================="

# 1. 필수 도구 확인
echo ""
echo -e "${YELLOW}[1/5] 필수 도구 확인...${NC}"

check_tool() {
    if command -v $1 &> /dev/null; then
        echo -e "  ${GREEN}✓${NC} $1 설치됨: $(command -v $1)"
        return 0
    else
        echo -e "  ${RED}✗${NC} $1 미설치"
        return 1
    fi
}

MISSING=0
for tool in gcc cppcheck valgrind make gdb; do
    check_tool $tool || MISSING=$((MISSING + 1))
done

if [ $MISSING -gt 0 ]; then
    echo ""
    echo -e "${YELLOW}누락된 도구가 있습니다. 설치가 필요합니다.${NC}"
    echo "Linux (Debian/Ubuntu): sudo apt install gcc cppcheck valgrind make gdb"
    echo "macOS: brew install cppcheck valgrind"
    exit 1
fi

# 2. GCC 버전 확인
echo ""
echo -e "${YELLOW}[2/5] GCC 버전 확인...${NC}"
gcc --version | head -1

# 3. 경고 플래그 테스트
echo ""
echo -e "${YELLOW}[3/5] 경고 플래그 테스트...${NC}"

cat > /tmp/test_warnings.c << 'EOF'
#include <stdio.h>
int main(void) {
    int unused;
    printf("Hello\n");
    return 0;
}
EOF

echo "  Wall -Wextra 테스트:"
gcc -Wall -Wextra -c /tmp/test_warnings.c -o /tmp/test_warnings.o 2>&1 || true
rm -f /tmp/test_warnings.c /tmp/test_warnings.o

# 4. 정적 분석 도구 테스트
echo ""
echo -e "${YELLOW}[4/5] 정적 분석 도구 테스트...${NC}"

cat > /tmp/test_analysis.c << 'EOF'
#include <stdlib.h>
int main(void) {
    char *p = malloc(10);
    p[0] = 'a';
    free(p);
    p[0] = 'b';  # use after free
    return 0;
}
EOF

echo "  Cppcheck 테스트:"
cppcheck --enable=all /tmp/test_analysis.c 2>&1 | head -5
rm -f /tmp/test_analysis.c

# 5. Valgrind 테스트
echo ""
echo -e "${YELLOW}[5/5] Valgrind 테스트...${NC}"

cat > /tmp/test_valgrind.c << 'EOF'
#include <stdlib.h>
int main(void) {
    int *arr = malloc(5 * sizeof(int));
    arr[0] = 1;
    free(arr);
    return 0;
}
EOF

gcc -g -o /tmp/test_valgrind /tmp/test_valgrind.c
echo "  Valgrind 메모리 검증:"
valgrind --leak-check=full /tmp/test_valgrind 2>&1 | tail -5
rm -f /tmp/test_valgrind.c /tmp/test_valgrind

echo ""
echo "========================================="
echo -e "${GREEN}환경 세팅 완료!${NC}"
echo "========================================="
```

## 5.3 Makefile 설정

```makefile
# NASA Power of 10 교육용 Makefile

CC = gcc
CLANG = clang

# Power of 10 권장 경고 플래그
CFLAGS = -std=c11 \
         -Wall \
         -Wextra \
         -Werror \
         -Wshadow \
         -Wformat=2 \
         -Wconversion \
         -Wsign-conversion \
         -Wpedantic \
         -Wstrict-prototypes \
         -Wmissing-prototypes \
         -Wnull-dereference \
         -Wdouble-promotion \
         -Wwrite-strings \
         -Warray-bounds=2 \
         -Wswitch-enum \
         -Wimplicit-fallthrough

# 디버그 빌드
CFLAGS_DEBUG = $(CFLAGS) -g -O0 -DDEBUG

# 릴리스 빌드
CFLAGS_RELEASE = $(CFLAGS) -O2 -DNDEBUG

# 정적 분석 플래그
CPPCHECK_FLAGS = --enable=all \
                 --suppress=missingIncludeSystem \
                 --std=c11 \
                 --force

# 소스 파일
BAD_SRCS = $(wildcard code_examples/bad/*.c)
GOOD_SRCS = $(wildcard code_examples/good/*.c)
BAD_OBJS = $(BAD_SRCS:.c=.o)
GOOD_OBJS = $(GOOD_SRCS:.c=.o)

# 타겟
.PHONY: all clean bad good analyze valgrind check help

all: help

help:
	@echo "========================================="
	@echo " NASA Power of 10 교육용 빌드 시스템"
	@echo "========================================="
	@echo ""
	@echo "사용법:"
	@echo "  make bad       - 나쁜 코드 컴파일 (경고 표시)"
	@echo "  make good      - 좋은 코드 컴파일 (경고 없음)"
	@echo "  make analyze   - 정적 분석 실행"
	@echo "  make valgrind  - 메모리 검증 실행"
	@echo "  make check     - 전체 검증 실행"
	@echo "  make clean     - 빌드 파일 정리"
	@echo ""

# 나쁜 코드 컴파일 (경고 의도)
bad:
	@echo "========================================="
	@echo " 나쁜 코드 컴파일 (경고 예상)"
	@echo "========================================="
	@for src in $(BAD_SRCS); do \
		echo ""; \
		echo "파일: $$src"; \
		echo "---"; \
		$(CC) $(CFLAGS_DEBUG) -c $$src -o $${src%.c}.o 2>&1 || true; \
		echo ""; \
	done

# 좋은 코드 컴파일 (경고 없음)
good:
	@echo "========================================="
	@echo " 좋은 코드 컴파일 (경고 없음)"
	@echo "========================================="
	@for src in $(GOOD_SRCS); do \
		echo ""; \
		echo "파일: $$src"; \
		echo "---"; \
		$(CC) $(CFLAGS_DEBUG) -c $$src -o $${src%.c}.o 2>&1; \
		if [ $$? -eq 0 ]; then \
			echo "  ✓ 컴파일 성공"; \
		else \
			echo "  ✗ 컴파일 실패"; \
		fi; \
		echo ""; \
	done

# 정적 분석
analyze:
	@echo "========================================="
	@echo " 정적 분석 (Cppcheck)"
	@echo "========================================="
	@echo ""
	@echo "--- 나쁜 코드 분석 ---"
	@for src in $(BAD_SRCS); do \
		echo ""; \
		echo "파일: $$src"; \
		cppcheck $(CPPCHECK_FLAGS) $$src 2>&1; \
	done
	@echo ""
	@echo "--- 좋은 코드 분석 ---"
	@for src in $(GOOD_SRCS); do \
		echo ""; \
		echo "파일: $$src"; \
		cppcheck $(CPPCHECK_FLAGS) $$src 2>&1; \
	done

# Valgrind 검증
valgrind:
	@echo "========================================="
	@echo " Valgrind 메모리 검증"
	@echo "========================================="
	@for src in $(GOOD_SRCS); do \
		echo ""; \
		echo "파일: $$src"; \
		bin=$${src%.c}; \
		$(CC) $(CFLAGS_DEBUG) -o $$bin $$src 2>&1; \
		if [ $$? -eq 0 ]; then \
			valgrind --leak-check=full --error-exitcode=1 $$bin 2>&1 | tail -10; \
		fi; \
	done

# 전체 검증
check: good analyze
	@echo ""
	@echo "========================================="
	@echo " 전체 검증 완료"
	@echo "========================================="

# 빌드 파일 정리
clean:
	rm -f code_examples/bad/*.o
	rm -f code_examples/good/*.o
	rm -f code_examples/good/*_rule*
	@echo "빌드 파일이 정리되었습니다."
```

## 5.4 각 규칙별 검증 방법

### 규칙 1-3 검증: 제어 흐름

```bash
# goto 사용 검출
grep -rn "goto " code_examples/

# 재귀 함수 검출
grep -rn "void.*(" code_examples/ | grep -v "//"

# 복잡한 제어 흐름 검출
cppcheck --enable=style code_examples/
```

### 규칙 4 검증: 동적 메모리 할당

```bash
# malloc/calloc/realloc/free 사용 검출
grep -rn "malloc\|calloc\|realloc\|free" code_examples/

# Valgrind으로 메모리 누수 검증
valgrind --leak-check=full --track-origins=yes ./program
```

### 규칙 5 검증: 변수 범위

```bash
# 전역 변수 검출
grep -rn "^[a-zA-Z].*[a-zA-Z0-9_]*;" code_examples/ | grep -v "static\|extern\|define"

# 변수 선언 위치 검출
for src in code_examples/good/*.c; do
    echo "=== $src ==="
    grep -n "int \|char \|float \|double " $src
done
```

### 규칙 6 검증: 반환값 검증

```bash
# 함수 호출 후 반환값 검사 여부
grep -B1 -A1 "fopen\|malloc\|fgets\|fread" code_examples/good/*.c
```

### 규칙 7 검증: 전처리기 제한

```bash
# 매크로 사용 검출
grep -rn "#define" code_examples/ | grep -v "include\|ifndef\|endif"

# 함수형 매크로 검출
grep -rn "#define.*(" code_examples/
```

### 규칙 8 검증: 포인터 사용 제한

```bash
# 포인터 산술 검출
grep -rn "ptr++\|ptr--\|ptr +\|ptr -\|\*ptr" code_examples/

# VLA 사용 검출
grep -rn "\[.*\]" code_examples/ | grep -v "define\|//\|\\*"
```

### 규칙 9 검증: 컴파일 경고

```bash
# 모든 경고 활성화 컴파일
for src in code_examples/good/*.c; do
    echo "=== $src ==="
    gcc -Wall -Wextra -Werror -c $src 2>&1
done
```

## 5.5 실습 과제

### 과제 1: 환경 세팅 완료

1. 모든 필수 도구 설치
2. `verify.sh` 실행하여 환경 확인
3. `make help` 명령어로 Makefile 동작 확인

### 과제 2: 나쁜 코드 컴파일

```bash
make bad
```

경고 메시지를 분석하고 각 경고가 어떤 Power of 10 규칙을 위반하는지 기록하세요.

### 과제 3: 좋은 코드 컴파일 및 검증

```bash
make good
make analyze
```

좋은 코드가 왜 경고 없이 컴파일되는지 분석하세요.

### 과제 4: 정적 분석 보고서 작성

```bash
cppcheck --enable=all --xml code_examples/good/*.c 2> report.xml
```

생성된 XML 보고서를 분석하고 요약하세요.

## 다음 챕터로

[Chapter 06: 논평 및 균형잡힌 분석](chapter_06_논평_및_분석.md)
