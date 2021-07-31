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
	bool transparency;

private:
	EditorState editor;

	bool bChoosingFiles;
	fs::path currentDirectory;
	std::map<fs::path, int8_t> fileAddBuffer;
	void showFileChooser();
	void addFiles();
};

