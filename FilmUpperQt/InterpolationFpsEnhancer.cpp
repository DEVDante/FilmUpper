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
	VideoFrame* resultFrame = new VideoFrame(_targetQuality->FrameSizeX, _targetQuality->FrameSizeY);
	for (int horizontalIndex = 0; horizontalIndex < _targetQuality->FrameSizeX; ++horizontalIndex) {
		for (int verticalIndex = 0; verticalIndex < _targetQuality->FrameSizeY; ++verticalIndex) {
			resultFrame->Frame[horizontalIndex][verticalIndex][0] = VideoFrame::BlendColors(_currentFrame->Frame[horizontalIndex][verticalIndex][0], _nextFrame->Frame[horizontalIndex][verticalIndex][0], _currentFrameCooficiency);
			resultFrame->Frame[horizontalIndex][verticalIndex][1] = VideoFrame::BlendColors(_currentFrame->Frame[horizontalIndex][verticalIndex][1], _nextFrame->Frame[horizontalIndex][verticalIndex][1], _currentFrameCooficiency);
			resultFrame->Frame[horizontalIndex][verticalIndex][2] = VideoFrame::BlendColors(_currentFrame->Frame[horizontalIndex][verticalIndex][2], _nextFrame->Frame[horizontalIndex][verticalIndex][2], _currentFrameCooficiency);
		}
	}
	return resultFrame;
}
