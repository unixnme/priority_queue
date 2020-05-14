#include <iostream>
#include <string>
#include <vector>
#include "hara/PriorityQueue.h"

template<typename Queue>
bool CheckIdentical(Queue &a, Queue &b) {
    if (a.Size() != b.Size())
        return false;
    while (!a.Empty()) {
        if (a.Top() != b.Top())
            return false;
        a.Pop(), b.Pop();
    }
    return true;
}

template<typename Queue>
bool CheckCopyConstructor(Queue &q) {
    Queue copy{q};
    return CheckIdentical(q, copy);
}

template<typename Queue>
bool CheckOrder(Queue q) {
    auto n = q.Size();
    std::vector<std::pair<typename Queue::K, typename Queue::V>> result;
    while (!q.Empty()) {
        result.push_back(q.Top());
        q.Pop();
    }

    if (n != result.size()) return false;
    for (auto i = 1; i < n; ++i) {
        if (typename Queue::C{}(result[i - 1].second, result[i].second))
            return false;
    }
    return true;
}

template<typename Queue>
bool CheckMoveConstructor(Queue q) {
    auto n = q.Size();
    Queue moved{std::move(q)};
    return !q && moved.Size() == n;
}

template<typename Queue>
bool CheckMoveOperator(Queue q) {
    auto n = q.Size();
    Queue moved;
    moved = std::move(q);
    return !q && moved.Size() == n;
}

template<typename Queue>
bool CheckCopyOperator(Queue q) {
    Queue copy;
    copy = q;
    return CheckIdentical(q, copy);
}

template<typename Queue>
void Print(Queue q) {
    while (!q.Empty()) {
        std::cout << q.Top().first << ": " << q.Top().second << std::endl;
        q.Pop();
    }
}

int main() {
    std::vector<std::pair<int, std::string>> values = {{0,  "a"},
                                                       {1,  "z"},
                                                       {2,  "f"},
                                                       {10, "axx"},
                                                       {-2, "exh"}};

    PriorityQueue<int, std::string, std::greater<std::string>,
            PriorityQueueImpl1<int, std::string, std::greater<std::string>>> queue{values.begin(), values.end()};

    assert(CheckCopyConstructor(queue));
    assert(CheckOrder(PriorityQueue<int, std::string>{values.begin(), values.end()}));
    assert(CheckMoveConstructor(PriorityQueue<int, std::string>{values.begin(), values.end()}));
    assert(CheckCopyOperator(PriorityQueue<int, std::string>{values.begin(), values.end()}));
    assert(CheckMoveOperator(PriorityQueue<int, std::string>{values.begin(), values.end()}));
    Print(PriorityQueue<int, std::string, std::greater<std::string>>{{0, "c"},
                                                                     {1, "b"},
                                                                     {5, "A"},
                                                                     {4, "a"},
                                                                     {2, "z"},
                                                                     {3, "X"}});

    PriorityQueue<int, std::string, std::greater<std::string>,
            PriorityQueueImpl2<int, std::string, std::greater<std::string>>> queue2{values.begin(), values.end()};
    auto copy1{queue2};
    assert(CheckCopyConstructor(queue2));
    assert(CheckOrder(copy1));
    assert(CheckMoveConstructor(copy1));
    assert(CheckCopyOperator(copy1));
    assert(CheckMoveOperator(copy1));
    Print(PriorityQueue<int, std::string, std::less<std::string>,
            PriorityQueueImpl2<int, std::string, std::less<std::string>>>{{0, "c"},
                                                                             {1, "b"},
                                                                             {5, "A"},
                                                                             {4, "a"},
                                                                             {2, "z"},
                                                                             {3, "X"}});

    return 0;
}
