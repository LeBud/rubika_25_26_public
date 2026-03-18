#include "TaskMgr.h"

void TaskMgr::Init() {
    //Je suppose que j'init mon vector de thread ?
    for (int i = 0; i < WorkerCount; ++i) {
        workers.emplace_back(std::thread(&TaskMgr::WorkerThreadUpdate, this));
    }

    for (int i = 0; i < SyncCount; ++i) {
        syncThreads.emplace_back(std::thread(&TaskMgr::SyncThreadUpdate, this));
    }
}

void TaskMgr::Shut() {
    {
        std::unique_lock lock(workerQueue_mutex);
        ExitApp = true;
    }

    workerMutex_condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }

    workers.clear();
}

void TaskMgr::RegisterTask(std::function<void()> task, ePhase phase) {
    if (phase == ePhase::Worker) {
        {
            std::unique_lock lock(workerQueue_mutex);
            workerTasks.push(task);
        }
        
        ++WorkerActiveTasks;
        
        workerMutex_condition.notify_one();
    }
    else if (phase == ePhase::Update) {
        {
            std::unique_lock lock(syncQueue_mutex);
            updateSyncTasks.push(task);
        }
        
        ++UpdateActiveTasks;
        
        syncMutex_condition.notify_one();
    }
    else if (phase == ePhase::Draw) {
        {
            std::unique_lock lock(syncQueue_mutex);
            drawSyncTasks.push(task);
        }
        
        ++DrawActiveTasks;
        
        syncMutex_condition.notify_one();
    }
}

void TaskMgr::StartPhase(ePhase phase) {
    CurrentPhase = phase;
    syncMutex_condition.notify_all();
}

void TaskMgr::WaitPhase() {
    // if (CurrentPhase == ePhase::Update) {
    //     std::unique_lock lock(waitMutex);
    //     wait_condition.wait(lock, [this]() { return UpdateActiveTasks == 0 || ExitApp; });
    // }
    // else if (CurrentPhase == ePhase::Draw) {
    //     std::unique_lock lock(waitMutex);
    //     wait_condition.wait(lock, [this]() { return DrawActiveTasks == 0 || ExitApp; });
    // }

    std::unique_lock lock(waitMutex);
    wait_condition.wait(lock, [this]() {
        if (CurrentPhase == ePhase::Update)
            return UpdateActiveTasks == 0 || ExitApp;
        if (CurrentPhase == ePhase::Draw)
            return DrawActiveTasks == 0 || ExitApp;

        return false;
    });
}

void TaskMgr::WorkerThreadUpdate() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock(workerTask_mutex);
            workerMutex_condition.wait(lock, [this]() { return !workerTasks.empty() || ExitApp; });
        }
        
        {
            std::unique_lock lock(workerQueue_mutex);
            if (workerTasks.empty()) continue;
            
            task = workerTasks.front();
            workerTasks.pop();
        }
        
        task();
        --WorkerActiveTasks;
    }
}

void TaskMgr::SyncThreadUpdate() {
    while (true) {
        std::function<void()> task;
            {
                std::unique_lock lock(syncTask_mutex);
                syncMutex_condition.wait(lock, [this]() {
                    std::unique_lock lock(syncQueue_mutex);
                    
                    if (CurrentPhase == ePhase::Update)
                        return !updateSyncTasks.empty() || ExitApp;
                    if (CurrentPhase == ePhase::Draw)
                        return !drawSyncTasks.empty() || ExitApp;
                });
            }

        if (CurrentPhase == ePhase::Update) {
            {
                std::unique_lock lock(syncQueue_mutex);
                if (updateSyncTasks.empty()) continue;
                
                task = updateSyncTasks.front();
                updateSyncTasks.pop();
            }
            
            task();
            --UpdateActiveTasks;
        }
        else if (CurrentPhase == ePhase::Draw) {
            {
                std::unique_lock lock(syncQueue_mutex);
                if (drawSyncTasks.empty()) continue;
                
                task = drawSyncTasks.front();
                drawSyncTasks.pop();
            }

            task();
            --DrawActiveTasks;
        }

        wait_condition.notify_one();
    }
}

