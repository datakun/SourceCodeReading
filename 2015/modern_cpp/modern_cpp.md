# C++

## C++14 변경점 정리

### Rvalue references
#### 목적
1. "move" 의 구현 (move semantics)
2. 완벽한 전달(perfect forwarding)

#### Rvalue, Lvalue
> 좌측값은 대입(assignment) 시에 왼쪽 혹은 오른쪽에 오는 식(expression)이고, 우측값은 대입 시에 오직 오른쪽에만 오는 식이다"

```cpp
int x = 1;
int y = 2;

// x 와 y 는 모두 좌측값
x = y; // ok
y = x; // ok
x = x * y; // ok

// a * b 는 우측값
int z = x * y; // ok. 우측값이 대입 연산에서 우측에 있으니까
x * y = 42; // error. 우측값이 대입 연산에서 좌측에 있으니까
```
#### Rvalue, Lvalue by C++
> "좌측값은 어떠한 메모리 위치를 가리키는데, & 연산자를 통해 그 위치를 참조할 수 있다. 우측값은 좌측값이 아닌 값들이다"

```cpp
// 좌측값들
int x = 1;
x = 2; //
int* p = &i; // i의 메모리 주소
int& foo(); // foo의 메모리 주소
foo() = 42; // foo()는 좌측값
int* p1 = &foo(); // foo()의 주소값

// 우측값
int foo2(); // int 를 리턴하는 함수
int y = 0;
y = foo2(); // foo2가 우측값
int* p2 = &foo2(); // 우측값의 주소는 참조불가
y = 3; // '3'은 우측값
```
#### Move의 구현(Move Semantics)
클래스 A를 아주 거대한 '객체'의 포인터를 담고 있는 클래스라 가정하자. 예를 들어서, std::vector라 가정한다. 클래스 A를 복사하는 가장 보편적인 방법은 다음과 같다.
```cpp
A& A::operator=(A const & rhs)
{
// [...]
// m_pResource_리소스 소멸
// rhs.m_pResource_복제된 클래스 생성
// m_pResource_복제된 버전으로 변경
// [...]
}
A foo(); // foo() 는 X 타입의 객체를 리턴하는 함수 즉 우측값
A a;
A = foo();
```

이 우측값을 '이동' 시키는 연산이 이를 바로 move 연산이다. 그리고 우측값 이동을 위해서 당연히 객체를 새로 생성하는 것이 아니라 우측값의 메모리 위치를 교환하는 것이 훨씬 '효율'이 좋다. 따라서 이 move 연산을 위해서 꼭 필요한 연산이 'Rvalue references'이다. C++11 에서는 아래와 같은 함수의 오버로딩으로 구현할 수 있습니다.

```cpp
X& X::operator=(<Any Type> rhs)
{
// [...]
//  m_pResource 와 rhs.m_pResource 를 교환
// [...]
}
```

우리의 'Any Type'은 레퍼런스여야 한다. 또한 'Any Type'이 지켜야 할 것은 기존의 레퍼런스(&)와 차별화를 두어서 'Any Type'과 '일반 레퍼런스 타입'을 사용한 두 함수 사이에서 좌측값의 경우 '보통의 레퍼런스 타입'을, 우측값의 경우 'Any Type'을 택하도록 만들어야 한다. 그래서 C++ 개발자들은 이 'Any Type' 우측값 참조(rvalue reference) 라는 이름을 붙였다.

#### 우측값 참조(rvalue reference)
임의의 타입 A에 대해 A&& 를 A 의 우측값 참조라고 정의한다. 또, 쉽게 구별하기 위해 기존의 레퍼런스 A& 를 좌측값 참조 라고 부르도록 가정하자. 우측값 참조는 기존의 레퍼런스 A& 와 몇 가지 예외를 제외하고는 유사하게 작동한다. 다만 둘의 가장 큰 차이점은 함수 오버로딩에서 좌측값은 좌측값 레퍼런스를, 우측값은 우측값 레퍼런스를 선호한다는 것이다.

```cpp
void foo(A& x); // 좌측값 참조 오버로드
void foo(A&& x); // 우측값 참조 오버로드

A a;
A foo2();

foo(x); // 인자에 좌측값이 들어 갔으므로 좌측값 참조 함수가 오버로딩
foo(foo2()); // 인자에 우측값이 들어 갔으므로 우측값 참조 함수가 오버로딩
```

대다수의 경우 move 연산을 위해서는 우측값 참조로 인자를 받는 경우는 복사 생성자나 대입 연산자들 밖에 없다.

```cpp
A& A::operator=(A const & rhs); // 기존의 구현 방법
A& A::operator=(A&& rhs)
{
  // Move 연산: this 와 rhs 의 내용을 swap 한다.
  return *this;
}
```

### ref-qualifiers // TODO

### Variadic templates
Variadic Template을 지원한다는 것은 임의의 많은 template parameter를 취하는 템플릿을 지원한다는 것을 의미한다. 'template<typename ... Values> class tuple;'

```cpp
// 세개의 template 인자를 갖는 some_instance_name 
// tuple<int, std::vector<int>, std::map<std::string, std::vector<int>>> some_instance_name;
//인자의 개수가 0개여도 괜찮습니다.
// tuple<> a;
// 몇개의 인자를 가지는 Variadic templates
// template<typename T, typename ... Values> class tuple;
#include <iostream>
using namespace std;
 
class dog {
public:
    void speak() const { cout << "bow wow" << endl; }
};

class cat {
public:
    void speak() const { cout << "meow" << endl; }
};
 
template <typename T>
void speak(const T& p)
{
    p.speak();
}
 
template <typename T, typename ... ARGS>
void speak(const T& p, ARGS ... args)
{  
    p.speak();
    speak(args...);
}

/*
template <class Dog, class Cat, class ... ARGS>
void speak(const Dog& d, const Cat& c, ARGS ... args)
{
    d.speak();
    speak(args...);
}
*/
 
int main()
{
    dog a, b, c, d;
    cat e, f, g, h;
    speak(a, e, b, f, c, g, d, h);
    //speak(e, b, f, c, g, d, h); //Compile Error!
    return 0;
}

```


### static_assert
복잡한 문제를 해결하기 위해서, 무한 추적점, 브레이크포인트, 디버깅요소들을 무수히 많은 방법을 사용했을 것이다. 그중에서는 assert, #error를 많이 사용할것이다. 하지만, 이 둘은 불편한점이 있다. 만약, 런타임에 생기는 미세한 요소를 잡는데에는 assert를 쓸 수 있겠지만, C++에서 설계할때, 클래스상의 에러를 잡을땐 크게 불편하다. C++로 하는데 프로젝트가 작을리는 없고, 빌드타임도 너무 길어서 런타임까지 가서 디버깅하기가 너무 힘들다. C++0x에 추가될 static_assert는 컴파일타임에 assert를 해주는 키워드이다.

```cpp
// static_assert(/*조건식(Expression)*/, /*에러메세지*/);
#include <iostream>
using namespace std;
const int assertNum = 1000;
int main(void)
{
	static_assert(assertNum == 1000, "assertNum is 1000");
	return 0;
}
```

### auto

### Trailing return types(리턴 타입 추적하기), decltype
```cpp
auto GetCPUSpeedInHertz() -> long
{
    return 1234567890;
}
// long 타입을 리턴한다.
```

```cpp
auto GetPI() -> decltype( 3.14 )
{
    return 3.14159;
}
// 리턴타입이 decltype으로 추론

template <typename FirstType, typename SecondType>
auto AddThem( FirstType t1, SecondType t2 ) -> decltype( t1 + t2 )
{
    return t1 + t2;
}
// 리턴타입이 decltype으로 추론
```

decltype 설명에서 언급했듯이 타입은 표현식으로 결정이 된다. t1 + t2 에 대한 타입이 결정되는 것이다. 만약 컴파일러가 타입 업그레이드가 가능하다면 그렇게 할 것이다. 만약 타입이 class의 일종이고 overloaded 된 operator+ 가 호출된다면 operator+ 의 리턴타입이 최종 리턴타입으로 결정될 것이다. 이 타입추론이 실제 template 함수를 특정 데이터타입과 함께 인스턴스화 될 때 정해진다는 사실은 매우 중요하다.


### Lambdas
일반적으로 람다는 아래와 같은 뜻으로 사용된다.
* 상태를 유지하는 익명 함수익명 함수(Anonymous Function)를 뜻함
* 바깥쪽 범위에 사용할 수 있는 변수에 액세스할 수 있음

기본적으로 람다는 함수 객체처럼 암시적으로 함수 객체 클래스를 만들고, 함수 객체를 생성해서 전달합니다. 일반적인 람다의 사용법은 아래와 같다.
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;
    for(int i = 0 ; i < 5 ; i++) {
        v.push_back(i);
    }

    cout << "Original Loop" << endl;
    for(auto it = v.begin() ; it != v.end() ; ++it) {
        cout << *it << endl;
    }

    cout << "Lambda Loop" << endl;
    for_each(v.begin(), v.end(), [](int i) {
        cout << i << endl;
    });
}
```

함수 포인터는 아래와 같은 단점이 있습니다.
* 함소 포인터가 상태를 유지하는 게 불가능하고
* 함수 객체는 별도의 클래스를 정의해야 하는 불편함

람다는 이 둘의 단점을 보완해서 장점만을 가져왔다. 직접적으로 말해서 람다는 별도의 클래스를 만들지 않고, 상태를 유지하는게 가능하다.


```cpp
// ver. 초보자
vector<int>::const_iterator iter = cardinal.begin();
vector<int>::const_iterator iter_end = cardinal.end();
int total_elements = 1;
while( iter != iter_end )
{
	total_elements *= *iter;
	++iter;
}
```

```cpp
// ver. 난 좀 함
int total_elements = 1;
for_each( cardinal.begin(), cardinal.end(), product<int>(total_elements) );
template <typename T>
struct product
{
	product( T & storage ) : value(storage) {}
	template< typename V>
	void operator()( V & v )
	{
		value *= v;
	}
	T & value;
};
```

```cpp
// ver. 람다
int total_elements = 1;
for_each( cardinal.begin(), cardinal.end(), [&total_elements](int i){total_elements *= i;} );
```

### Right angle brackets
템플릿을 써본 C++ 프로그래머는 누구나 다음과 같은 현상을 겪어 보았을 것이다.
```cpp
#include <vector>  
typedef std::vector<std::vector<int> > Table;  // OK  
typedef std::vector<std::vector<bool>> Flags;  // 오류  
```

두 번째 문장에서 컴파일 오류가 나는 것은, 컴파일러가 '>>'를 두 개의 '>'가 아니라 하나의 '>>' 연산자로 인식하기 때문입니다. 이는 사실 좀 당황스럽고 짜증나는 일이다. 그런데 이런 일이 발생하는 것은 그렇다고 이 문제가 단지 기존 표준 제정자들이 당연한 뭔가를 간과했기 때문인 것만은 아니다. 컴파일러가 >>를 >>라는 단일한 연산자로 인식하는 것은 소위 ‘maximum munch’라는 원칙을 따른 것이다. 이 최대한 물어뜯기 원칙은 예를 들어 i+++j가 i+ (++j)가 아니라 (i++) +j 로 해석되게 하는 것으로, C++ 소스 코드의 파싱을 좀 더 간단하게 만드는 중요한 원칙이다. 따라서 단순히 ‘이제부터는 >>가 컴파일 오류를 일으키지 않게 하자’라고 선언하는 것으로 문제가 해결되지는 않는다. 적어도 템플릿 구문에 대해서는 이러한 원칙과는 다른 새로운 해석 규칙을 만들어야 한다. 

표준 C++0x 에서는 아래와 같이 붙여서 사용이 가능하다. 

```cpp
typedef std::vector<std::vector<bool>> Flags;  // C==0x에서 OK  
```

### Default template args for function templates
C++에서는 아래와 같이 템플릿 클래스에 대해서 기본 생성자를 지정할 수 있다
```cpp
#include <iostream>
using namespace std;
template <class T = long>class A
{
public:
	T a;
    T getA()
    {
    	return this->a;
    }
    void setA(T a)
    {
    	this->a = a;
    }
};

//template <typename B, typename T = int> void Bar(b b = 0, T t = 0) { }
//Bar(10);

int main()
{
	A<> aa;
    aa.setA(10L);
    cout << aa.getA();
    return 0;
}
```