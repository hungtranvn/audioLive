#ifndef IAUDIODATACONSUMER_HPP
#define IAUDIODATACONSUMER_HPP
#include "IAudioDataExchange"
class IAudioDataConsumer : public virtual IWorker, public virtual IAudioDataExchange {
    ~IAudioDataConsumer() override = default;
};

#endif // IAUDIODATACONSUMER_HPP 
