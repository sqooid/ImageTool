#include "GUIManager.h"

void GUIManager::renderWindow()
{
	static bool amGay = false;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Fullscreen window", NULL, windowFlags));
	{
		if (amGay)
		{
			ImGui::Text("Ur gay");
		}
		else
		{
			ImGui::Checkbox("I'm gay", &amGay);

		}
	}
	ImGui::End();
}

