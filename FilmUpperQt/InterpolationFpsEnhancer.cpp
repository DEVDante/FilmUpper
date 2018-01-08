#include "InterpolationFpsEnhancer.h"

InterpolationFpsEnhancer::InterpolationFpsEnhancer(FrameEnhancerBase * frameEnhancer, FilmQualityInfo * qualityInfo): FpsEnhancerBase(frameEnhancer, qualityInfo)
{
	_nextFrameDelta =  _sourceQuality->FrameRate->getNumericalRate() / _targetQuality->FrameRate->getNumericalRate();
	_currentFrame = _frameEnhancer->ReadNextEnhancedFrame();
	_nextFrame = _frameEnhancer->ReadNextEnhancedFrame();
	_currentFrameCooficiency = 0;
}

VideoFrame* InterpolationFpsEnhancer::ReadNextFrame() {
	if (_currentFrameCooficiency > 1)
	{
		delete _currentFrame;
		_currentFrame = _nextFrame;
		_nextFrame = _frameEnhancer->ReadNextEnhancedFrame();
		_currentFrameCooficiency -= 1;
	}
	
	VideoFrame* resultFrame = InterpolateFrames();
	_currentFrameCooficiency += _nextFrameDelta;
	return resultFrame;
}

VideoFrame * InterpolationFpsEnhancer::InterpolateFrames() const
{
	VideoFrame* resultFrame = new VideoFrame(_targetQuality->Height, _targetQuality->Width);
	for (int verticalIndex = 0; verticalIndex < _targetQuality->Height; ++verticalIndex) {
		for (int horizontalIndex = 0; horizontalIndex < _targetQuality->Width; ++horizontalIndex) {
			resultFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3] = VideoFrame::BlendColors(_currentFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3], _nextFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3], _currentFrameCooficiency);
			resultFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 1] = VideoFrame::BlendColors(_currentFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 1], _nextFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 1], _currentFrameCooficiency);
			resultFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 2] = VideoFrame::BlendColors(_currentFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 2], _nextFrame->Frame[(verticalIndex * _targetQuality->Width + horizontalIndex) * 3 + 2], _currentFrameCooficiency);
		}
	}
	return resultFrame;
}
