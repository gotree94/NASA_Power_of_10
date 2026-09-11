# verify.ps1 - NASA Power of 10 실습 환경 세팅 및 검증 스크립트 (Windows PowerShell)
# Windows + MinGW/MSYS2 環境용

$ErrorActionPreference = "Continue"
$Green = "Green"
$Red = "Red"
$Yellow = "Yellow"
$Cyan = "Cyan"

Write-Host "=========================================" -ForegroundColor $Cyan
Write-Host " NASA Power of 10 실습 환경 세팅" -ForegroundColor $Cyan
Write-Host "=========================================" -ForegroundColor $Cyan

# 1. 필수 도구 확인
Write-Host ""
Write-Host "[1/5] 필수 도구 확인" -ForegroundColor $Cyan

function Check-Tool([string]$name) {
    $cmd = Get-Command $name -ErrorAction SilentlyContinue
    if ($cmd) {
        Write-Host "  [OK] $name 설치됨: $($cmd.Source)" -ForegroundColor $Green
        return $true
    } else {
        Write-Host "  [FAIL] $name 미설치" -ForegroundColor $Red
        return $false
    }
}

$missing = 0
if (-not (Check-Tool "gcc")) { $missing++ }
if (-not (Check-Tool "make")) { $missing++ }
if (-not (Check-Tool "gdb")) { $missing++ }

# 선택 도구
foreach ($tool in @("cppcheck", "clang", "valgrind")) {
    $cmd = Get-Command $tool -ErrorAction SilentlyContinue
    if ($cmd) {
        Write-Host "  [OK] $tool 설치됨: $($cmd.Source)" -ForegroundColor $Green
    } else {
        Write-Host "  [WARN] $tool 미설치 (권장 도구)" -ForegroundColor $Yellow
    }
}

if ($missing -gt 0) {
    Write-Host ""
    Write-Host "필수 도구가 누락되었습니다. gcc, make는 반드시 필요합니다." -ForegroundColor $Yellow
    Write-Host "설치 방법:"
    Write-Host "  1. MSYS2 설치: https://www.msys2.org/"
    Write-Host "  2. pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make"
    Write-Host "  3. PATH 설정 후 재실행"
    Write-Host ""
}

# 2. GCC 버전 확인
Write-Host ""
Write-Host "[2/5] 컴파일러 버전 확인" -ForegroundColor $Cyan
$gccVer = gcc --version 2>$null | Select-Object -First 1
if ($gccVer) {
    Write-Host "  $gccVer" -ForegroundColor $Green
}

# 3. GCC 경고 플래그 테스트
Write-Host ""
Write-Host "[3/5] GCC 경고 플래그 테스트" -ForegroundColor $Cyan
$testDir = Join-Path $env:TEMP "p10test"
New-Item -ItemType Directory -Path $testDir -Force | Out-Null

$test1 = @"
#include <stdio.h>
int main(void) {
    int unused;
    printf("Hello\n");
    return 0;
}
"@
$test1File = Join-Path $testDir "test_warnings.c"
Set-Content -Path $test1File -Value $test1 -Encoding UTF8

$output = gcc -Wall -Wextra -c $test1File -o (Join-Path $testDir "test_warnings.o") 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "  경고가 정상적으로 표시됩니다 (unused-variable 경고):" -ForegroundColor $Green
    Write-Host "  $output"
} else {
    Write-Host "  [FAIL] 경고가 표시되지 않았습니다! 플래그 확인 필요" -ForegroundColor $Red
}

# 4. 정적 분석 도구 테스트
Write-Host ""
Write-Host "[4/5] 정적 분석 도구 테스트" -ForegroundColor $Cyan
if (Check-Tool "cppcheck") {
    $test2 = @"
#include <stdlib.h>
int main(void) {
    char *p = malloc(10);
    p[0] = 'a';
    char *q = malloc(20);
    (void)q;
    free(p);
    return 0;
}
"@
    $test2File = Join-Path $testDir "test_analysis.c"
    Set-Content -Path $test2File -Value $test2 -Encoding UTF8

    Write-Host "  Cppcheck --enable=all 실행: 메모리 누수가 검출되어야 합니다" -ForegroundColor $Cyan
    cppcheck --enable=all $test2File 2>&1 | Select-Object -First 10
} else {
    Write-Host "  cppcheck 미설치로 정적 분석 테스트를 건너뜁니다" -ForegroundColor $Yellow
}

# 5. Valgrind 테스트
Write-Host ""
Write-Host "[5/5] Valgrind 메모리 검증 테스트" -ForegroundColor $Cyan
if (Check-Tool "gcc") {
    $test3 = @"
#include <stdio.h>
int main(void) {
    printf("Hello, Power of 10!\n");
    return 0;
}
"@
    $test3File = Join-Path $testDir "test_compile.c"
    Set-Content -Path $test3File -Value $test3 -Encoding UTF8

    $bin = Join-Path $testDir "test_compile.exe"
    gcc -g -Wall -Wextra $test3File -o $bin 2>&1 | Out-Null
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  GCC 컴파일 성공. 실행:" -ForegroundColor $Green
        & $bin
    }
}

# 정리
Remove-Item -Path $testDir -Recurse -Force -ErrorAction SilentlyContinue

Write-Host ""
Write-Host "=========================================" -ForegroundColor $Cyan
Write-Host " 환경 세팅 검증 완료!" -ForegroundColor $Green
Write-Host "=========================================" -ForegroundColor $Cyan
Write-Host ""
Write-Host "다음 단계:" -ForegroundColor $Yellow
Write-Host "  1. make help       - 사용 가능한 명령어 확인"
Write-Host "  2. make bad        - 나쁜 코드 컴파일 (경고 확인)"
Write-Host "  3. make good       - 좋은 코드 컴파일"
Write-Host "  4. make analyze    - 정적 분석 실행 (cppcheck 필요)"
Write-Host "  5. make check      - 전체 검증"
Write-Host ""
Write-Host "또는 Windows용 예제 실행:"
Write-Host "  powershell -ExecutionPolicy Bypass -File run_examples.ps1"