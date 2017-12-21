#pragma once
#include <qcolor.h>
#include <boost\multi_array.hpp>

class VideoFrame {
public:
	VideoFrame(int height, int width) {
		Frame = new int[height * width * 3];
		_width = width;
		_height = height;
	}

	~VideoFrame() {
		delete Frame;
	}
	
	int* Frame;

	static inline int BlendColors(int leftColor, int rightColor, double blendRatio) {
		return (int)(((double)leftColor * blendRatio) + ((double)rightColor * (1 - blendRatio)));
	}

	VideoFrame* Clone()
	{
		VideoFrame* retFrame = new VideoFrame(_height, _width);
		for(int s = 0; s < _height * _width * 3; s++)
		{
			retFrame->Frame[s] = Frame[s];
		}
		return retFrame;
	}

private:
	int _width = 0;
	int _height = 0;
};