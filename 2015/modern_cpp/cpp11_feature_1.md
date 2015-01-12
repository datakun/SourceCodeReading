# 2015.01.12
## Static assert
- http://msdn.microsoft.com/ko-kr/library/dd293588.aspx
- 컴파일 단계에서 Assertion 테스트 수행.
- 지정된 식이 False일 경우, 에러와 함께 컴파일 중단.
- True일 경우, 프로그램에 영향을 끼치지 않고 컴파일 완료.
- 예제
```cpp
static_assert(constant-expression, string-literal);
```
1. constant-expression : bool 결과를 반환할 수 있는 식
2. string-literal : 컴파일 실패시 표시할 메시지(컴파일러의 기본 문자 집합의 문자를 이용)

```cpp
int a = 10;
static_assert(a > 10, "a is under then 10");
```

## Auto
- 

## Trailing return types
