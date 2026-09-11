#!/bin/bash
# verify.sh - NASA Power of 10 실습 환경 세팅 및 검증 스크립트
# Linux/macOS/Windows (Git Bash, MSYS2, WSL)에서 사용 가능

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo "========================================="
echo " NASA Power of 10 실습 환경 세팅"
echo "========================================="

check_tool() {
    if command -v "$1" &> /dev/null; then
        echo -e "  ${GREEN}[OK]${NC} $1 설치됨: $(command -v "$1")"
        return 0
    else
        echo -e "  ${RED}[FAIL]${NC} $1 미설치"
        return 1
    fi
}

# 1. 필수 도구 확인
echo ""
echo -e "${CYAN}[1/5] 필수 도구 확인${NC}"

MISSING=0
for tool in gcc make gdb; do
    check_tool "$tool" || MISSING=$((MISSING + 1))
done

# 선택 도구 (설치 권장)
for tool in cppcheck clang valgrind; do
    if command -v "$tool" &> /dev/null; then
        echo -e "  ${GREEN}[OK]${NC} $tool 설치됨: $(command -v "$tool")"
    else
        echo -e "  ${YELLOW}[WARN]${NC} $tool 미설치 (권장 도구)"
    fi
done

if [ $MISSING -gt 0 ]; then
    echo ""
    echo -e "${YELLOW}필수 도구가 누락되었습니다. gcc, make는 반드시 필요합니다.${NC}"
    echo "Linux (Debian/Ubuntu): sudo apt install gcc make gdb cppcheck valgrind clang"
    echo "macOS: xcode-select --install && brew install cppcheck valgrind"
    echo "Windows: MSYS2 또는 WSL 설치 필요"
    echo ""
fi

# 2. GCC 버전 확인
echo ""
echo -e "${CYAN}[2/5] 컴파일러 버전 확인${NC}"
if command -v gcc &> /dev/null; then
    gcc --version | head -1
    echo "  표준: C11 (C99/C11/C17 확인)"
fi

# 3. GCC 경고 플래그 테스트
echo ""
echo -e "${CYAN}[3/5] GCC 경고 플래그 테스트${NC}"
if command -v gcc &> /dev/null; then
    if [ ! -d /tmp/p10test ]; then
        mkdir -p /tmp/p10test
    fi

    cat > /tmp/p10test/test_warnings.c << 'EOF'
#include <stdio.h>
int main(void) {
    int unused;
    printf("Hello\n");
    return 0;
}
EOF

    echo "  -Wall -Wextra 플래그로 컴파일: 경고가 표시되어야 합니다"
    if gcc -Wall -Wextra -c /tmp/p10test/test_warnings.c -o /tmp/p10test/test_warnings.o 2>&1; then
        echo -e "  ${RED}경고가 표시되지 않았습니다! 플래그 확인 필요${NC}"
    else
        echo -e "  ${GREEN}경고가 정상적으로 표시됩니다${NC}"
    fi
fi

# 4. 정적 분석 도구 테스트
echo ""
echo -e "${CYAN}[4/5] 정적 분석 도구 테스트${NC}"
if command -v cppcheck &> /dev/null; then
    cat > /tmp/p10test/test_analysis.c << 'EOF'
#include <stdlib.h>
int main(void) {
    char *p = malloc(10);
    p[0] = 'a';
    char *q = malloc(20);
    (void)q;
    free(p);
    return 0;
}
EOF

    echo "  Cppcheck --enable=all 실행: 메모리 누수가 검출되어야 합니다"
    cppcheck --enable=all /tmp/p10test/test_analysis.c 2>&1 | head -10
else
    echo -e "  ${YELLOW}cppcheck 미설치로 정적 분석 테스트를 건너뜁니다${NC}"
fi

# 5. Valgrind 테스트
echo ""
echo -e "${CYAN}[5/5] Valgrind 메모리 검증 테스트${NC}"
if command -v valgrind &> /dev/null && command -v gcc &> /dev/null; then
    cat > /tmp/p10test/test_valgrind.c << 'EOF'
#include <stdlib.h>
int main(void) {
    int *arr = malloc(5 * sizeof(int));
    arr[0] = 1;
    free(arr);
    return 0;
}
EOF

    if gcc -g -o /tmp/p10test/test_valgrind /tmp/p10test/test_valgrind.c 2>/dev/null; then
        echo "  Valgrind --leak-check=full 실행:"
        valgrind --leak-check=full /tmp/p10test/test_valgrind 2>&1 | tail -8
    fi
fi

# 정리
rm -rf /tmp/p10test

echo ""
echo "========================================="
echo -e "${GREEN}환경 세팅 검증 완료!${NC}"
echo "========================================="
echo ""
echo "다음 단계:"
echo "  1. make help      - 사용 가능한 명령어 확인"
echo "  2. make bad       - 나쁜 코드 컴파일 (경고 확인)"
echo "  3. make good      - 좋은 코드 컴파일 (경고 없음)"
echo "  4. make analyze   - 정적 분석 실행"
echo "  5. make check     - 전체 검증"
echo ""
echo "또는 모든 예제 실행:"
echo "  bash run_examples.sh"
echo ""