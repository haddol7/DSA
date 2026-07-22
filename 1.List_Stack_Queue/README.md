# LC1

* 배열은 캐시 지역성에 유리함
* 캐시는 cache line 단위 (보통 64 byte)로 메모리를 가져오므로, int 배열이면 원소 하나 접근 시 16개가 같이 가져와지는 셈
* 캐시 지역성은 Big O에는 안보이는 요소지만, 실측에선 의미가 있음
* 캐시 미스가 발생했을 경우 L1 히트보다 50 ~ 100배 정도 느림

* vector 랑 list의 중간 삽입은 Big O 표기 상 list가 O(1)로 유리함
    * 캐시 지역성과 연결지어보면, list는 노드가 힙에 흩어져 있기 때문에 순회할 때마다 캐시 미스가 발생함

### 구조체 레이아웃

* AoS (Array of Sturcture)
```
struct Particle { Vec3 pos; Vec3 vel; float life; Color c; };
std::vector<Particle> particles;
```

* SoA (Structure of Arrays)
```
struct Particles {
    std::vector<Vec3> pos;
    std::vector<Vec3> vel;
    std::vector<float> life;
    std::vector<Color> c;
}
```

만약 위치만 업데이트하는 루프에서는 AoS는 사용하지 않는 life, c까지 캐시에 가져와져서 캐시라인을 낭비하게 됨. SoA는 필요한 데이터만 연속으로 가져오기에 캐시 효율 및 SIMD 벡터화에 유리함 

-> 이게 Data-Oriented Design(DOS)이고, Unity DOTS/ECS, UE Mass 프레임워크 기반임.

* std::array
- '[]' 연산자 - Out of Range 시 UB (Seg fault 포함)
- .at() 연산자 - std::out_of_range throw 함
- C 스타일 배열과 동일한 래퍼 클래스
- void foo(std::array<int, 100> arr); // 호출 시 int 100개를 전체 복사
- void bar(int ary[100]); // 포인터 8바이트만 전달. C 배열의 decay
- 크기 비교 연산자 사용 시, 크기가 동일해야 함
- iterator 순회 중에 push_back 등을 하면 전부 무효화되므로 주의

```
template <size_t N>
void print(std::array<int, N> arr);
```

* std::vector
- push_back(), insert()보다, emplace(), emplace_back()이 성능 향상에 도움이 될 수 있음
    - 생성 비용이 있는 타입 (클래스, string 경우)은 전자의 경우 임시 객체의 생성, 이동, 삭제가 발생
    - trivial한 타입에는 딱히 도움은 안됨 (int 등)
    - vec.push_back(s) - 복사
    - vec.push_back(std::move(s)) - 이동

* lvalue & rvalue
- 일단 컴파일러는 마법이 아니다.
```
string(const string& other);    //lvalue
string(string&& other);         //rvalue
```
- 이 오버로딩된 함수를 컴파일러는 카테고리?만을 보고 판단한다.
- lvalue와 rvalue의 "유래" - 정확한 의미가 아님
    - x (left value) = 42; (right value)
    - 메모리 주소에 답할 수 있으면 lvalue, 없으면 rvalue
    - type과 카테고리(l,r)는 별개의 축이니 헷갈리지 말도록
- const citizen& c 같이 함수 인자를 받는 경우에는 rvalue, lvalue 둘 다 들어올 수 있음
    - 그리고 인자 c는 함수 내부에서 lvalue로 동작하게 됨
    - foo(citizen{"daeha", 25}); <- rvalue이며, 스택에는 실재하지만 이름이 없음 (걍 C++ 규정 상 우리가 접근할 수 없음)
    - foo(...) { c ~~ } <- c는 lvalue가 되었고, 이제 c라는 이름이 생겨버림. 그렇다면 얘는 주소를 물을 수 있게 됨
    - 아~ rvalue는 주소가 찾을 수 있냐 없냐(trivial value인지 아닌지)가 아니라, 그값을 실질적으로 물어볼 수 있냐 없냐의 차이구나

```
void f(citizen&& c) //rvalue만 받습니다
{
    &c //이제 c는 lvalue
    g(c) //이제 lvalue입니다
    g(std::move(c)) //하지만 이건 rvalue입니다
}  
```

* 람다식
1. functor - C++98
```
struct IsMinor
{
    bool operator()(const ciziten& c) const { return c.age < 19; } 
};

citizens.remove_if(IsMinor{});
```
이렇게 객체를 넘겨서 ()를 붙여 함수처럼 호출할 수 있는 functor가 있었음
remove_if 내부에서 pred(c)라고 호출하면, operator()가 실행됨
-> 람다도 컴파일러에서는 이것과 똑같이 동작함. syntactic sugar

2. 함수 포인터 - C스타일
```
bool isMinor(const citizen& c) { return c.age < 19; }
citizens.remove_if(isMinor) // 함수 포인터 decay
```
얘는 근데 캡처가 안됨. 19 대신 변수를 못 넣음.
```
citizens.remove_if([age](const citizen& c){ return c.age < age; });
```

3. Iterator - C++ 단순 스타일
```
for (auto it = citizens.begin(); it != citizens.end(); ++it)
{
    if (it->age < 19) citizens.erase(it); //이렇게 하면 it이 댕글링
}
for (auto it = citizens.begin(); it != citizens.end();)
{
    if (it-> age < 19) it = citizens.erase(it);
    else ++it;
}
```
- 확실히 remove_if가 훨씬 깔끔하다