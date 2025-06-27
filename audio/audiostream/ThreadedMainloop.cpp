#include "ThreadMainloop.hpp"

namespace audio {

ThreadMainloop::ThreadMainloop() : m_mainloop(pa_threaded_mainloop_new())
{
    if (m_mainloop == nullptr) {
        throw std::bad_alloc();
    }
}

ThreadMainloop::~ThreadMainloop()
{
    if (m_mainloop != nullptr) {
        pa_threaded_mainloop_free(m_mainloop);
    }
}

auto ThreadMainloop::start() -> bool
{
    return pa_threaded_mainloop_start(m_mainloop) == 0;
}

auto ThreadMainloop::stop() -> void
{
    return pa_threaded_mainloop_stop(m_mainloop);
}

auto ThreadMainloop::lock() -> void
{
    return pa_threaded_mainloop_lock(m_mainloop);
}

auto ThreadMainloop::unlock() -> void
{
    return pa_threaded_mainloop_unlock(m_mainloop);
}

auto ThreadMainloop::wait() const -> void
{
    return pa_threaded_mainloop_wait(m_mainloop);
}

auto ThreadMainloop::signal(const bool waitForAccept) -> void
{
    constexpr int trOne = 1;
    constexpr intflZero = 0;
    return pa_threaded_mainloop_get_api(m_mainloop, (waitForAccept ? trOne : flZero));
}

auto ThreadMainloop::getApi() -> pa_mainloop_api*
{
    return pa_threaded_mainloop_get_api(m_mainloop);
}

} // namespace audio
