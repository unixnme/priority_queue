#ifndef PRIORITY_QUEUE_PRIORITYQUEUEIMPL_H
#define PRIORITY_QUEUE_PRIORITYQUEUEIMPL_H

#include <queue>
#include <map>
#include <set>

/**
 * Priority Queue interface that every implementation should adhere to
 */
template<typename Key, typename Value, typename Compare>
class PriorityQueueItf {
public:
    using K = Key;
    using V = Value;

    virtual ~PriorityQueueItf() = default;

    virtual const std::pair<K, V> &Top() const = 0;

    virtual void Pop() = 0;

    virtual size_t Size() const = 0;

    virtual bool Empty() const = 0;

    virtual bool InsertOrUpdate(std::pair<K, V> pair) = 0;

    virtual bool Erase(const K &key) = 0;

    virtual bool Contain(const K &key) const = 0;

    virtual const V &Peek(const K &key) const = 0;

protected:
    struct Pair {
        explicit Pair(std::pair<K, V> pair) : x{std::move(pair)} {}
        explicit Pair(std::pair<const K, V> pair) : x{{pair.fist, pair.second}} {}
        std::pair<K, V> x;
        bool operator<(const Pair &that) const {
            return Compare{}(x.second, that.x.second) ||
                    (!Compare{}(that.x.second, x.second) && x.first < that.x.first);
        }
        bool operator>(const Pair &that) const {
            return Compare{}(that.x.second, x.second) ||
                   (!Compare{}(x.second, that.x.second) && x.first > that.x.first);
        }
    };
};

template<typename Key, typename Value, typename Compare>
class PriorityQueueImpl1 : public PriorityQueueItf<Key, Value, Compare> {
public:
    using K = Key;
    using V = Value;
    using C = Compare;

    PriorityQueueImpl1() = default;
    PriorityQueueImpl1(const PriorityQueueImpl1<K, V, C> &copy) = default;
    PriorityQueueImpl1(PriorityQueueImpl1<K, V, C> &&that) noexcept = default;

    /**
     * Complexity: O(N*lg(N))
     */
    template<typename Iterator>
    explicit PriorityQueueImpl1(Iterator begin, Iterator end)
            : queue{begin, end} {
        for (auto it = begin; it != end; ++it) {
            valid.emplace(*it);
        }
    }

    ~PriorityQueueImpl1() override = default;

    /**
     * Complexity: O(1)
     */
    const std::pair<K, V> &Top() const override { return queue.top().x; }

    /**
     * Complexity: Amortized O(1)
     */
    void Pop() override {
        valid.erase(queue.top().x.first);
        queue.pop();
        PopTillValid();
    }

    size_t Size() const override { return queue.size(); }

    bool Empty() const override { return queue.empty(); }

    /**
     * Complexity: O(lg(N))
     */
    bool InsertOrUpdate(std::pair<K, V> pair) override {
        bool found;
        auto it = valid.find(pair.first);
        if (it == valid.end()) {
            found = false;
            valid.emplace(pair);
        }
        else {
            found = true;
            it->second = pair.second;
        }
        queue.emplace(std::move(pair));
        PopTillValid();
        return found;
    }

    /**
     * Complexity: O(lg(N))
     */
    bool Erase(const K &key) override {
        auto it = valid.find(key);
        if (it == valid.end())
            return false;

        valid.erase(it);
        PopTillValid();
        return true;
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

    using Pair = typename PriorityQueueItf<K, V, C>::Pair;
    std::priority_queue<Pair> queue;
    std::map<K, V> valid;
};

template<typename Key, typename Value, typename Compare>
class PriorityQueueImpl2 : public PriorityQueueItf<Key, Value, Compare> {
public:
    using K = Key;
    using V = Value;
    using C = Compare;

    PriorityQueueImpl2() = default;
    PriorityQueueImpl2(const PriorityQueueImpl2<K, V, C> &copy) = default;
    PriorityQueueImpl2(PriorityQueueImpl2<K, V, C> &&that) noexcept = default;

    template<typename Iterator>
    explicit PriorityQueueImpl2(Iterator begin, Iterator end) {
        for (auto it = begin; it != end; ++it) {
            set.emplace(*it);
            valid.insert(*it);
        }
    }

    ~PriorityQueueImpl2() override = default;

    /**
     * Complexity: O(1)
     */
    const std::pair<K, V> &Top() const override {
        return set.begin()->x;
    }

    /**
     * Complexity: O(lg(N))
     */
    void Pop() override {
        valid.erase(set.begin()->x.first);
        set.erase(set.begin());
    }

    size_t Size() const override { return set.size(); }

    bool Empty() const override { return set.empty(); }

    /**
     * Complexity: O(lg(N))
     */
    bool InsertOrUpdate(std::pair<K, V> pair) override {
        bool found;
        auto it = valid.find(pair.first);
        if (it == valid.end()) {
            found = false;
            valid.insert(pair);
        } else {
            found = true;
            set.erase(Pair{*it});
            it->second = pair.second;
        }
        set.emplace(std::move(pair));
        return found;
    }

    /**
     * Complexity: O(lg(N))
     */
    bool Erase(const K &key) override {
        auto it = valid.find(key);
        if (it == valid.end()) return false;

        set.erase(Pair{*it});
        valid.erase(it);
        return true;
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
    using Pair = typename PriorityQueueItf<K, K, C>::Pair;
//    std::set<Pair, std::greater<Pair>> set;
    std::set<Pair> set;
    std::map<K, V> valid;
};

#endif //PRIORITY_QUEUE_PRIORITYQUEUEIMPL_H
