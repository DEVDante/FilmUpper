#pragma once
#include <qcolor.h>
#include <boost\multi_array.hpp>

typedef boost::multi_array<QColor*, 2> frameType;

class VideoFrame {
public:
	VideoFrame(int sizeX, int sizeY) {
		frameType::extent_gen ext;
		Frame.resize(ext[sizeX][sizeY]);
	}

	~VideoFrame() {
	}
	
	frameType Frame;

	static QColor* BlendColors(QColor * leftColor, QColor * rightColor, double blendRatio) {
		int red = (int)(((double)leftColor->red() * blendRatio) + ((double)rightColor->red() * (1 - blendRatio)));
		int green = (int)(((double)leftColor->green() * blendRatio) + ((double)rightColor->green() * (1 - blendRatio)));
		int blue = (int)(((double)leftColor->blue() * blendRatio) + ((double)rightColor->blue() * (1 - blendRatio)));
		if ((red > 255) || (green > 255) || (blue > 255))
			auto fail = 0;
		return new QColor(red, green, blue);
	}
};