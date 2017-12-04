#include "InterlaceFpsEnhancer.h"

VideoFrame * InterlaceFpsEnhancer::ReadNextFrame()
{
	if (_lastFrame == nullptr)
	{
		_lastFrame = _frameEnhancer->ReadNextEnhancedFrame();
	}
	if(_wasLastFrameEven)
	{
		return InterlaceFrame();
	}
	else
	{
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
				delete _lastFrame->Frame[h][w];
				_lastFrame->Frame[h][w] = new QColor(nextFrame->Frame[h][w]->rgb());
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
