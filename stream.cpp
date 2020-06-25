#include <iostream>
#include <cstdio>
#include <vector>
#include <type_traits>
#include <algorithm>

namespace stream {
// Stream class
template <typename Value>
class Stream {

public:
    Stream(std::vector<Value> values) : values(values) {}

    ~Stream() {}

private:
    std::vector<Value> values;
};

// Utility templates
template <typename T, typename = std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>
auto of(T iterable) {

    typedef typename std::remove_reference<decltype(*std::begin(iterable))>::type Value;

    std::vector<Value> initializer;

    initializer.insert(initializer.end(), std::begin(iterable), std::end(iterable));

    return Stream<Value>(initializer);
}
}

// Utility classes

int main(void) {

    std::vector<int> vec{1, 2, 3};

    auto stream = stream::of(vec);
    
    return 0;
}
