/// This is only a basic loop example in order to practice debugging

#include <iostream>
#include <vector>

void looper(const std::vector<int>& arr) {
    for (const auto& element : arr) {
        std::cout << element << std::endl;
    }
}

int main() {
    looper({1, 2, 4, 7, 7, 9});
    return 0;
}