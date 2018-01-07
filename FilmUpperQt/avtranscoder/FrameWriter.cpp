#include "FrameWriter.h"

#define VIDEO_STREAM_INDEX 0

FrameWriter::FrameWriter(std::string filename, std::string format_name, std::string video_enc_name, std::string audio_enc_name, FilmQualityInfo *nfo)
{
	/* SETUP PARAMS */
	fileParam["r"] = nfo->FrameRate->getNumericalRate();
	fileParam["pix_fmt"] = "yuv420p";
	fileParam["height"] = nfo->Height;
	fileParam["width"] = nfo->Width;

	/* INIT VARS */
	file = new avtranscoder::OutputFile(filename, format_name);
	videoEncoder = new avtranscoder::VideoEncoder(video_enc_name);
	frameConverter = new avtranscoder::VideoTransform();
	rgbDataBuffer = new unsigned char[nfo->Height * nfo->Width * 3];
	//videoCodec = new avtranscoder::VideoCodec(avtranscoder::eCodecTypeEncoder, video_enc_name);

	/* INSERT PARAMS */
	avtranscoder::VideoFrameDesc frameParam(nfo->Width, nfo->Height, fileParam["pix_fmt"]);
	frameParam.setParameters(fileParam);
	avtranscoder::VideoFrameDesc frameParamRGB(nfo->Width, nfo->Height, "rgb24");

	/* INIT FRAMES */
	frame = new avtranscoder::VideoFrame(frameParam);
	frame->assignValue(0);
	frameRGB = new avtranscoder::VideoFrame(frameParamRGB);
	videoEncoder->setupVideoEncoder(frameParam, fileParam);

	/* SETUP FILE FOR ENCODING */
	file->setupWrapping(fileParam);
	file->addVideoStream(videoEncoder->getVideoCodec());
	file->beginWrap();
}


FrameWriter::~FrameWriter()
{
	file->endWrap();

	delete frame;
	delete frameRGB;
	delete videoEncoder;
	delete frameConverter;
	delete file;
}


void FrameWriter::WriteFrame(VideoFrame *vframe)
{
	
	for (int i = 0; i < vframe->GetBufferSize(); i++)
		rgbDataBuffer[i] = vframe->Frame[i];
	frameRGB->assignBuffer(rgbDataBuffer);
	frameConverter->convert(*frameRGB, *frame);

	avtranscoder::CodedData packet;
	videoEncoder->encodeFrame(*frame, packet);

	file->wrap(packet, VIDEO_STREAM_INDEX);
}