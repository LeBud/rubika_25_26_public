#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#include "Profiler.h"

unsigned long long uFrameCount = 0;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);
    
    ImGui::SFML::Init(window);

    sf::Texture T_PA_Texture;
    sf::Texture T_Rayan_Texture;

    //Load texture from ressources folder
    if (!T_PA_Texture.loadFromFile("../Ressources/Sprite/CHALARON_PA_GP4.jpg"))
        return EXIT_FAILURE;
    
    if (!T_Rayan_Texture.loadFromFile("../Ressources/Sprite/EL_KOTOB_Ryan_GP4.jpg"))
        return EXIT_FAILURE;

    sf::Sprite paSprite(T_PA_Texture);
    sf::Sprite rayanSprite(T_Rayan_Texture);
    
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
        ImGui::ShowDemoWindow();

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();

        //Draw sprite
        paSprite.setScale(720 / paSprite.getLocalBounds().width,480 / paSprite.getLocalBounds().height);
        window.draw(paSprite);
        
        paSprite.setScale(720 / paSprite.getLocalBounds().width / 4,480 / paSprite.getLocalBounds().height / 4);
        window.draw(paSprite);

        rayanSprite.setScale(720 / rayanSprite.getLocalBounds().width / 4,480 / rayanSprite.getLocalBounds().height / 4);
        rayanSprite.setPosition(500, 0);
        window.draw(rayanSprite);

        ImGui::SFML::Render(window);
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
        ++uFrameCount;
    }

    ImGui::SFML::Shutdown();

    return 0;
}