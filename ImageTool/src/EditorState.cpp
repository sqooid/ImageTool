#include "EditorState.h"

#include <iostream>

#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/io/png.hpp>

void EditorState::makeBackgroundTransparent()
{

}

void EditorState::test()
{
	readImage("32gi2i.png");
	saveImage("32gi2i (2).png");
}

void EditorState::readImage(const fs::path& path)
{
	std::string pathString = path.string();
	gil::read_image(pathString, srcImage, gil::png_tag{});
}

void EditorState::saveImage(const fs::path& filePath)
{
	gil::write_view(filePath.string(), gil::view(srcImage), gil::png_tag{});
}

