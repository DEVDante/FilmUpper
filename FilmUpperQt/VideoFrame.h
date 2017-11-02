#pragma once
#include <qcolor.h>
#include <boost\multi_array.hpp>

typedef boost::multi_array<QColor*, 2> frameType;

class VideoFrame {
public:
	VideoFrame(int sizeX, int sizeY) {
		Frame = frameType(boost::extents[sizeX][sizeY]);
	}

	~VideoFrame() {
	}
	
	frameType Frame;
};