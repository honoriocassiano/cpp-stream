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
}

// Utility classes

int main(void) {

    std::vector<int> vec{1, 2, 3};

    auto stream = stream::of(vec);

    auto stream2 = stream.map([] (int value) -> float { return float(value); });
    
    return 0;
}
