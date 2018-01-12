#include "InterpolationFpsEnhancer.h"

InterpolationFpsEnhancer::InterpolationFpsEnhancer(FrameEnhancerBase * frameEnhancer, FilmQualityInfo * qualityInfo): FpsEnhancerBase(frameEnhancer, qualityInfo)
{
	_nextFrameDelta =  _sourceQuality->FrameRate->getNumericalRate() / _targetQuality->FrameRate->getNumericalRate();
	_currentFrame = _frameEnhancer->ReadNextEnhancedFrame();
	_nextFrame = _frameEnhancer->ReadNextEnhancedFrame();
	_currentFrameCooficiency = 0;
	_framesLeft = true;

	_nextFramePrefetch = new VFHack;
	_framePrefetch = std::thread(PrefetchFrame, _frameEnhancer, _nextFramePrefetch);
}

VideoFrame* InterpolationFpsEnhancer::ReadNextFrame() {
	if (!_framesLeft)
		return nullptr;
	if (_currentFrameCooficiency > 1)
	{
		delete _currentFrame;
		_currentFrame = _nextFrame;
		_framePrefetch.join();
		_nextFrame = _nextFramePrefetch->Frame;
		if(_nextFrame == nullptr)
		{
			_framesLeft = false;
			return _currentFrame;
		}
		_framePrefetch = std::thread(PrefetchFrame, _frameEnhancer, _nextFramePrefetch);
		_currentFrameCooficiency -= 1;
	}
	
	VideoFrame* resultFrame = InterpolateFrames();
	_currentFrameCooficiency += _nextFrameDelta;
	return resultFrame;
}

InterpolationFpsEnhancer::~InterpolationFpsEnhancer()
{
	delete _nextFramePrefetch;
}

bool InterpolationFpsEnhancer::AreFramesLeft()
{
	return _framesLeft;
}

VideoFrame * InterpolationFpsEnhancer::InterpolateFrames() const
{
	VideoFrame* resultFrame = new VideoFrame(_targetQuality->Height, _targetQuality->Width);
	for (int verticalIndex = 0; verticalIndex < _targetQuality->Height; ++verticalIndex) {
		for (int horizontalIndex = 0; horizontalIndex < _targetQuality->Width; ++horizontalIndex) {
			resultFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3] = VideoFrame::BlendColors(_currentFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3], _nextFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3], 1 - _currentFrameCooficiency);
			resultFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 1] = VideoFrame::BlendColors(_currentFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 1], _nextFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 1], 1 - _currentFrameCooficiency);
			resultFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 2] = VideoFrame::BlendColors(_currentFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 2], _nextFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 2], 1 - _currentFrameCooficiency);
		}
	}
	return resultFrame;
}

void InterpolationFpsEnhancer::PrefetchFrame(FrameEnhancerBase* frameEnhancer, VFHack * vf)
{
	vf->Frame = frameEnhancer->ReadNextEnhancedFrame();
}
