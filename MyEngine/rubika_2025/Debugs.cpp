#ifdef USE_IMGUI

#include "Debugs.h"

#include <ImGui/imgui.h>

#include "Entity.h"
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

				/*if (ImGui::BeginTable("Logs##TextureMgr", 8, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders)) {
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
				}*/
				
				
				for (auto t : tex->GetTextureMap()) {
					if (ImGui::TreeNode(t.first.c_str())) {
						ImGui::Text("Texture width / height : %d", t.second.Texture.getSize().x);
						ImGui::SameLine();
						ImGui::Text(", %d", t.second.Texture.getSize().y);

						ImGui::Text("Texture Size : %d mb", t.second.Texture.getMaximumSize());
						ImGui::Text("Animations count : %d", t.second.AnimationData.size());
						
						//Insérer ici par qui il est utilisé
						if (ImGui::TreeNode("Used By")) {
							auto used = Globals::GetInstance()->GetTextureMgr()->GetTextureUsedByEntity(t.first);

							if (!used.empty()) {
								ImGui::Text("Used By : %d", used.size());
								
								for (auto u : used) {
									ImGui::Text("Entity : %s", u->entityName);
								}
							}
							
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Animations")) {
							for (auto a : t.second.AnimationData) {
								if (ImGui::TreeNode(a.first.c_str())) {
								
									ImGui::Text("Start X/Y : %d", a.second.StartX);
									ImGui::SameLine();
									ImGui::Text(", %d", a.second.StartY);
								
									ImGui::Text("Size X/Y : %d", a.second.SizeX);
									ImGui::SameLine();
									ImGui::Text(", %d", a.second.SizeY);
								
									ImGui::Text("Offset X/Y : %d", a.second.OffsetX);
									ImGui::SameLine();
									ImGui::Text(", %d", a.second.OffsetY);
								
									ImGui::Text("Frames : %d", a.second.AnimationSpriteCount);
								
									if (a.second.TimeBetweenAnimationInS > 0)
										ImGui::Text("Time between frame : %.3f", a.second.TimeBetweenAnimationInS);
									else
										ImGui::Text("Time between frame : 0");
								
									ImGui::Text(a.second.IsReverted ? "Inverted Animation : True" : "Inverted Animation : False");
								
									ImGui::TreePop();
								}
							}
							
							ImGui::TreePop();
						}

						ImGui::TreePop();
					}
				}
				
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

#endif