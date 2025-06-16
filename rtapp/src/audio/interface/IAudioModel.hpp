#ifndef IAUDIOMODEL_HPP
#define IAUDIOMODEL_HPP
class IAudioModel : public virtual IWorker, public virtual IAudioDataConsumer {
public:
    ~IAudioModel() override = default;
};

#endif // IAUDIOMODEL_HPP
