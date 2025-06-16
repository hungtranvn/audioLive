#ifndef PA_THREADED_MAIN_LOOP_HPP
#define PA_THREADED_MAIN_LOOP_HPP

#include <pulse/thread-mainloop.h>

class PAThreadedMainloop {
public:
    PAThreadedMainloop();
    ~PAThreadedMainloop();

    PAThreadedMainloop(const PAThreadedMainloop&) = delete;
    auto operator=(const PAThreadedMainloop&) -> PAThreadedMainloop& = delete;

    PAThreadedMainloop(PAThreadedMainloop&&) = delete;
    auto operator=(PAThreadedMainloop&&) = delete;

    auto start() -> bool;
    auto stop();
    void lock();
    void unlock();
    void wait() const;
    void signal(bool waitForAccept);
    auto getApi() -> pa_mainloop_api*;
private:
    pa_threaded_mainloop* m_mainloop;
};
#endif // PA_THREADED_MAIN_LOOP_HPP
