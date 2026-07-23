#include <iostream>
#include <algorithm>

class singly_ll
{
    struct singly_ll_node
    {
        int data;
        singly_ll_node* next;
    };

public:
    using node = singly_ll_node;
    using node_ptr = node*;

private:
    node_ptr head;

public:
    void push_front(int val)
    {
        auto new_node = new node{val, NULL};
        if (head != NULL) new_node->next = head;
        head = new_node;
    }

    void pop_front()
    {
        auto first = head;
        if (head != NULL)
        {
            head = head->next;
            delete first;
        }
    }

    struct singly_ll_iterator
    {
    private:
        node_ptr ptr;
    public:
        singly_ll_iterator(node_ptr p) : ptr(p) {}
        int& operator*() { return ptr->data; }
        node_ptr get() { return ptr; }
        singly_ll_iterator& operator++()
        {
            ptr = ptr->next;
            return *this;
        }
        singly_ll_iterator operator++(int)
        {
            singly_ll_iterator result = *this;
            ++(*this);
            return result;
        }

        friend bool operator==(const singly_ll_iterator& left, const singly_ll_iterator& right) { return (left.ptr == right.ptr); }
        friend bool operator!=(const singly_ll_iterator& left, const singly_ll_iterator& right) { return (left.ptr != right.ptr); }
    };

    singly_ll_iterator begin() { return singly_ll_iterator(head); }
    singly_ll_iterator end() { return singly_ll_iterator(NULL); }
    singly_ll_iterator begin() const { return singly_ll_iterator(head); }
    singly_ll_iterator end() const { return singly_ll_iterator(NULL); } //이게 무슨 의미가 있는거지?

    singly_ll() = default;
    singly_ll(const singly_ll& other) : head(NULL)
    {
        if (other.head)
        {
            head = new node{0, NULL};
            auto cur = head;
            auto it = other.begin();
            while (true)
            {
                cur->data = *it;
                auto tmp = it;
                ++tmp;
                if (tmp == other.end()) break;
                cur->next = new node{0, NULL};
                cur = cur->next;
                it = tmp;
            }
        }
    }

    singly_ll(const std::initializer_list<int>& ilist) : head(NULL)
    {
        for (auto it = std::rbegin(ilist); it != std::rend(ilist); it++) push_front(*it);
    }

    ~singly_ll() { while (head) pop_front(); }
    singly_ll& operator=(singly_ll other)
    {
        std::swap(head, other.head);
        return *this;
        // 최적화(기존 head의 자원을 delete하지 않는 방식)는 안되지만.. 우아한(?) 트릭
    }
};

int main()
{
    singly_ll sll = {1, 2, 3};
    sll.push_front(0);

    std::cout << "첫번째 리스트 : ";
    for (auto i : sll) std::cout << i << " ";
    std::cout << std::endl;

    auto sll2 = sll;
    sll2.push_front(-1);
    std::cout << "두번째 리스트 : ";
    for (auto i : sll2) std::cout << i << " ";
    std::cout << std::endl;
    return 0;
}