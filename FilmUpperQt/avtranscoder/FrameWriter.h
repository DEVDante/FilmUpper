#pragma once
#include "IFrameReader.h"
#include <AvTranscoder\file\OutputFile.hpp>
#include <AvTranscoder\encoder\VideoEncoder.hpp>
#include <AvTranscoder\encoder\AudioEncoder.hpp>
#include <AvTranscoder\transform\VideoTransform.hpp>
#include <map>


class FrameWriter
{
private:
	avtranscoder::OutputFile *file;
	//avtranscoder::VideoCodec *videoCodec;
	avtranscoder::VideoFrame *frame;
	avtranscoder::VideoFrame *frameRGB;
	avtranscoder::VideoEncoder *videoEncoder;
	avtranscoder::VideoTransform *frameConverter;
	avtranscoder::ProfileLoader::Profile fileParam;

	unsigned char *rgbDataBuffer;

public:
	FrameWriter(std::string filename, std::string format_name, std::string video_enc_name, std::string audio_enc_name, FilmQualityInfo *nfo);
	~FrameWriter();
	void WriteFrame(VideoFrame*); // override;
};
