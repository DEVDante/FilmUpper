#include "InterlaceFpsEnhancer.h"

VideoFrame * InterlaceFpsEnhancer::ReadNextFrame()
{
	if (!_framesLeft)
		return nullptr;
	if(_wasLastFrameEven)
	{
		_wasLastFrameEven = !_wasLastFrameEven;
		return InterlaceFrame();
	}
	else
	{
		_wasLastFrameEven = !_wasLastFrameEven;
		return _lastFrame->Clone();
	}
}

bool InterlaceFpsEnhancer::AreFramesLeft()
{
	return _framesLeft;
}

VideoFrame * InterlaceFpsEnhancer::InterlaceFrame()
{
	if(_frameEnhancer->AreFramesLeft())
	{
		VideoFrame* nextFrame = _frameEnhancer->ReadNextEnhancedFrame();
		if(nextFrame == nullptr)
		{
			_framesLeft = false;
			return _lastFrame;
		}
		for(int h = 0; h < (_targetQuality->Height); h+=2)
		{
			for (int w = 0; w < _targetQuality->Width; ++w)
			{
				_lastFrame->Frame[(h * _targetQuality->Width + w) * 3] = nextFrame->Frame[(h * _targetQuality->Width + w) * 3];
				_lastFrame->Frame[(h * _targetQuality->Width + w) * 3 + 1] = nextFrame->Frame[(h * _targetQuality->Width + w) * 3 + 1];
				_lastFrame->Frame[(h * _targetQuality->Width + w) * 3 + 2] = nextFrame->Frame[(h * _targetQuality->Width + w) * 3 + 2];
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