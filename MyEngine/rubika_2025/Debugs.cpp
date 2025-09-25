#ifdef USE_IMGUI

#include "Debugs.h"

#include <ImGui/imgui.h>

void Debugs::DrawDebugWindow()
{
	GetInstance()->DrawDebug();
}

Logger& Debugs::GetLogger()
{
	return GetInstance()->Logger;
}

Debugs* Debugs::GetInstance()
{
	if (!Instance)
	{
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
			if (ImGui::BeginTabItem("Logger"))
			{
				Logger::DrawLogger();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("TextureMgr"))
			{
				ImGui::Text("This is the textureMgr Tab!\nKawai-des !");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

#endif