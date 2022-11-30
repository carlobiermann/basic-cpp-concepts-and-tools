/// All code derived from The Cherno's benchmarking video:
/// https://www.youtube.com/watch?v=xlAH4dbMVnU
/// As well as his Basic Intrumentation Profiler code:
/// https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e
///
/// Further edits are based on comments under The Cherno's github gist from:
/// User "davechurchill": https://pastebin.com/qw5Neq4U
/// User "maronsson": https://gist.github.com/maronsson/073840bf94e4d6df94c5f294a6e96e03


/// In case it happens that two functions start at exaclty the same time you can also add
/// usleep(1) so the visualization tool doesn't mess up the display
/// Source: https://gist.github.com/maronsson/073840bf94e4d6df94c5f294a6e96e03?permalink_comment_id=3288526#gistcomment-3288526


#include <iostream>
#include <unistd.h>
#include <profiler.hpp>


void a () {
    PROFILE_FUNCTION();
    usleep(50000);
}

void b() {
    PROFILE_FUNCTION();
    usleep(50000);
}

void f1() {
    PROFILE_FUNCTION();
    usleep(200000);
    a();
}

void f2() {
    PROFILE_FUNCTION();
    usleep(200000);
    b();
}

void f3() {
    PROFILE_FUNCTION();
    usleep(200000);
    a();
    b();
}


int main() {
    cb::benchmarking::Profiler::Instance().begin_session("profiler-test");

    std::vector<std::thread> workers;

    workers.push_back(std::thread(f1));
    workers.push_back(std::thread(f2));
    workers.push_back(std::thread(f3));

    for (auto& w : workers) {
        w.join();
    }

    cb::benchmarking::Profiler::Instance().end_session();
    return 0;
}
