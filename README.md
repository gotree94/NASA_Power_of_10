# NASA "Power of 10" 코딩 규칙 - 교육 자료

> 우주선 시스템을 어떻게 �코딩할까? 극한 C 프로그래밍 가이드

## 이 교육 자료의 목적

미국 항공우주국(NASA)의 **"Power of 10"** 코딩 규칙은 안전이 중요한 소프트웨어, 특히 C 프로그래밍 언어로 개발되는 소프트웨어를 위한 지침입니다. 정적 분석(static analysis)을 용이하게 하고 잠재적으로 위험한 코딩 관행을 줄이기 위해 마련된 이 규칙들은本次 교육을 통해 실습 중심으로 학습합니다.

## 학습 목표

1. NASA Power of 10 규칙의 배경과 철학을 이해한다
2. 각 규칙이 왜 필요한지 근거를 학습한다
3. 나쁜 코드 예시와 좋은 코드 예시를 비교한다
4. 실제 개발 환경에서 정적 분석 도구를 사용하여 검증한다
5. 다양한 프로그래밍 환경에서의 적용 가능성을 분석한다

## 교육 자료 구성

```
NASA_Power_of_10_교육자료/
├── README.md                          # 이 파일 - 전체 오버뷰
├── chapters/
│   ├── chapter_01_개요_및_배경.md       # Power of 10 소개
│   ├── chapter_02_규칙_1_3.md          # 규칙 1-3: 제어흐름, 반복문, Goto/재귀
│   ├── chapter_03_규칙_4_6.md          # 규칙 4-6: 메모리, 스코프, 반환값
│   ├── chapter_04_규칙_7_9.md          # 규칙 7-9: 전처리기, 포인터, 경고
│   ├── chapter_05_실습_환경_세팅.md     # 개발 환경 준비 및 검증
│   └── chapter_06_논평_및_분석.md       # 비판적 분석과 균형잡힌 시각
├── code_examples/
│   ├── bad/                           # Power of 10을 위반하는 나쁜 예시
│   │   ├── bad_rule01_goto.c
│   │   ├── bad_rule02_unbounded_loop.c
│   │   ├── bad_rule03_recursion.c
│   │   ├── bad_rule04_dynamic_alloc.c
│   │   ├── bad_rule05_wide_scope.c
│   │   ├── bad_rule06_no_return_check.c
│   │   ├── bad_rule07_preprocessor.c
│   │   ├── bad_rule08_pointer_arith.c
│   │   └── bad_rule09_no_warnings.c
│   └── good/                          # Power of 10을 준수하는 좋은 예시
│       ├── good_rule01_structured_flow.c
│       ├── good_rule02_bounded_loop.c
│       ├── good_rule03_iterative.c
│       ├── good_rule04_static_alloc.c
│       ├── good_rule05_narrow_scope.c
│       ├── good_rule06_return_check.c
│       ├── good_rule07_no_preprocessor.c
│       ├── good_rule08_safe_pointer.c
│       └── good_rule09_clean_compile.c
├── Makefile                           # 전체 빌드 및 검증 스크립트
└── verify.sh                          # 검증 자동화 스크립트
```

## 빠른 시작

```bash
# 1. 환경 세팅 (Linux/Mac)
chmod +x verify.sh
./verify.sh setup

# 2. 나쁜 코드 컴파일 및 경고 확인
make bad

# 3. 좋은 코드 컴파일 및 검증
make good

# 4. 정적 분석 실행
make analyze
```

## 참고 자료

- 원본 영상: https://youtu.be/gKWJbdtHLe4
- NASA JPL Power of 10: https://users.ece.cmu.edu/~adrian/731-sp04/readings/GL-vda-c.pdf
- Gerard J. Holzmann의 원본 논문

## 학습 순서

| 순서 | 챕터 | 내용 | 예상 시간 |
|------|------|------|----------|
| 1 | Chapter 01 | 개요 및 배경 | 15분 |
| 2 | Chapter 02 | 규칙 1-3 | 30분 |
| 3 | Chapter 03 | 규칙 4-6 | 30분 |
| 4 | Chapter 04 | 규칙 7-9 | 25분 |
| 5 | Chapter 05 | 실습 환경 세팅 | 20분 |
| 6 | Chapter 06 | 논평 및 분석 | 15분 |
| - | 실습 | 코드 예시 실행 및 검증 | 30분 |

**총 예상 학습 시간: 약 2시간 45분**
