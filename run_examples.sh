#!/bin/bash
# run_examples.sh - 모든 예제 코드 빌드 및 실행 스크립트
# Linux/macOS/Windows (Git Bash, MSYS2, WSL)에서 사용 가능

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

CFLAGS="-std=c11 -g -Wall -Wextra"

base_dir=$(dirname "$0")
bad_dir="$base_dir/code_examples/bad"
good_dir="$base_dir/code_examples/good"

echo "========================================="
echo " NASA Power of 10 예제 실행"
echo "========================================="

# 1. 나쁜 코드 빌드 및 실행 (경고 확인 목적)
echo ""
echo -e "${CYAN}=== 나쁜 코드 예제 (경고 및 오류 확인) ===${NC}"
for src in "$bad_dir"/*.c; do
    name=$(basename "$src" .c)
    echo ""
    echo -e "${YELLOW}파일: $name.c${NC}"
    echo "---"
    echo "컴파일 시도 (경고 활성화):"
    if gcc $CFLAGS "$src" -o "/tmp/$name" 2>&1; then
        echo -e "${GREEN}  [PASS] 컴파일 성공${NC}"
        echo "실행:"
        "/tmp/$name" 2>&1 | head -20 || true
    else
        echo -e "${RED}  [FAIL] 컴파일 실패${NC}"
    fi
done

# 2. 좋은 코드 빌드 및 실행 (경고 없음 검증)
echo ""
echo -e "${CYAN}=== 좋은 코드 예제 (권장 방식) ===${NC}"
for src in "$good_dir"/*.c; do
    name=$(basename "$src" .c)
    echo ""
    echo -e "${YELLOW}파일: $name.c${NC}"
    echo "---"
    echo "컴파일 시도 (-Werror로 모든 경고를 에러 처리):"
    if gcc $CFLAGS -Werror "$src" -o "/tmp/$name" 2>&1; then
        echo -e "${GREEN}  [PASS] 컴파일 성공 (경고 없음)${NC}"
        echo "실행:"
        "/tmp/$name" 2>&1 | head -20 || true
    else
        echo -e "${RED}  [FAIL] 컴파일 실패${NC}"
    fi
done

# 3. 정적 분석 (cppcheck 사용 가능 시)
echo ""
echo -e "${CYAN}=== 정적 분석 (cppcheck) ===${NC}"
if command -v cppcheck &> /dev/null; then
    echo "나쁜 코드 분석:"
    cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem "$bad_dir" 2>&1 | head -30 || true
    echo ""
    echo "좋은 코드 분석:"
    cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem "$good_dir" 2>&1 | head -30 || true
else
    echo "cppcheck 미설치 - 정적 분석을 건너뜁니다."
    echo "설치: sudo apt install cppcheck  (또는) pacman -S cppcheck"
fi

echo ""
echo "========================================="
echo -e "${GREEN}모든 예제 검증 완료!${NC}"
echo "========================================="