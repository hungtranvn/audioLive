#ifndef THREAD_MAIN_LOOP_HPP
#define THREAD_MAIN_LOPP_HPP

#include <pulse/thread-mainloop.h>

namespace audio {

class ThreadMainloop {
public:
    ThreadMainloop();
    ~ThreadMainloop();


    ThreadMainloop(const ThreadMainloop&) = delete;
    auto operator(const ThreadMainloop&) -> ThreadMainloop& = delete;

    ThreadMainloop(ThreadMainloop&&) = delete;
    auto operator=(ThreadMainloop&&) -> ThreadMainloop& = delete;

    auto start() -> bool;
    auto stop() -> void;
    auto lock() -> void;
    auto unlock() -> void;
    auto wait() const -> void;
    auto signal(bool waitForAccept) -> void;
    auto getApi() -> pa_mainloop_api*;
private:
    pa_threaded_mainloop* m_mainloop;
};

} // namespace audio

#endif // THREAD_MAIN_LOOP_HPP
