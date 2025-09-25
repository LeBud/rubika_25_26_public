#ifdef USE_IMGUI

#include "Debugs.h"

#include <ImGui/imgui.h>

#include "Globals.h"

void Debugs::DrawDebugWindow()
{
	GetInstance()->DrawDebug();
}

Logger& Debugs::GetLogger()
{
	return GetInstance()->Logger;
}

TextureMgr& Debugs::GetTextureMgr() {
	return *Globals::GetInstance()->GetTextureMgr();
}

Debugs* Debugs::GetInstance()
{
	if (!Instance){
		Instance = new Debugs();
	}

	return Instance;
}

void Debugs::DeleteInstance()
{
	delete Instance;
	Instance = nullptr;
}

void Debugs::DrawDebug()
{
	// Draw your debug window here
	if (ImGui::Begin("Console")) {
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			//Draw the default logger
			if (ImGui::BeginTabItem("Logger"))
			{
				Logger::DrawLogger(); 
				ImGui::EndTabItem();
			}
			//Draw the textureMgr debugger
			if (ImGui::BeginTabItem("TextureMgr"))
			{
				auto tex = Globals::GetInstance()->GetTextureMgr(); //Get texturesMap
				
				//ImGui::Text("This is the textureMgr Tab!");
				
				/*ImGui::Text("Textures Loaded : %d", tex->GetTextureMap().size());
				for (auto t : tex->GetTextureMap()) {
					ImGui::Text("Texture : %s", t.first.c_str());	
				}*/

				if (ImGui::BeginTable("Logs##TextureMgr", 8, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders)) {
					ImGui::TableSetupColumn("Texture"); //Nom - Chemin de la texture
					ImGui::TableSetupColumn("Animations Data"); //Combien il y a d'animation pour ce sprite
					ImGui::TableSetupColumn("Start X,Y"); //la position des sprites
					ImGui::TableSetupColumn("Size X, Y"); //la taille des sprites
					ImGui::TableSetupColumn("Offset X, Y"); //offset entre chaque sprites
					ImGui::TableSetupColumn("Frames"); //Combien il y a de sprites dans l'animation
					ImGui::TableSetupColumn("Time Between Frames"); //le temps entre chaque animations
					ImGui::TableSetupColumn("Inverted Animation"); //Animation joué a l'envers ou non
					ImGui::TableHeadersRow();
					ImGui::TableNextRow();

					for (auto t : tex->GetTextureMap())
					{

						auto animations = t.second.AnimationData;
						for (auto a : animations) {
							ImVec4 color = ImVec4(1, 1, 1, 1);
							color = ImVec4(1, 1, 1, 1);
						

							ImGui::TableSetColumnIndex(0);
							ImGui::TextColored(color, t.first.c_str());
							ImGui::TableSetColumnIndex(1);
							ImGui::Text("Animations :%s", a.first.c_str());
							ImGui::TableSetColumnIndex(2);
							ImGui::Text("X :%d", a.second.StartX);
							ImGui::Text("Y :%d", a.second.StartY);
							ImGui::TableSetColumnIndex(3);
							ImGui::Text("X :%d", a.second.SizeX);
							ImGui::Text("Y :%d", a.second.SizeY);
							ImGui::TableSetColumnIndex(4);
							ImGui::Text("X :%d", a.second.OffsetX);
							ImGui::Text("Y :%d", a.second.OffsetY);
							ImGui::TableSetColumnIndex(5);
							ImGui::Text("Frames :%d", a.second.AnimationSpriteCount);
							ImGui::TableSetColumnIndex(6);
							if (a.second.TimeBetweenAnimationInS > 0)
								ImGui::Text("Time between frame :%.3f", a.second.TimeBetweenAnimationInS);
							else
								ImGui::Text("Time between frame :0");
							ImGui::TableSetColumnIndex(7);
							ImGui::Text(a.second.IsReverted ? "Inverted Animation : True" : "Inverted Animation : False");
							ImGui::TableNextRow();
						}
					}

					ImGui::EndTable();
				}
				
				
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

#endif