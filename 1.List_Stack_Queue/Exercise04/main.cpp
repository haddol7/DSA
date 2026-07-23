#include <iostream>
#include <forward_list>
#include <vector>

int main()
{
    std::vector<std::string> vec = {
        "Lewis Hamilton", "Ha daejin", "Kim Ddobbi", "Test", "쓰기 귀찮다"
    };

    auto it = vec.begin(); //당연히 상수 시간
    std::cout << "최근 우승자 : " << *it << std::endl;

    it += 1;
    std::cout << *it << std::endl;
    std::advance(it, -1);
    std::cout << *it << std::endl;

    std::forward_list<std::string> fwd(vec.begin(), vec.end());

    auto it1 = fwd.begin();
    std::advance(it1, 1);
    std::cout << *it1 << std::endl;
    //std::advance(it1, -1); UB
    std::cout << *it1 << std::endl;

    return 0;
}