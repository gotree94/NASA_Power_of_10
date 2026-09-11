# run_examples.ps1 - NASA Power of 10 예제 빌드 및 실행 스크립트 (Windows PowerShell)
# gcc (MinGW/MSYS2) 필요

$ErrorActionPreference = "Continue"

$baseDir = $PSScriptRoot
$badDir = Join-Path $baseDir "code_examples\bad"
$goodDir = Join-Path $baseDir "code_examples\good"
$tempDir = Join-Path $env:TEMP "p10examples"

New-Item -ItemType Directory -Path $tempDir -Force | Out-Null

Write-Host "=========================================" -ForegroundColor "Cyan"
Write-Host " NASA Power of 10 예제 실행" -ForegroundColor "Cyan"
Write-Host "=========================================" -ForegroundColor "Cyan"

# gcc 확인
$gcc = Get-Command gcc -ErrorAction SilentlyContinue
if (-not $gcc) {
    Write-Host "[ERROR] gcc가 설치되어 있지 않습니다. MSYS2 또는 MinGW를 설치하세요." -ForegroundColor "Red"
    exit 1
}

# 1. 나쁜 코드 빌드 및 실행
Write-Host ""
Write-Host "=== 나쁜 코드 예제 (경고 및 오류 확인) ===" -ForegroundColor "Cyan"

$badFiles = Get-ChildItem -Path $badDir -Filter "*.c"
foreach ($file in $badFiles) {
    $name = $file.BaseName
    $outFile = Join-Path $tempDir "$name.exe"
    Write-Host ""
    Write-Host "파일: $($file.Name)" -ForegroundColor "Yellow"
    Write-Host "---"

    $output = & gcc -std=c11 -g -Wall -Wextra $file.FullName -o $outFile 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  [PASS] 컴파일 성공" -ForegroundColor "Green"
        Write-Host "실행:"
        & $outFile 2>&1 | Select-Object -First 10
    } else {
        Write-Host "  [FAIL] 컴파일 실패" -ForegroundColor "Red"
        Write-Host $output | Select-Object -First 15
    }
}

# 2. 좋은 코드 빌드 및 실행
Write-Host ""
Write-Host "=== 좋은 코드 예제 (권장 방식) ===" -ForegroundColor "Cyan"

$goodFiles = Get-ChildItem -Path $goodDir -Filter "*.c"
foreach ($file in $goodFiles) {
    $name = $file.BaseName
    $outFile = Join-Path $tempDir "$name.exe"
    Write-Host ""
    Write-Host "파일: $($file.Name)" -ForegroundColor "Yellow"
    Write-Host "---"

    $output = & gcc -std=c11 -g -Wall -Wextra -Werror $file.FullName -o $outFile 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  [PASS] 컴파일 성공 (경고 없음)" -ForegroundColor "Green"
        Write-Host "실행:"
        & $outFile 2>&1 | Select-Object -First 10
    } else {
        Write-Host "  [FAIL] 컴파일 실패" -ForegroundColor "Red"
        Write-Host $output | Select-Object -First 15
    }
}

# 3. 정적 분석 (cppcheck)
Write-Host ""
Write-Host "=== 정적 분석 (cppcheck) ===" -ForegroundColor "Cyan"
$cppcheck = Get-Command cppcheck -ErrorAction SilentlyContinue
if ($cppcheck) {
    Write-Host "나쁜 코드 분석:"
    & cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem $badDir 2>&1 | Select-Object -First 20
    Write-Host ""
    Write-Host "좋은 코드 분석:"
    & cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem $goodDir 2>&1 | Select-Object -First 20
} else {
    Write-Host "cppcheck 미설치 - 정적 분석을 건너뜁니다." -ForegroundColor "Yellow"
    Write-Host "설치: pacman -S mingw-w64-x86_64-cppcheck"
}

# 정리
Remove-Item -Path $tempDir -Recurse -Force -ErrorAction SilentlyContinue

Write-Host ""
Write-Host "=========================================" -ForegroundColor "Cyan"
Write-Host " 모든 예제 검증 완료!" -ForegroundColor "Green"
Write-Host "=========================================" -ForegroundColor "Cyan"