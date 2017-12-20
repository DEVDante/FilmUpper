#pragma once
#include <qcolor.h>
#include <boost\multi_array.hpp>

typedef boost::multi_array<int, 3> frameType;

class VideoFrame {
public:
	VideoFrame(int height, int width) {
		frameType::extent_gen ext;
		Frame.resize(ext[height][width][3]);
		_width = width;
		_height = height;
	}

	~VideoFrame() {
	}
	
	frameType Frame;

	static inline int BlendColors(int leftColor, int rightColor, double blendRatio) {
		return (int)(((double)leftColor * blendRatio) + ((double)rightColor * (1 - blendRatio)));
	}

	VideoFrame* Clone()
	{
		VideoFrame* retFrame = new VideoFrame(_height, _width);
		for(int h = 0; h < _height; h++)
		{
			for (int w = 0; h < _width; w++)
			{
				retFrame->Frame[h][w][0] = Frame[h][w][0];
				retFrame->Frame[h][w][1] = Frame[h][w][1];
				retFrame->Frame[h][w][2] = Frame[h][w][2];
			}
		}
		return retFrame;
	}

private:
	int _width = 0;
	int _height = 0;
};