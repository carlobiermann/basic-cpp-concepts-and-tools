/// All code derived from The Cherno's benchmarking video
/// https://www.youtube.com/watch?v=YG4jexlSAjc

#include <array>
#include <chrono>
#include <iostream>
#include <memory>


/// A scope based timer
class Timer {
public:
    Timer() {
        m_start_timepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        stop();
    }

    void stop() {
        auto end_timepoint = std::chrono::high_resolution_clock::now();

        auto start =
                std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;

        std::cout << duration << "us (" << ms << "ms)\n";
    }


private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timepoint;
};

int main() {
    int value = 0;
    {
        Timer timer;
        for (size_t i = 0; i < 1000000; i++) {
            value += 2;
        }
    }
    std::cout << value << std::endl;

    /// Profiling make_shared and make_unique in Release and Debug
    /// It's important to profile in Release mode
    struct Vector2 {
        float x, y;
    };

    std::cout << "Make shared" << std::endl;
    {
        std::array<std::shared_ptr<Vector2>, 1000> shared_ptrs;
        Timer timer;
        for (size_t i = 0; i < shared_ptrs.size(); i++) {
            shared_ptrs[i] = std::make_shared<Vector2>();
        }
    }

    std::cout << "Make new" << std::endl;
    {
        std::array<std::shared_ptr<Vector2>, 1000> shared_ptrs;
        Timer timer;
        for (size_t i = 0; i < shared_ptrs.size(); i++) {
            shared_ptrs[i] = std::shared_ptr<Vector2>(new Vector2());
        }
    }

    std::cout << "Make unique" << std::endl;
    {
        std::array<std::unique_ptr<Vector2>, 1000> unique_ptrs;
        Timer timer;
        for (size_t i = 0; i < unique_ptrs.size(); i++) {
            unique_ptrs[i] = std::make_unique<Vector2>();
        }
    }

    return 0;
}