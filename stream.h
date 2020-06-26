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

private:
    template <typename BuilderType>
    class Builder {
        public:
            Builder() {}
            virtual ~Builder() {}

            Builder<BuilderType> operator<<(const BuilderType& type) {
                values.push_back(type);

                return *this;
            }

            Stream<BuilderType> build() {
                return Stream<BuilderType>(std::move(values));
            }

        private:
            std::vector<BuilderType> values;
    };

public:
    template <typename Mapper>
    auto map(Mapper &&mapper) {

        typedef typename std::result_of<Mapper(Type)>::type NewType;

        Builder<NewType> builder;

        for (const auto &value : values) {
            builder << values;
        }

        return builder.build();
    }

    template <typename Filter, typename = std::enable_if<std::is_same<bool, typename std::invoke_result<Filter, Type>::type>::value>>
    auto filter(Filter &&func) {

        Builder<Type> builder;

        for (const auto& value: values) {
            if (func(value)) builder << value;
        }

        return builder.build();
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
