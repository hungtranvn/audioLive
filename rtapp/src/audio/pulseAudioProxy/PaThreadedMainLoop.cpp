#include "PaThreadedMainloop.hpp"

PaThreadedMainloop::PaThreadedMainloop() : m_mainloop(pa_threaded_mainloop_new()_)
{
    if (m_mainloop == nullptr) {
        throw std::bad_alloc();
    }
}

PaThreadMainloop::~PaThreadMainloop()
{
    if (m_mainloop != nullptr) {
        pa_threaded_mainloop_free(m_mainloop);
    }
}

auto PaThreadedMainloop::start() -> bool
{
    return pa_threaded_mainloop_start(m_mainloop) == 0;
}

void PaThreadedMainloop::stop()
{
    return pa_threaded_mainloop_stop(m_mainloop);
}

void PaThreadMainloop::lock()
{
    return pa_threaded_mainloop_lock(m_mainloop);
}

void PaThreadMainloop::unlock()
{
    return pa_threaded_mainloop_unlock(m_mainloop);
}

void PaThreadedMainloop::wait() const
{
    return pa_threaded_mainloop_wait(m_mainloop);
}

void PaThreadedMainloop::getApi() -> pa_mainloop_api*
{
    return pa_threaded_mainloop_get_api(m_mainloop);
}
