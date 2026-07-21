#include <iostream>
#include <sstream>
#include <algorithm>

struct student
{
    std::string name;
    int standard;
};

std::ostream& operator<<(std::ostream& os, const student& s)
{
    return (os << "[" << s.name << ", " << s.standard << "]");
}

template <typename T>
class dynamic_array
{
    T* data;
    size_t n;

public:
    dynamic_array(size_t n)
    {
        this->n = n;
        data = new T[n];
    }
    dynamic_array(const dynamic_array<T>& arr)
    {
        n = arr.n;
        data = new T[n];
        for (size_t i = 0; i < n; i++) data[i] = arr[i];
    }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    T& at(size_t index)
    {
        if (index < n) return data[index];
        throw "Index Out of range";
    }
    size_t size() const { return n; }
    ~dynamic_array() { delete[] data; }

    T* begin() { return data; }
    const T* begin() const { return data; }
    T* end() { return (data + n); }
    const T* end() const { return (data + n); }

    friend dynamic_array<T> operator+(const dynamic_array<T>& arr1, const dynamic_array<T>& arr2)
    {
        dynamic_array<T> ret(arr1.size() + arr2.size());
        std::copy(arr1.begin(), arr1.end(), ret.begin());
        std::copy(arr2.begin(), arr2.end(), ret.begin() + arr1.size());

        return ret;
    }

    std::string to_string(const std::string& sep = ", ")
    {
        if (n == 0) return "";

        std::ostringstream os;
        os << data[0];

        for (size_t i = 1; i < n; i++) os << sep << data[i];
        return os.str();
    }

};

int main()
{
    int nStudents;
    std::cout << "1반 학생 수 입력: ";
    std::cin >> nStudents;

    dynamic_array<student> class1(nStudents);
    for (size_t i = 0; i < class1.size(); i++)
    {
        std::string name;
        int standard;
        std::cout << i + 1 << "번째 학생과 나이를 입력하세요: ";
        std::cin >> name >> standard;
        class1[i] = student{name, standard};
    }

    auto class2 = class1;
    std::cout << class2.to_string() << std::endl;

    std::cout << (class1 + class2).to_string() << std::endl;
    return (0);
}