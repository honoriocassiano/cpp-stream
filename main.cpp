// Project libraries
#include "stream.h"

// Standard libraries
#include <iostream>
#include <cstdio>

// Main function
int main(void) {

    std::vector<int> vec{1, 2, 3};

    auto stream = stream::of(vec).map([](int value) -> float { return value * 1.2f; });

    auto values = stream.collect(stream::VectorCollector<float>());

    // auto stream2 = stream.filter([] (int n) -> bool { return n > 2; });
    // auto stream2 = stream.map([] (int value) -> float { return float(value); });
    for (auto value : values) {
        std::cout << value << " ";
    }

    std::cout << std::endl;

    return 0;
}
