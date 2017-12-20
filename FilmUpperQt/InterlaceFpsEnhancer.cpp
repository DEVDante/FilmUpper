#include "InterlaceFpsEnhancer.h"

VideoFrame * InterlaceFpsEnhancer::ReadNextFrame()
{
	if (_lastFrame == nullptr)
	{
		_lastFrame = _frameEnhancer->ReadNextEnhancedFrame();
	}
	if(_wasLastFrameEven)
	{
		_wasLastFrameEven != _wasLastFrameEven;
		return InterlaceFrame();
	}
	else
	{
		_wasLastFrameEven != _wasLastFrameEven;
		return _lastFrame->Clone();
	}
}

bool InterlaceFpsEnhancer::AreFramesLeft()
{
	return (_frameEnhancer->AreFramesLeft() || !_wasLastFrameEven);
}

VideoFrame * InterlaceFpsEnhancer::InterlaceFrame()
{
	if(_frameEnhancer->AreFramesLeft())
	{
		VideoFrame* nextFrame = _frameEnhancer->ReadNextEnhancedFrame();
		for(int h = 0; h < (_targetQuality->FrameSizeX / 2); ++h)
		{
			for (int w = 0; w < _targetQuality->FrameSizeY; ++w)
			{
				_lastFrame->Frame[h][w][0] = nextFrame->Frame[h][w][0];
				_lastFrame->Frame[h][w][1] = nextFrame->Frame[h][w][1];
				_lastFrame->Frame[h][w][2] = nextFrame->Frame[h][w][2];
			}
		}
		auto tmp = _lastFrame;
		_lastFrame = nextFrame;
		return tmp;
	}
	else
	{
		return _lastFrame;
	}
	return nullptr;
}
