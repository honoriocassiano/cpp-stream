#include <iostream>
#include <cstdio>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <functional>

namespace stream {

// Stream class
template <typename Type>
class Stream {

public:
    Stream(std::vector<Type> values) : values(values) {}

    ~Stream() {}

    template <typename Mapper>
    auto map(Mapper mapper) {

        typedef typename std::result_of<Mapper(Type)>::type NewType;

        std::vector<NewType> newValues;

        newValues.reserve(values.size());

        for (auto it = values.begin(); it != values.end(); it++) {
            newValues.push_back(mapper(*it));
        }

        return Stream<NewType>(newValues);
    }

    auto filter(const std::function<bool(Type)>& func) {
        std::vector<Type> newValues;

        for (auto it = values.begin(); it != values.end(); it++) {
            if (func(*it)) {
                newValues.push_back(*it);
            }
        }

        return Stream<Type>(newValues);
    }

    template <typename Collector>
    auto collect(Collector collector) const {

        for (auto it = values.begin(); it != values.end(); it++) {
            collector.consume(*it);
        }

        return collector.finish();
    }

private:
    std::vector<Type> values;
};

// Utility templates
template <typename T, typename = std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>
auto of(T iterable) {

    typedef typename std::remove_reference<decltype(*std::begin(iterable))>::type Type;

    std::vector<Type> initializer;

    initializer.insert(initializer.end(), std::begin(iterable), std::end(iterable));

    return Stream<Type>(initializer);
}

// Utility classes
template <typename T>
class Collector {

    typedef decltype(*std::begin(std::declval<T>())) ValueType;

public:
    Collector() {}
    virtual ~Collector() {}

    void consume(const ValueType& value) {}

    T finish() {}
};

template <typename ValueType>
class VectorCollector : public Collector<std::vector<ValueType>> {

public:
    void consume(const ValueType& value) {
        values.push_back(value);
    }

    auto finish() {
        return values;
    }

private:
    std::vector<ValueType> values;
};

}

// Main function
int main(void) {

    std::vector<int> vec{1, 2, 3};

    auto stream = stream::of(vec).map([](int value) -> float { return value * 1.2f; });

    auto collector = stream::VectorCollector<float>();

    auto values = stream.collect(collector);

    // auto stream2 = stream.filter([] (int n) -> bool { return n > 2; });
    // auto stream2 = stream.map([] (int value) -> float { return float(value); });
    for (auto value : values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
