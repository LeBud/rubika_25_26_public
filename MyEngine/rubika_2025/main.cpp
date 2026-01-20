#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include "Debugs.h"
#include "DrunkardWalk.h"
#include "GameMgr.h"
#include "Profiler.h"

#include "Globals.h"

unsigned long long uFrameCount = 0;

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

    Globals::GetInstance()->GetRandomMgr()->Init();
    Globals::GetInstance()->GetRandomMgr()->CreateInstance();

    DrunkardWalk* drunk = new DrunkardWalk(256,256,5,25,1);
    if (drunk->Generate()) {
        std::cout << "Generate drunkard walk!" << std::endl;
    }
    
    sf::Clock clock;
    clock.restart();

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
        
#ifdef USE_IMGUI
        ImGui::SFML::Update(window, imGuiTime);
#endif

        // sample
        //ImGui::ShowDemoWindow();
        //Logger::DrawLogger();
        
#ifdef USE_IMGUI
        Debugs::DrawDebugWindow();
#endif
        
        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();

        //========================= Update du GameMgr =========================
        window.draw(drunk->Sprite());
        Globals::GetInstance()->GetGameMgr()->Update(fDeltaTimeS);
        Globals::GetInstance()->GetGameMgr()->Draw(window);
        
#ifdef USE_IMGUI
        ImGui::SFML::Render(window);
#endif
        
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
        ++uFrameCount;
    }

#ifdef USE_IMGUI
    ImGui::SFML::Shutdown();
#endif

    return 0;
}