#include "GuiWindow.h"

namespace constants {
	std::vector<std::string> validFileTypes = {
		".jpg",
		".jpeg",
		".png"
	};
}

GuiWindow::GuiWindow() :
	bChoosingFiles{ false },
	editor{},
	transparency{ false }
{
	currentDirectory = fs::current_path();
}

void GuiWindow::renderWindow()
{
	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (ImGui::Begin("Main Window", NULL, windowFlags))
	{
		// Button to open file chooser
		if (ImGui::Button("Add Files"))
		{
			bChoosingFiles = true;
		}

		// Button to remove flagged files
		if (ImGui::Button("Remove selected"))
		{
			auto itSelected = editor.currentEditingFiles.begin();
			while (itSelected != editor.currentEditingFiles.end())
			{
				if ((*itSelected).second == 2)
				{
					itSelected = editor.currentEditingFiles.erase(itSelected);
				}
				else
				{
					++itSelected;
				}
			}
		}

		// Show chosen files
		ImGui::Text("Currently editing files:");
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::BeginChild("Files", ImVec2(windowSize.x * 0.95f, std::max(windowSize.y * 0.2f, TEXT_BASE_WIDTH * 2.0f)), true, 0);

		for (auto& file : editor.currentEditingFiles)
		{
			// If flagged for possible removal
			if (file.second == 2)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.05f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.05f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.05f, 0.8f, 0.8f));
				if (ImGui::Button(file.first.string().c_str()))
				{
					file.second = 1;
				}
				ImGui::PopStyleColor(3);
			}
			else
			{
				// If normal
				if (ImGui::Button(file.first.string().c_str()))
				{
					file.second = 2;
				}
			}
		}

		ImGui::EndChild();
	}

	/*
	Editing options
	*/
	if (ImGui::CollapsingHeader("Transparency"))
	{
		if (ImGui::Button("Enable"))
		{
			editor.test();
		}
	}

	ImGui::End();

	// Popups and other things
	if (bChoosingFiles)
	{
		showFileChooser();
	}


}

void GuiWindow::setEditor(EditorState& editor)
{
	this->editor = editor;
}

void GuiWindow::showFileChooser()
{
	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	ImGui::OpenPopup("Add Files");

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	// Centering popup
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopup("Add Files"), NULL, windowFlags)
	{
		ImGui::TextWrapped("Current directory: %s", currentDirectory.string().c_str());

		if (ImGui::Button("Select all"))
		{
			for (const auto& entry : fs::directory_iterator(currentDirectory))
			{
				std::string extension = entry.path().extension().string();

				if (std::any_of(constants::validFileTypes.begin(), constants::validFileTypes.end(),
					[extension](const std::string& ext) {return ext == extension;}))
					fileAddBuffer[entry.path()] = 1;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("De-select all"))
		{
			for (const auto& entry : fs::directory_iterator(currentDirectory))
			{
				std::string extension = entry.path().extension().string();

				if (std::any_of(constants::validFileTypes.begin(), constants::validFileTypes.end(),
					[extension](const std::string& ext) {return ext == extension;}))
					fileAddBuffer.erase(entry.path());
			}
		}
		// Files
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

		ImGui::BeginChild("Dir",
			ImVec2(ImGui::GetMainViewport()->WorkSize.x * 0.7f, ImGui::GetMainViewport()->WorkSize.y * 0.7f), false, window_flags);
		// Back navigation
		if (ImGui::Button(".."))
		{
			currentDirectory = currentDirectory.parent_path();
		}
		for (const auto& entry : fs::directory_iterator(currentDirectory))
		{
			std::string filename = entry.path().filename().string();
			std::string extension = entry.path().extension().string();

			// If item is a folder
			if (extension.empty())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.13f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.13f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.13f, 0.8f, 0.8f));
				if (ImGui::Button(filename.c_str()))
				{
					currentDirectory = entry.path();
				}
				ImGui::PopStyleColor(3);
			}

			// Check if item is a valid file type
			else if (std::any_of(constants::validFileTypes.begin(), constants::validFileTypes.end(),
				[extension](const std::string& ext) {return ext == extension;}))
			{
				// If item is NOT selected
				if (fileAddBuffer.find(entry.path()) == fileAddBuffer.end())
				{
					if (ImGui::Button(filename.c_str()))
					{
						fileAddBuffer[entry.path()] = 1;
					}
				}

				// If item is selected
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.2f, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.2f, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.2f, 0.8f, 0.8f));
					if (ImGui::Button(filename.c_str()))
					{
						fileAddBuffer.erase(entry.path());
					}
					ImGui::PopStyleColor(3);
				}
			}

			// Else not a file of interest
		}



		ImGui::EndChild();

		ImGui::Separator();

		// Buttons
		if (ImGui::Button("Add selected"))
		{
			editor.currentEditingFiles = fileAddBuffer;
			// Close file chooser
			bChoosingFiles = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			bChoosingFiles = false;
		}
	}
	ImGui::End();
}

void GuiWindow::addFiles()
{
}

