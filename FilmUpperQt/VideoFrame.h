#pragma once

class VideoFrame {
public:
	VideoFrame(int height, int width) {
		Frame = new uint8_t[height * width * 3];
		_width = width;
		_height = height;
	}

	~VideoFrame() {
		delete[] Frame;
	}
	
	uint8_t* Frame;

	static inline int BlendColors(int leftColor, int rightColor, double blendRatio) {
		return (int)(((double)leftColor * blendRatio) + ((double)rightColor * (1 - blendRatio)));
	}

	int GetBufferSize() {
		return _width * _height * 3;
	}

	VideoFrame* Clone()
	{
		VideoFrame* retFrame = new VideoFrame(_height, _width);
		/*for(int s = 0; s < _height * _width * 3; s++)
		{
			retFrame->Frame[s] = Frame[s];
		}*/
		memcpy(retFrame->Frame, Frame, GetBufferSize());
		return retFrame;
	}

private:
	int _width = 0;
	int _height = 0;
};