#include <iostream>
#include <vector>

#include "profiler.hpp"


/// An example of Perfect Forwarding
namespace std {
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}// namespace std

namespace examples {
/// What std::forward looks like
template<typename T>
T&& forward(std::remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);/// Relying on reference collapsing
}

/// What std::move looks like
template<typename T>
std::remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}
}// namespace examples

class Widget {
private:
    int m_i{0};
    std::string m_s{};
    int* m_pi{nullptr};

public:
    Widget(int i, std::string s, int* pi) : m_i(i), m_s(s), m_pi(pi) {
    }

    // Copy constructor
    /// Using const qualifier was necessary here
    Widget(const Widget& w) : m_i(w.m_i), m_s(w.m_s), m_pi(w.m_pi) {
        std::cout << "Copy constructor is being called" << std::endl;
    }

    // Move constructor
    /// Using noexcept achieved a significant speedup
    Widget(Widget&& w) noexcept : m_i(std::move(w.m_i)), m_s(std::move(w.m_s)), m_pi(std::move(w.m_pi)) {
        w.m_pi = nullptr;
        std::cout << "Move constructor is being called" << std::endl;
    }

    // Move assignment operator
    Widget& operator=(Widget&& w) noexcept {
        delete m_pi;
        m_i = std::move(w.m_i);
        m_s = std::move(w.m_s);
        m_pi = std::move(w.m_pi);
        w.m_pi = nullptr;
        return *this;
    }
};


int main() {
    {
        std::cout << "Using copy constructors: " << std::endl;
        std::string s("Long string that needs to be copied");
        std::vector<Widget> v{};
        constexpr size_t N(10);
        cb::benchmarking::SimpleTimer timer;
        for (size_t i = 0; i < N; ++i) {
            Widget w{1, s, nullptr};// This calls the move constructor
            std::cout << "Pushing back" << std::endl;
            v.push_back(w);// This calls the copy constructor
        }
    }

    {
        std::cout << "Using move constructors: " << std::endl;
        std::string s("Long string that needs to be copied");
        std::vector<Widget> v{};
        constexpr size_t N(10);
        cb::benchmarking::SimpleTimer timer;
        for (size_t i = 0; i < N; ++i) {
            Widget w{1, s, nullptr};// This calls the move constructor
            std::cout << "Pushing back" << std::endl;
            v.push_back(std::move(w));// This calls the move constructor
        }
    }

    return 0;
}
