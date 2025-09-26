#ifdef USE_IMGUI

#include "Debugs.h"

#include <ImGui/imgui.h>

#include "Entity.h"
#include "GameMgr.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

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
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)){
			//Draw the default logger
			if (ImGui::BeginTabItem("Logger")){
				Logger::DrawLogger(); 
				ImGui::EndTabItem();
			}
			
			//Draw the textureMgr debugger
			if (ImGui::BeginTabItem("TextureMgr")){
				DrawTextureMgr();
				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("Entity")) {
				DrawEntityDebugger();
				ImGui::EndTabItem();
			}
			
			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

//Peut-être nécessiter d'être modifier pour ajouter des PushID au for loop
void Debugs::DrawTextureMgr() {
	auto tex = Globals::GetInstance()->GetTextureMgr(); //Get texturesMap
	
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
						ImGui::Text("Entity : %s", u->entityName.c_str());
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
}

void Debugs::DrawEntityDebugger() {
	static int clicked = 0;
	
	ImGui::Text("All Entities");

	if (ImGui::Button("Create Entity")) //CreateEntity
		clicked++;
	if (clicked & 1) {
		Globals::GetInstance()->CreateBaseEntity("Debug Entity");
		clicked = 0;
	}
	
	if (ImGui::CollapsingHeader("Entities")) {
		
		//Display chaque component de l'entity
		//Pouvoir modifier dynamiquement les component (transform - sprite - etc.)
		//Position dans le transform
		//Animation, sprite dans le spriteComponent

		for (auto e : Globals::GetInstance()->GetGameMgr()->GetEntities()) {
			ImGui::PushID(e);
			if (ImGui::TreeNode("##", e->entityName.c_str())) {
				ImGui::Text("Entity : %s", e->entityName.c_str());

				for (auto c : e->GetAllComponents()) {
					if (c == e->GetComponent<SpriteComponent>()) {

						auto component = e->GetComponent<SpriteComponent>();
						
						if (ImGui::TreeNode("Sprite Component")) {

							if (ImGui::BeginCombo("Animations", component->currentAnimationName.c_str())) {
								for (auto t : component->textureData->AnimationData) {
									const bool isSelected = t.first == component->currentAnimationName;
									if (ImGui::Selectable(t.first.c_str(), isSelected)) {
										component->SetAnimation(t.first);
									}

									if (isSelected)
										ImGui::SetItemDefaultFocus();
								}

								ImGui::EndCombo();
							}
							
							ImGui::Checkbox("Pause Animation", &component->bPause);
							component->PlayAnimation(component->bPause);
							
							ImGui::Text("Current frame :%d", component->currentSpriteIndex);
							
							//Changer la texture du sprite <--------------
							
							ImGui::TreePop();
						}
					}

					if (c == e->GetComponent<TransformComponent>()) {
						if (ImGui::TreeNode("Transform Component")) {
							sf::Vector2f pos = e->GetComponent<TransformComponent>()->GetPosition();

							if (ImGui::DragFloat2("Position X/Y", &pos.x)) {
								e->GetComponent<TransformComponent>()->SetPosition(pos);
							}

							float angle = e->GetComponent<TransformComponent>()->GetRotation().asDegrees();

							if (ImGui::DragFloat("Rotation", &angle)) {
								sf::Angle newAngle = sf::degrees(angle);
								e->GetComponent<TransformComponent>()->SetRotation(newAngle);
							}

							sf::Vector2f scale = e->GetComponent<TransformComponent>()->GetScale();
							if (ImGui::DragFloat2("Scale", &scale.x)) {
								e->GetComponent<TransformComponent>()->SetScale(scale);
							}
							
							ImGui::TreePop();
						}
					}
				}
				
				if (ImGui::Button("Delete Entity"))
					clicked++;
				if (clicked & 1) {
					Globals::GetInstance()->GetGameMgr()->DestroyEntity(e);
					clicked = 0;
				}
				
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		
		ImGui::Unindent(); //at the end of collapsing header
	}
}

#endif
