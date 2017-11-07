#include "InterpolationFpsEnchanter.h"

InterpolationFpsEnchanter::InterpolationFpsEnchanter(FrameEnchanterBase * frameEnchanter, FilmQualityInfo * qualityInfo): FpsEnchanterBase(frameEnchanter, qualityInfo)
{

}

VideoFrame* InterpolationFpsEnchanter::ReadNextFrame() {
	if (_currentFrame == nullptr)
	{
		_currentFrame = _frameEnchanter->ReadNextEnchantedFrame();
		_nextFrame = _frameEnchanter->ReadNextEnchantedFrame();
	}
	
	VideoFrame* resultFrame = new VideoFrame(_targetQuality->FrameSizeX, _targetQuality->FrameSizeY);
	


	return resultFrame;
}