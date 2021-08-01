#include "EditorState.h"

#include <iostream>

#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/io/png.hpp>

static bool isMatchingColor(float pixel1[4], float pixel2[4], float maxDiff)
{
	return (pixel1[0] - pixel2[0]) * (pixel1[0] - pixel2[0]) < maxDiff * maxDiff &&
		(pixel1[1] - pixel2[1]) * (pixel1[1] - pixel2[1]) < maxDiff * maxDiff &&
		(pixel1[2] - pixel2[2]) * (pixel1[2] - pixel2[2]) < maxDiff * maxDiff &&
		(pixel1[3] - pixel2[3]) * (pixel1[3] - pixel2[3]) < maxDiff * maxDiff;
}

void EditorState::makeBackgroundTransparent(float color[4], float newAlpha, float tolerance)
{
	gil::transform_pixels(
		const_view(srcImage),
		view(dstImage),
		[color, tolerance, newAlpha](auto& pixel) {
			gil::red_t   R;
			gil::green_t G;
			gil::blue_t  B;
			gil::alpha_t A;

			const uint8_t RPix = gil::get_color(pixel, R);
			const uint8_t GPix = gil::get_color(pixel, G);
			const uint8_t BPix = gil::get_color(pixel, B);
			const uint8_t APix = gil::get_color(pixel, A);

			float pixelFloats[4] = {
				(float)RPix / 255.0f,
				(float)GPix / 255.0f,
				(float)BPix / 255.0f,
				(float)APix / 255.0f,
			};

			if (isMatchingColor(pixelFloats, color, tolerance))
			{
				return gil::rgba8_image_t::value_type(0, 0, 0, newAlpha * 255);
			}

			return gil::rgba8_image_t::value_type(RPix, GPix, BPix, APix);
		}
	);
}

void EditorState::test()
{
	readImage("jpgtest.jpg");

	saveImage("jpgout.png");
}

void EditorState::readImage(const fs::path& path)
{
	std::cout << "Started read image\n";
	std::string pathString = path.string();
	// Check extension
	std::string extension = path.extension().string();
	// If PNG
	if (extension == ".png")
	{
		gil::read_image(pathString, srcImage, gil::png_tag{});
	}
	else
	{
		// Is JPEG
		gil::read_and_convert_image(pathString, srcImage, gil::jpeg_tag{});
	}
	dstImage = gil::argb8_image_t(srcImage.dimensions());
	std::cout << "Finished read image\n";
}

void EditorState::saveImage(const fs::path& filePath)
{
	std::cout << "Started save image\n" << filePath.string() << "\n";
	gil::write_view(filePath.string(), gil::view(dstImage), gil::png_tag{});
	std::cout << "Finished save image\n\n";
}

