#pragma once
#include <condition_variable>
#include <functional>
#include <queue>
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

private:
    void WorkerThreadUpdate();
    void SyncThreadUpdate();
    
    ePhase CurrentPhase;
    bool ExitApp = false;

    //Parallel Workers
    const int WorkerCount = 4;
    
    std::vector<std::thread> workers;
    
    std::queue<std::function<void()>> workerTasks;

    std::condition_variable workerMutex_condition;
    std::mutex workerQueue_mutex;
    std::mutex workerTask_mutex;

    std::atomic<int> WorkerActiveTasks;

    //Sync Workers
    const int SyncCount = 4;
    
    std::vector<std::thread> syncThreads;

    std::queue<std::function<void()>> updateSyncTasks;
    std::queue<std::function<void()>> drawSyncTasks;

    //Update Queue
    std::condition_variable syncMutex_condition;
    std::mutex syncQueue_mutex;
    std::mutex syncTask_mutex;
    
    std::atomic<int> UpdateActiveTasks;
    std::atomic<int> DrawActiveTasks;

    //Draw Queue
    // std::condition_variable drawMutex_condition;
    // std::mutex drawQueue_mutex;
    // std::mutex drawTask_mutex;
    // std::atomic<int> DrawActiveTasks;

    std::mutex waitMutex;
    std::condition_variable wait_condition;
};
