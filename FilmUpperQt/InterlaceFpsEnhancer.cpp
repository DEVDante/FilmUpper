#include "InterlaceFpsEnhancer.h"

VideoFrame * InterlaceFpsEnhancer::ReadNextFrame()
{
	if (!_framesLeft)
		return nullptr;
	if(_wasLastFrameOdd)
	{
		_wasLastFrameOdd = !_wasLastFrameOdd;
		return InterlaceFrame();
	}
	else
	{
		_wasLastFrameOdd = !_wasLastFrameOdd;
		return _lastFrame->Clone();
	}
}

bool InterlaceFpsEnhancer::AreFramesLeft()
{
	return _framesLeft;
}

VideoFrame * InterlaceFpsEnhancer::InterlaceFrame()
{
	_framePrefetch.join();
	VideoFrame* nextFrame = _nextFramePrefetch->Frame;
	if(nextFrame == nullptr)
	{
		_framesLeft = false;
		return _lastFrame;
	}
	_framePrefetch = std::thread(PrefetchFrame, _frameEnhancer, _nextFramePrefetch);
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

void InterlaceFpsEnhancer::PrefetchFrame(FrameEnhancerBase * frameEnhancer, VFHack * vf)
{
	vf->Frame = frameEnhancer->ReadNextEnhancedFrame();
}

InterlaceFpsEnhancer::InterlaceFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* targetQuality)
	: FpsEnhancerBase(frameEnhancer, targetQuality)
{
	//Force 2 times increase in framerate
	_targetQuality->FrameRate->num = _sourceQuality->FrameRate->num * 2;
	_targetQuality->FrameRate->den = _sourceQuality->FrameRate->den;

	_lastFrame = _frameEnhancer->ReadNextEnhancedFrame();

	_nextFramePrefetch = new VFHack;
	_framePrefetch = std::thread(PrefetchFrame, _frameEnhancer, _nextFramePrefetch);
}

InterlaceFpsEnhancer::~InterlaceFpsEnhancer()
{
	delete _nextFramePrefetch;
}
