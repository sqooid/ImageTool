#pragma once

#include <boost/gil.hpp>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <boost/gil.hpp>

namespace fs = std::filesystem;
namespace gil = boost::gil;

enum FILE_TYPES
{
	JPEG,
	PNG
};

class EditorState
{
public:
	void makeBackgroundTransparent(float color[4], float newAlpha, float tolerance);
	std::map<fs::path, int8_t> currentEditingFiles;
	void readImage(const fs::path& path);
	void saveImage(const fs::path& filePath);
	void test();

private:


	gil::rgba8_image_t srcImage;
	gil::rgba8_image_t dstImage;
};