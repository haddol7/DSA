# LC1

* 배열은 메모리 청크이며, 각 원소가 인접하기 때문에 하나의 원소에 접근할 때 주변 원소도 함께 캐시로 가져오기 때문에 캐시 지역성에 유리함
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

만약 위치만 업데이트하는 루프에서는 AoS는 사용하지 않는 HP, c까지 캐시에 가져와져서 캐시라인을 낭비하게 됨. SoA는 필요한 데이터만 연속으로 가져오기에 캐시 효율 및 SIMD 벡터화에 유리함 

-> 이게 Data-Oriented Design(DOS)이고, Unity DOTS/ECS, UE Mass 프레임워크 기반임.

* std::array
- '[]' 연산자 - Out of Range 시 UB (Seg fault 포함)
- .at() 연산자 - std::out_of_range throw 함
- C 스타일 배열과 동일한 래퍼 클래스
- void foo(std::array<int, 100> arr); // 호출 시 int 100개를 전체 복사
- void bar(int ary[100]); // 포인터 8바이트만 전달. C 배열의 decay
- 크기 비교 연산자 사용 시, 크기가 동일해야 함
- iterator 순회 중에 push_back 등을 하면 전부 무효화되므로 주의
-

```
template <size_t N>
void print(std::array<int, N> arr);
```