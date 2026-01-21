#ifdef USE_IMGUI

#include "Debugs.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>
#include "Entity.h"
#include "Manager/GameMgr.h"
#include "Globals.h"
#include "Component/SpriteComponent.h"
#include "Component/TransformComponent.h"

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

			if (ImGui::BeginTabItem("RandomMgr")) {
				DrawRandomMgr();
				ImGui::EndTabItem();
			}
			
			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}


void Debugs::DrawDrunkConsole() {
	if (ImGui::Begin("Drunkard Walk")) {
		auto rdm = Globals::GetInstance()->GetRandomMgr();

		static int size = 256;
		static int iteration = 5;
		static int distance = 25;
		static int spawnNum = 1;
		
		ImGui::SliderInt("Size X", &size, 16, 512);
		ImGui::SliderInt("Size Y", &size, 16, 512);
		ImGui::SliderInt("Iteration", &iteration, 1, 25);
		ImGui::SliderInt("Distance", &distance, 5, 50);
		ImGui::SliderInt("Spawn number", &spawnNum, 1, 24);
		
		if (ImGui::Button("Generate Drunkard Walk"))
			rdm->GenerateDrunkardWalk(size,size,iteration,distance,spawnNum);

		if (rdm->drunk != nullptr)
			ImGui::Image(rdm->drunk->Sprite(), {250,250});
	}
	ImGui::End();
}

void Debugs::DrawCellularConsole() {
	if (ImGui::Begin("Drunkard Walk")) {
		auto rdm = Globals::GetInstance()->GetRandomMgr();

		static int size = 256;
		static float spawnPercent = 0.6f;
		static int neighborThresh = 5;
		static int iteration = 5;
		
		ImGui::SliderInt("Size X", &size, 16, 512);
		ImGui::SliderInt("Size Y", &size, 16, 512);
		ImGui::SliderFloat("Spawn Percent", &spawnPercent, 0.1f, 1.f);
		ImGui::SliderInt("Neighbor Threshold", &neighborThresh, 1, 25);
		ImGui::SliderInt("Iteration", &iteration, 1, 25);
		
		if (ImGui::Button("Generate Drunkard Walk"))
			rdm->GenerateCellularAutomata(size,size,spawnPercent,neighborThresh,iteration);

		if (rdm->cellular != nullptr)
			ImGui::Image(rdm->cellular->Sprite(), {250,250});
	}
	ImGui::End();
}

//Peut-être nécessiter d'être modifier pour ajouter des PushID au for loop
void Debugs::DrawTextureMgr() {
	auto tex = Globals::GetInstance()->GetTextureMgr(); //Get texturesMap
	
	for (auto t : tex->GetTextureMap()) {
		if (ImGui::TreeNode(t.first.c_str())) {
			ImGui::Text("Texture width / height : %u", t.second.Texture.getSize().x);
			ImGui::SameLine();
			ImGui::Text(", %u", t.second.Texture.getSize().y);

			ImGui::Text("Texture Size : %u mb", t.second.Texture.getMaximumSize());
			ImGui::Text("Animations count : %zu", t.second.AnimationData.size());
			
			//Insérer ici par qui il est utilisé
			if (ImGui::TreeNode("Used By")) {
				auto used = Globals::GetInstance()->GetTextureMgr()->GetTextureUsedByEntity(t.first);

				if (!used.empty()) {
					ImGui::Text("Used By : %zu", used.size());
					
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
	
		for (auto e : Globals::GetInstance()->GetGameMgr()->GetEntities()) {
			ImGui::PushID(e);
			if (ImGui::TreeNode("##", e->entityName.c_str())) {
				ImGui::Text("Entity : %s", e->entityName.c_str());

				for (auto c : e->GetAllComponents()) {
					if (c == e->GetComponent<SpriteComponent>()) {

						auto component = e->GetComponent<SpriteComponent>();
						
						if (ImGui::TreeNode("Sprite Component")) {

							auto textures = Globals::GetInstance()->GetTextureMgr()->GetTextureMap();
							
							if (ImGui::BeginCombo("Textures", component->textureData->TextureName.c_str())) {
								for (auto t : textures) {
									const bool isSelected = t.first == component->textureData->TextureName;
									if (ImGui::Selectable(t.first.c_str(), isSelected)) {
										component->SetTexture(t.first);
									}

									if (isSelected)
										ImGui::SetItemDefaultFocus();
								}

								ImGui::EndCombo();
							}
							
							else {
								
							}
							
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

void Debugs::DrawRandomMgr() {
	auto rdm = Globals::GetInstance()->GetRandomMgr();
	
	if (ImGui::Button("Create Random Instance")) {
		rdm->CreateInstance();
	}

	if (ImGui::Button("Delete Last Instance")) {
		auto last = rdm->GetInstance(rdm->idValue - 1);
		if (last != nullptr) {
			auto last = std::prev(rdm->instanceMap.end());
			rdm->DestroyInstance(last->first);
		}
	}

	static int clicked = 0;
	if (ImGui::Button("Pop Drunkard Walk")) {
		clicked++;
	}
	if (clicked & 1) {
		DrawDrunkConsole();
	}
	
	if (ImGui::BeginTable("table1", 5))
	{
		// We could also set ImGuiTableFlags_SizingFixedFit on the table and all columns will default to ImGuiTableColumnFlags_WidthFixed.
		ImGui::TableSetupColumn("Seed", ImGuiTableColumnFlags_WidthFixed, 200.0f);
		ImGui::TableSetupColumn("RandUInt", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("RandInt32", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("RandDouble", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("RandNormalDouble", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		for (auto rdmId : rdm->instanceMap)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < 5; column++)
			{
				ImGui::TableSetColumnIndex(column);
				if (column == 0) {
					ImGui::Text("%u", rdmId.first);
				}
				if (column == 1) {
					ImGui::Text("%u", rdmId.second->RandUInt(0,10));
				}
				if (column == 2) {
					ImGui::Text("%d", rdmId.second->RandInt32(0,10));
				}
				if (column == 3) {
					ImGui::Text("%f", rdmId.second->RandDouble(0,10));
				}
				if (column == 4) {
					ImGui::Text("%f", rdmId.second->RandNormalDouble(0,10));
				}
			}
		}
		ImGui::EndTable();
	}
}

#endif
