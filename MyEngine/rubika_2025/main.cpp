#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include "Debug/Debugs.h"
#include "Procedural/DrunkardWalk.h"
#include "Manager/GameMgr.h"
#include "Profiler.h"

#include "Globals.h"
#include "Procedural/CellularAutomata.h"

unsigned long long uFrameCount = 0;

void PopulateUpdate() {
    for (int i = 0; i < 10; ++i)
    {
        Globals::GetInstance()->GetTaskMgr()->RegisterTask([i]()
            {
                PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Update %d", i);

                Sleep(100);

                PROFILER_EVENT_END();
            },
            TaskMgr::ePhase::Update);
    }
}

void PopulateDraw() {
    for (int i = 0; i < 20; ++i)
    {
        Globals::GetInstance()->GetTaskMgr()->RegisterTask([i]()
            {
                PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw %d", i);

                Sleep(50);

                PROFILER_EVENT_END();
            },
            TaskMgr::ePhase::Draw);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

#ifdef USE_IMGUI
    ImGui::SFML::Init(window);
#endif

    /*Logger::Info("This is an info message. That is useful");
    Logger::Warning("This is a warning message. Should I worry?");
    Logger::Error("This is an error message. Oh sh*t!!");*/

    Globals::GetInstance()->Init();

    Globals::GetInstance()->CreateBaseEntity("Hello");
    Globals::GetInstance()->CreateBaseEntity("Hola");
    Globals::GetInstance()->CreateBaseEntity("Non Non");
    
    sf::Clock clock;
    clock.restart();

    Globals::GetInstance()->GetTaskMgr()->Init();
    
    for (int i = 0; i < 100; ++i) { //Parallel task Test
        Globals::GetInstance()->GetTaskMgr()->RegisterTask([i]() {
            PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Task %d", i);

            Sleep(1000);

            PROFILER_EVENT_END();
        }, TaskMgr::ePhase::Worker);
    }
    
    while (window.isOpen())
    {
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", uFrameCount);

        int64_t deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
#ifdef USE_IMGUI
            ImGui::SFML::ProcessEvent(window, event.value());
#endif
        }

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
        //========================= Update =========================
        PopulateUpdate();
        Globals::GetInstance()->GetTaskMgr()->StartPhase(TaskMgr::ePhase::Update);
        
#ifdef USE_IMGUI
        ImGui::SFML::Update(window, imGuiTime);
#endif
        
        Globals::GetInstance()->GetGameMgr()->Update(fDeltaTimeS);
        
#ifdef USE_IMGUI
        Debugs::DrawDebugWindow();
#endif
        
        Globals::GetInstance()->GetTaskMgr()->WaitPhase();
        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        //========================= Draw =========================
        PopulateDraw();
        Globals::GetInstance()->GetTaskMgr()->StartPhase(TaskMgr::ePhase::Draw);
        
        window.clear();
        
        Globals::GetInstance()->GetGameMgr()->Draw(window);
        
#ifdef USE_IMGUI
        ImGui::SFML::Render(window);
#endif
        window.display();
        
        Globals::GetInstance()->GetTaskMgr()->WaitPhase();
        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
        ++uFrameCount;
    }

    Globals::GetInstance()->GetTaskMgr()->Shut();
    
#ifdef USE_IMGUI
    ImGui::SFML::Shutdown();
#endif

    return 0;
}