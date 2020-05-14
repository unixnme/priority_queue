#ifndef PRIORITY_QUEUE_PRIORITYQUEUEIMPL_H
#define PRIORITY_QUEUE_PRIORITYQUEUEIMPL_H

#include <queue>
#include <map>

template<typename Key, typename Value, typename Compare>
class PriorityQueueImpl {
public:
    using K = Key;
    using V = Value;

    virtual ~PriorityQueueImpl() = default;

//    virtual PriorityQueueImpl& operator=(const PriorityQueueImpl &copy) = 0;
//    virtual PriorityQueueImpl& operator=(PriorityQueueImpl &&copy) noexcept = 0;

    virtual const std::pair<K, V> &Top() const = 0;

    virtual void Pop() = 0;

    virtual size_t Size() const = 0;

    virtual bool Empty() const = 0;

    virtual void InsertOrUpdate(std::pair<K, V> pair) = 0;

    virtual void Erase(const K &key) = 0;

    virtual bool Contain(const K &key) const = 0;

    virtual const V &Peek(const K &key) const = 0;

protected:
    struct Pair {
        explicit Pair(std::pair<K, V> pair) : x{std::move(pair)} {}
        std::pair<K, V> x;
        bool operator<(const Pair &that) const {
            return Compare{}(x.second, that.x.second) ||
                    (!Compare{}(that.x.second, x.second) && x.first < that.x.first);
        }
    };

    inline void Assert(bool cond, const std::string &msg = std::string{}) const {
        if (!(cond)) throw std::runtime_error(msg);
    }
};

template<typename Key, typename Value, typename Compare>
class PriorityQueueImpl1 : public PriorityQueueImpl<Key, Value, Compare> {
public:
    using K = Key;
    using V = Value;
    using C = Compare;

    PriorityQueueImpl1() = default;
    PriorityQueueImpl1(const PriorityQueueImpl1<K, V, C> &copy) = default;
    PriorityQueueImpl1(PriorityQueueImpl1<K, V, C> &&that) = default;

    template<typename Iterator>
    explicit PriorityQueueImpl1(Iterator begin, Iterator end)
            : queue{begin, end} {
        for (auto it = begin; it != end; ++it) {
            valid.emplace(*it);
        }
    }

    ~PriorityQueueImpl1() override {};

    /**
     * Complexity: O(1)
     */
    const std::pair<K, V> &Top() const override {
        PriorityQueueImpl<K, V, C>::Assert(!Empty(), "PriorityQueueImpl1::Top");
        return queue.top().x;
    }

    /**
     * Complexity: Amortized O(1)
     */
    void Pop() override {
        if (Empty()) return;
        valid.erase(queue.top().x.first);
        queue.pop();
        PopTillValid();
    }

    size_t Size() const override { return queue.size(); }

    bool Empty() const override { return queue.empty(); }

    /**
     * Complexity: O(lg(N))
     */
    void InsertOrUpdate(std::pair<K, V> pair) override {
        auto it = valid.find(pair.first);
        if (it == valid.end())
            valid.emplace(pair);
        else
            it->second = pair.second;
        queue.emplace(std::move(pair));
        PopTillValid();
    }

    /**
     * Complexity: O(lg(N))
     */
    void Erase(const K &key) override {
        auto it = valid.find(key);
        if (it == valid.end()) return;

        valid.erase(it);
        PopTillValid();
    }

    /**
     * Complexity: O(lg(N))
     */
    bool Contain(const K &key) const override {
        return valid.find(key) != valid.end();
    }

    /**
     * Complexity: O(lg(N))
     */
    const V &Peek(const K &key) const override { return valid.at(key); }

private:
    /**
     * Complexity: Amortized O(1)
     */
    void PopTillValid() {
        while (!queue.empty()) {
            auto it = valid.find(queue.top().x.first);
            if (it == valid.end() || it->second != queue.top().x.second)
                // this is a spurious element
                queue.pop();
            else break;
        }
    }

    using Pair = typename PriorityQueueImpl<K, V, C>::Pair;
    std::priority_queue<Pair> queue;
    std::map<K, V> valid;
};

#endif //PRIORITY_QUEUE_PRIORITYQUEUEIMPL_H
