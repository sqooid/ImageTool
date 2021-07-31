#pragma once

#include <boost/gil.hpp>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <boost/gil.hpp>

namespace fs = std::filesystem;
namespace gil = boost::gil;

class EditorState
{
public:
	void makeBackgroundTransparent();
	std::map<fs::path, int8_t> currentEditingFiles;
	void test();

private:
	void readImage(const fs::path& path);
	void saveImage(const fs::path& filePath);

	gil::argb8_image_t srcImage;
	gil::argb8_image_t dstImage;
};