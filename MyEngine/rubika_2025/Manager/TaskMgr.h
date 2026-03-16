#pragma once
#include <functional>
#include <thread>

class TaskMgr {
public:
    void Init();
    void Shut();

    enum class ePhase {
        None,
        Worker,
        Update,
        Draw,
    };

    void RegisterTask(std::function<void()> task, ePhase phase);
    void StartPhase(ePhase phase);
    void WaitPhase();

    const int WorkerCount = 4;
    std::vector<std::thread> workers;

    void WorkerThreadUpdate();
    
private:
    ePhase CurrentPhase;
};
