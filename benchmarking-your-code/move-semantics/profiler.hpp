#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>


#define PROFILING 1
#ifdef PROFILING
#define PROFILE_SCOPE(name) cb::benchmarking::ScopedTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#endif


namespace cb::benchmarking {

    struct ProfileResult {
        const std::string name;
        long long start, end;
        std::thread::id threadID;
    };

    class Profiler {

    public:

        static Profiler &Instance() {
            static Profiler instance;
            return instance;
        }

        ~Profiler() {
            end_session();
        }

        void begin_session(const std::string &name, const std::string &filepath = "results.json") {
            if (m_active_session) {
                end_session();
            }
            m_active_session = true;
            m_output_stream.open(filepath);
            write_header();
            m_session_name = name;
        }

        void end_session() {
            if (!m_active_session) {
                return;
            }
            m_active_session = false;
            write_footer();
            m_output_stream.close();
            m_profile_count = 0;
        }

        void write_profile(const ProfileResult &result) {
            /// Keep in mind that locking/unlocking the mutex
            /// affects the profiling results as well
            std::lock_guard<std::mutex> lock(m_lock);

            if (m_profile_count++ > 0) {
                m_output_stream << ",";
            }

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_output_stream << "{";
            m_output_stream << "\"cat\":\"function\",";
            m_output_stream << "\"dur\":" << (result.end - result.start) << ',';
            m_output_stream << "\"name\":\"" << name << "\",";
            m_output_stream << "\"ph\":\"X\",";
            m_output_stream << "\"pid\":0,";
            m_output_stream << "\"tid\":" << result.threadID << ",";
            m_output_stream << "\"ts\":" << result.start;
            m_output_stream << "}";
        }

        void write_header() {
            m_output_stream << "{\"otherDate\": {},\"traceEvents\":[";
        }

        void write_footer() {
            m_output_stream << "]}";
        }

    private:
        // What does this mean? Possibly related to creating a singleton
        Profiler() {}

        std::string m_session_name = "None";
        std::ofstream m_output_stream;
        unsigned int m_profile_count = 0;
        std::mutex m_lock;
        bool m_active_session = false;

    };


    class ScopedTimer {
        ProfileResult m_result;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timepoint;
        bool m_stopped;

    public:
        ScopedTimer(const std::string &name)
                : m_result({name, 0, 0, {}}), m_stopped(false) {
            m_start_timepoint = std::chrono::high_resolution_clock::now();
        }

        ~ScopedTimer() {
            if (!m_stopped) {
                stop();
            }
        }

        void stop() {
            auto end_timepoint = std::chrono::high_resolution_clock::now();
            m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>(
                    m_start_timepoint).time_since_epoch().count();
            m_result.end = std::chrono::time_point_cast<std::chrono::microseconds>(
                    end_timepoint).time_since_epoch().count();
            m_result.threadID = std::this_thread::get_id();
            Profiler::Instance().write_profile(m_result);
            m_stopped = true;
        }
    };

/// A simple scope based timer
    class SimpleTimer {
    public:
        SimpleTimer() {
            m_start_timepoint = std::chrono::high_resolution_clock::now();
        }

        ~SimpleTimer() {
            stop();
        }

    private:

        std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timepoint;

        void stop() {
            auto end_timepoint = std::chrono::high_resolution_clock::now();

            auto start = std::chrono::time_point_cast<std::chrono::microseconds>(
                    m_start_timepoint).time_since_epoch().count();
            auto end = std::chrono::time_point_cast<std::chrono::microseconds>(
                    end_timepoint).time_since_epoch().count();

            auto duration = end - start;
            double ms = duration * 0.001;

            std::cout << duration << "us (" << ms << "ms)\n";
        }
    };

} // namespace cb::benchmarking




