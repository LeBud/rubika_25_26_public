#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include "Entity.h"
#include "GameMgr.h"
#include "Profiler.h"

#include "Logger.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "TextureMgr.h"
#include "TransformComponent.h"

unsigned long long uFrameCount = 0;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);

    /*Logger::Info("This is an info message. That is useful");
    Logger::Warning("This is a warning message. Should I worry?");
    Logger::Error("This is an error message. Oh sh*t!!");*/

    //Singleton
    /*Globals* Instance = Globals::GetInstance();
    Instance->Init();
    Instance->Print();*/

    Globals::GetInstance()->Init();

    //Generate a blank texture to be able te create a sprite before retrieving the texture we want
    sf::Image img({1,1}, sf::Color::Magenta);
    sf::Texture blankText;
    blankText.loadFromImage(img);
    
    //Sprite that we will use
    sf::Sprite paSprite(blankText);
    sf::Sprite rayanSprite(blankText);
    
    //Load texture from ressources folder
    //paSprite.setTexture(Globals::GetInstance()->GetTextureMgr()->GetTextureData("sample4k").Texture, true);

    //Ajout d'entité et de component a l'entité
    Entity* testEntity = new Entity();
    testEntity->AddComponent<TransformComponent>();
    testEntity->AddComponent<SpriteComponent>();
    testEntity->GetComponent<TransformComponent>()->SetPosition(sf::Vector2f{200,200});
    
    if (Globals::GetInstance()->GetTextureMgr()->LoadTexture("../Ressources/IsaacSprite.png")) {
        SpriteComponent* sprite = testEntity->GetComponent<SpriteComponent>();
        sprite->SetTexture("../Ressources/IsaacSprite.png");
        sprite->SetAnimation("Body_Vertical");
        sprite = nullptr;
    }
    
    Globals::GetInstance()->GetGameMgr()->AddEntity(testEntity);

    
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

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
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

        Globals::GetInstance()->GetGameMgr()->Update(fDeltaTimeS);
        Globals::GetInstance()->GetGameMgr()->Draw(window);

        ImGui::SFML::Render(window);
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
        ++uFrameCount;
    }

    ImGui::SFML::Shutdown();

    return 0;
}