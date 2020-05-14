#ifndef PQUEUE_PRIORITYQUEUE_H
#define PQUEUE_PRIORITYQUEUE_H

#include "PriorityQueueImpl.h"

/**
 * Priority Queue support Key,Value update & look up
 * @tparam Key : unique keys for each priority values
 * @tparam Value : priority values
 * @tparam Compare : priority value comparison; default is max priority queue
 * @tparam Impl : implementation choice; see PriorityQueueImpl.h
 */
template<typename Key, typename Value,
        typename Compare = std::less<Value>,
        typename Impl = PriorityQueueImpl1<Key, Value, Compare>>
class PriorityQueue {
public:
    using K = Key;
    using V = Value;
    using C = Compare;

    /**
     * Empty queue
     */
    PriorityQueue() : impl{new Impl} {
        static_assert(std::is_same<typename Impl::C, Compare>::value, "");
    };

    /**
     * copy from exisiting queue
     *
     * complexity: O(N)
     */
    PriorityQueue(const PriorityQueue &copy) : impl{new Impl{*copy.impl}} {
        static_assert(std::is_same<typename Impl::C, Compare>::value, "");
    }

    /**
     * move construction
     * once moved, 'that' will be invalidated
     * any attempt to call methods of 'that' after move will result in seg-fault
     *
     * complexity: O(1)
     */
    PriorityQueue(PriorityQueue &&that) noexcept:
            impl{that.impl} { that.impl = nullptr; }

    /**
     * construct with container iterator of pair<Key, Value>
     *
     * complexity: guaranteed O(N*lg(N))
     */
    template<typename Iterator>
    PriorityQueue(Iterator begin, Iterator end) : impl{new Impl{begin, end}} {}

    ~PriorityQueue() { delete impl; };

    /**
     * return false if queue is invalidated
     * attempt to call any methods of an invalidated queue will result in seg-fault
     * a queue is invalidated after move constructor & operator
     *
     * complexity: O(1)
     */
    explicit operator bool() const { return impl != nullptr; }

    /**
     * copy operator
     *
     * complexity: O(N)
     */
    PriorityQueue &operator=(const PriorityQueue &copy) {
        if (this == &copy) return *this;
        delete impl;
        impl = new Impl{*copy.impl};
        return *this;
    }

    /**
     * move operator
     * after this operation, 'that' will be invalidated
     * further attempt to call methods of 'that' will result in seg-fault
     *
     * complexity: O(1)
     */
    PriorityQueue &operator=(PriorityQueue &&that) noexcept {
        if (this == &that) return *this;
        delete impl;
        impl = that.impl;
        that.impl = nullptr;
        return *this;
    }

    /**
     * comparison of queues not allowed (due to different implementations)
     */
    bool operator==(const PriorityQueue<K, V, C> &that) = delete;

    /**
     * access the top element in the queue
     * undefined behavior if queue is empty
     *
     * complexity: O(1)
     */
    const std::pair<K, V> &Top() const { return impl->Top(); }

    /**
     * remove the top element
     * undefined behavior if queue is empty
     *
     * complexity: guaranteed O(lg(N))
     */
    void Pop() { impl->Pop(); }

    /**
     * complexity: O(1)
     */
    size_t Size() const { return impl->Size(); }

    /**
     * complexity: O(1)
     */
    bool Empty() const { return impl->Empty(); }

    /**
     * If key already exists in the queue, update its value
     * Otherwise, insert key, value
     *
     * return whether the key was in the queue
     *
     * complexity: O(lg(N))
     */
    bool InsertOrUpdate(std::pair<K, V> pair) {
        return impl->InsertOrUpdate(std::move(pair));
    }

    /**
     * remove key and its associated value in the queue
     *
     * return whether the key was in the queue
     *
     * complexity: O(lg(N))
     */
    bool Erase(const K &key) { return impl->Erase(key); }

    /**
     * return whether the queue contains the key
     *
     * complexity: O(lg(N))
     */
    bool Contain(const K &key) const { return impl->Contain(key); }

    /**
     * Peek at priority value of the given key
     * throws exception if key not found
     *
     * complexity: O(lg(N))
     */
    const V &Peek(const K &key) const { return impl->Peek(key); }

private:
    Impl *impl;
};

#endif //PQUEUE_PRIORITYQUEUE_H
