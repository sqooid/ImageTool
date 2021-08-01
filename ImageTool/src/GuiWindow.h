#pragma once

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "EditorState.h"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <map>
#include <algorithm>

namespace fs = std::filesystem;



class GuiWindow
{
public:
	GuiWindow();
	void renderWindow();
	void setEditor(EditorState& editor);

private:
	EditorState editor;

	// Transparency
	void showTransparencyPanel();
	bool bEnableTransparency;
	ImVec4 transparencyColor;
	float alpha;
	float tolerance;

	// Saving
	void saveImages();
	char newFileSuffix[64];
	char newFilePrefix[64];

	// File chooser
	bool bChoosingFiles;
	fs::path currentDirectory;
	std::map<fs::path, int8_t> fileAddBuffer;
	void showFileChooser();
};

