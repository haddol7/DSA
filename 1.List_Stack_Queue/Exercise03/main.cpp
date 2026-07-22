#include <iostream>
#include <forward_list>
#include <string>

struct citizen
{
    std::string name;
    int age;
};

std::ostream &operator<<(std::ostream &os, const citizen &c)
{
    return (os << "[" << c.name << ", " << c.age << "]");
}

int main()
{
    std::forward_list<citizen> citizens = {
        {"Kim", 22}, {"Lee", 0}, {"Ha", 26}, {"Jim", 12312323}
    };
    auto citizens_copy = citizens;

    std::cout << "전체 시민들: ";
    for (const auto& c : citizens)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    citizens.remove_if([](const citizen &c) {
        return (c.age < 19);
    });

    std::cout << "투표권이 있는 시민들 : ";
    for (const auto &c : citizens) std::cout << c << " ";
    return 0;
}