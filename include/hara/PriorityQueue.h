#ifndef PQUEUE_PRIORITYQUEUE_H
#define PQUEUE_PRIORITYQUEUE_H

#include "PriorityQueueImpl.h"

template<typename Key, typename Value,
        typename Compare = std::less<Value>,
        typename Impl = PriorityQueueImpl1<Key, Value, Compare>>
class PriorityQueue {
public:
    using K = Key;
    using V = Value;
    using C = Compare;

    PriorityQueue() : impl{new Impl} {};

    PriorityQueue(const PriorityQueue &copy) : impl{new Impl{*copy.impl}} {}

    PriorityQueue(PriorityQueue &&that) noexcept:
            impl{that.impl} { that.impl = nullptr; }

    template<typename Iterator>
    PriorityQueue(Iterator begin, Iterator end) : impl{new Impl{begin, end}} {}

    ~PriorityQueue() { delete impl; };

    explicit operator bool() const { return impl != nullptr; }

    PriorityQueue &operator=(const PriorityQueue &copy) {
        if (this == &copy) return *this;
        delete impl;
        impl = new Impl{*copy.impl};
        return *this;
    }

    PriorityQueue &operator=(PriorityQueue &&that) noexcept {
        if (this == &that) return *this;
        impl = that.impl;
        that.impl = nullptr;
        return *this;
    }

    bool operator==(const PriorityQueue<K, V, C> &that) = delete;

    const std::pair<K, V> &Top() const { return impl->Top(); }

    void Pop() { impl->Pop(); }

    size_t Size() const { return impl->Size(); }

    bool Empty() const { return impl->Empty(); }

    void InsertOrUpdate(std::pair<K, V> pair) { impl->InsertOrUpdate(std::move(pair)); }

    void Erase(const K &key) { impl->Erase(key); }

    bool Contain(const K &key) const { return impl->Contain(key); }

    /**
     * throws exception if key not found
     * @param key
     * @return
     */
    const V &Peek(const K &key) const { return impl->Peek(key); }

private:
    Impl *impl;
};

#endif //PQUEUE_PRIORITYQUEUE_H
