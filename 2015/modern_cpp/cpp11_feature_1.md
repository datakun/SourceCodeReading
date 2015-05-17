# 2015.01.12
## static_assert
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
<<<<<<< HEAD

## auto
- 타입을 추론하는 선언 지정자.
- 초기화 식의 타입이 변화하는 경우 바로 동작 및 보장함.
- 타입 이름의 오타를 걱정 할 필요 없어서 효율적으로 코딩 가능.
- 명확한 초기화 식이 아닐 땐, auto를 사용하지 말자.


## Trailing return types
=======
>>>>>>> 69a3be80bbba4d0fb6841ca4b92f3d6b1a4944bf
