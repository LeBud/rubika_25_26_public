#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include "GameMgr.h"
#include "Profiler.h"

#include "Logger.h"
#include "Globals.h"
#include "TextureMgr.h"

unsigned long long uFrameCount = 0;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);

    /*Logger::Info("This is an info message. That is useful");
    Logger::Warning("This is a warning message. Should I worry?");
    Logger::Error("This is an error message. Oh sh*t!!");*/

    //Singleton
    Globals* Instance = Globals::getInstance();
    Instance->Init();
    Instance->Print();

    //Generate a blank texture to be able te create a sprite before retrieving the texture we want
    sf::Image img({1,1}, sf::Color::Magenta);
    sf::Texture blankText;
    blankText.loadFromImage(img);
    
    //Sprite that we will use
    sf::Sprite paSprite(blankText);
    sf::Sprite rayanSprite(blankText);
    
    //Load texture from ressources folder
    if (Instance->GetTextureMgr()->LoadTexture("../Ressources/sample4k.jpg"))
        paSprite.setTexture(Instance->GetTextureMgr()->GetTextureData("sample4k").Texture, true);
    
    /*if (Instance->GetTextureMgr()->LoadTexture("../Ressources/Sprite/EL_KOTOB_Ryan_GP4.jpg"))
        rayanSprite.setTexture(Instance->GetTextureMgr()->GetTextureData("EL_KOTOB_Ryan_GP4").texture, true);*/
    
    sf::Clock clock;
    clock.restart();

    while (window.isOpen())
    {
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", uFrameCount);

        int deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            ImGui::SFML::ProcessEvent(window, event.value());
        }

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
        ImGui::SFML::Update(window, imGuiTime);

        // sample
        //ImGui::ShowDemoWindow();
        Logger::DrawLogger();

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();

        //Draw sprite
        /*paSprite.setScale(sf::Vector2f(720 / paSprite.getLocalBounds().size.x,480 / paSprite.getLocalBounds().size.y));
        window.draw(paSprite);*/
        
        /*paSprite.setScale(sf::Vector2f(720 / paSprite.getLocalBounds().size.x / 4,480 / paSprite.getLocalBounds().size.y / 4));
        window.draw(paSprite);

        rayanSprite.setScale(sf::Vector2f(720 / rayanSprite.getLocalBounds().size.x / 4,480 / rayanSprite.getLocalBounds().size.y / 4));
        rayanSprite.setPosition(sf::Vector2f(500, 0));
        window.draw(rayanSprite);*/

        Instance->gameMgr->Update(fDeltaTimeMS);
        Instance->gameMgr->Draw(window);
        

        ImGui::SFML::Render(window);
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
        ++uFrameCount;
    }

    ImGui::SFML::Shutdown();

    return 0;
}