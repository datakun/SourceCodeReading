- C++ 11 에서 생성자 오버로딩 시, 다른 생성자를 오버로딩 된 생성자에서 사용하고 싶다.
  - https://en.wikipedia.org/wiki/C++11#Object_construction_improvement
  ```
  class SomeType  {
      int number;
   
  public:
      SomeType(int new_number) : number(new_number) {}
      SomeType() : SomeType(42) {}
  };
  ```
