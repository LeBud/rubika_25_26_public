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
            std::unique_lock lock(updateQueue_mutex);
            updateSyncTasks.push(task);
        }
        
        ++UpdateActiveTasks;
        
        updateMutex_condition.notify_one();
    }
    else if (phase == ePhase::Draw) {
        {
            std::unique_lock lock(drawQueue_mutex);
            drawSyncTasks.push(task);
        }
        
        ++DrawActiveTasks;
        
        drawMutex_condition.notify_one();
    }
}

void TaskMgr::StartPhase(ePhase phase) {
    CurrentPhase = phase;

    if (phase == ePhase::Update) {
        updateMutex_condition.notify_all();
    }
    else if (phase == ePhase::Draw) {
        drawMutex_condition.notify_all();
    }
    
    for (std::thread &sync : syncThreads) {
        sync.join();
    }
}

void TaskMgr::WaitPhase() {
    //Utiliser le condition variable pour savoir si j'ai encore des tâches a accomplir dans une Queue
    //Savoir si je passe à la suite, où si je le sleep en attendant que ma queue se termine

    //wait_condition.wait()
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
        if (CurrentPhase == ePhase::Update && UpdateActiveTasks > 0) {
            {
                std::unique_lock lock(updateTask_mutex);
                updateMutex_condition.wait(lock, [this]() { return !updateSyncTasks.empty() || ExitApp; });
            }
            
            {
                std::unique_lock lock(updateQueue_mutex);
                task = updateSyncTasks.front();
                updateSyncTasks.pop();
            }
            
            task();
            --UpdateActiveTasks;
        }
        else if (CurrentPhase == ePhase::Draw && DrawActiveTasks > 0) {
            {
                std::unique_lock lock(drawTask_mutex);
                drawMutex_condition.wait(lock, [this]() { return !drawSyncTasks.empty() || ExitApp; });
            }
            
            {
                std::unique_lock lock(drawQueue_mutex);
                task = drawSyncTasks.front();
                drawSyncTasks.pop();
            }
            
            task();
            --DrawActiveTasks;
        }
    }
}

