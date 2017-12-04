#pragma once
#include <qcolor.h>
#include <boost\multi_array.hpp>

typedef boost::multi_array<QColor*, 2> frameType;

class VideoFrame {
public:
	VideoFrame(int height, int width) {
		frameType::extent_gen ext;
		Frame.resize(ext[height][width]);
		_width = width;
		_height = height;
	}

	~VideoFrame() {
	}
	
	frameType Frame;

	static QColor* BlendColors(QColor * leftColor, QColor * rightColor, double blendRatio) {
		int red = (int)(((double)leftColor->red() * blendRatio) + ((double)rightColor->red() * (1 - blendRatio)));
		int green = (int)(((double)leftColor->green() * blendRatio) + ((double)rightColor->green() * (1 - blendRatio)));
		int blue = (int)(((double)leftColor->blue() * blendRatio) + ((double)rightColor->blue() * (1 - blendRatio)));
		return new QColor(red, green, blue);
	}

	VideoFrame* Clone()
	{
		VideoFrame* retFrame = new VideoFrame(_height, _width);
		for(int h = 0; h < _height; h++)
		{
			for (int w = 0; h < _width; w++)
			{
				retFrame->Frame[h][w] = new QColor(Frame[h][w]->rgb());
			}
		}
		return retFrame;
	}

private:
	int _width = 0;
	int _height = 0;
};