# NASA Power of 10 교육용 Makefile

CC = gcc
CLANG = clang

# Power of 10 권장 경고 플래그
CFLAGS = -std=c11 \
         -Wall \
         -Wextra \
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

# GOOD 코드는 Werror로 컴파일 (경고를 에러로 처리)
CFLAGS_GOOD = $(CFLAGS_DEBUG) -Werror

# 정적 분석 플래그
CPPCHECK_FLAGS = --enable=all \
                 --suppress=missingIncludeSystem \
                 --std=c11

# 소스 파일
BAD_SRCS = $(wildcard code_examples/bad/*.c)
GOOD_SRCS = $(wildcard code_examples/good/*.c)

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
		$(CC) $(CFLAGS_GOOD) -c $$src -o $${src%.c}.o 2>&1; \
		if [ $$? -eq 0 ]; then \
			echo "  [PASS] 컴파일 성공"; \
		else \
			echo "  [FAIL] 컴파일 실패"; \
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
		$(CC) $(CFLAGS_DEBUG) -o $$bin.exe $$src 2>&1; \
		if [ $$? -eq 0 ]; then \
			valgrind --leak-check=full --error-exitcode=1 ./$$bin.exe 2>&1 | tail -10; \
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
	rm -f code_examples/bad/*.exe
	rm -f code_examples/good/*.exe
	@echo "빌드 파일이 정리되었습니다."