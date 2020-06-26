#include <vector>
#include <type_traits>
#include <algorithm>
#include <functional>

namespace stream {

// Stream class
template <typename Type>
class Stream {

public:
    Stream(const std::vector<Type>& values) : values(values) {}

    virtual ~Stream() {}

    template <typename Mapper>
    auto map(Mapper &&mapper) {

        typedef typename std::result_of<Mapper(Type)>::type NewType;

        std::vector<NewType> newValues;

        newValues.reserve(values.size());

        for (auto it = values.begin(); it != values.end(); it++) {
            newValues.push_back(mapper(*it));
        }

        return Stream<NewType>(newValues);
    }

    template <typename Filter, typename = std::enable_if<std::is_same<bool, typename std::invoke_result<Filter, Type>::type>::value>>
    auto filter(Filter &&func) {

        std::vector<Type> newValues;

        for (auto it = values.begin(); it != values.end(); it++) {
            if (func(*it)) {
                newValues.push_back(*it);
            }
        }

        return Stream<Type>(newValues);
    }

    std::size_t count() const {
        return values.size();
    }

    template <typename Collector>
    auto collect(Collector &&collector) {

        for (auto it = values.begin(); it != values.end(); it++) {
            collector << *it;
        }

        return *collector;
    }

private:
    std::vector<Type> values;
};

// Utility functions
template <typename T, typename = std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>
auto of(T iterable) {

    typedef typename std::remove_reference<decltype(*std::begin(iterable))>::type Type;

    std::vector<Type> initializer;

    initializer.insert(initializer.end(), std::begin(iterable), std::end(iterable));

    return Stream<Type>(initializer);
}

// Utility classes
template <typename ValueType>
class VectorCollector {

public:
    VectorCollector<ValueType>& operator<<(const ValueType& value) {
        values.push_back(value);

        return *this;
    }

    std::vector<ValueType> operator*() {
        return values;
    }

private:
    std::vector<ValueType> values;
};

}
