#include "InterpolationFpsEnhancer.h"

InterpolationFpsEnhancer::InterpolationFpsEnhancer(FrameEnhancerBase * frameEnhancer, FilmQualityInfo * qualityInfo): FpsEnhancerBase(frameEnhancer, qualityInfo)
{
	_nextFrameDelta = _targetQuality->FrameRate->getNumericalRate() / _sourceQuality->FrameRate->getNumericalRate();
}

VideoFrame* InterpolationFpsEnhancer::ReadNextFrame() {
	if (_currentFrame == nullptr)
	{
		_currentFrame = _frameEnhancer->ReadNextEnhancedFrame();
		_nextFrame = _frameEnhancer->ReadNextEnhancedFrame();
	}
	
	
	if (_currentFrameCooficiency > 1)
	{
		delete _currentFrame;
		_currentFrame = _nextFrame;
		VideoFrame* tmpFrame = nullptr;
		while (_currentFrameCooficiency > 1) {
			if (_frameEnhancer->AreFramesLeft())
			{
				if (tmpFrame != nullptr)
					delete tmpFrame;
				tmpFrame = _frameEnhancer->ReadNextEnhancedFrame();
			}
			else
			{
				break;
			}
			_currentFrameCooficiency--;			
		}
		_nextFrame = tmpFrame;
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
			resultFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3] = VideoFrame::BlendColors(_currentFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3], _nextFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3], _currentFrameCooficiency);
			resultFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3 + 1] = VideoFrame::BlendColors(_currentFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3 + 1], _nextFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3 + 1], _currentFrameCooficiency);
			resultFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3 + 2] = VideoFrame::BlendColors(_currentFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3 + 2], _nextFrame->Frame[verticalIndex * _targetQuality->Width + horizontalIndex * 3 + 2], _currentFrameCooficiency);
		}
	}
	return resultFrame;
}
