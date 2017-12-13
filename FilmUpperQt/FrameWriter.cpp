//#include "FrameWriter.h"
/*
FrameWriter::FrameWriter( std::string filename, std::string format_name)
{
	av_register_all();

	if (avformat_alloc_output_context2(&formatCTX, NULL, format_name, filename) != 0)
		throw std::runtime_error("Couldn't allocate output format context");


	codec = avcodec_find_encoder_by_name(enc_name);
	if (codec == NULL) 
		throw std::runtime_error("Couldn't find given encoder");

	codecCTX = avcodec_alloc_context3(codec);
	if (avcodec_copy_context(codecCTX, codecCtxOriginal) != 0) 
		throw std::runtime_error("Couldn't copy codec context");

	if (avcodec_open2(codecCTX, codec, NULL)<0)
		throw std::runtime_error("Couldn't open codec");

    frame=av_frame_alloc();
    if (frame == NULL)
        throw std::bad_alloc::bad_alloc();
    frameRGB=av_frame_alloc();
    if (frameRGB == NULL)
		throw std::bad_alloc::bad_alloc();

    numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height);
    frameBuffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    avpicture_fill((AVPicture *)frameRGB, frameBuffer, AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height);

}


FrameReader::~FrameReader()
{
	avcodec_close(codecCTX);
	avcodec_close(codecCtxOriginal);
	avformat_close_input(&formatCTX);
}


void FrameReader::WriteFrame(VideoFrame *frame)
{
    struct SwsContext *sws_ctx = NULL;
    int frameFinished;
    AVPacket packet;

    sws_ctx = sws_getContext(codecCTX->width, codecCTX->height, codecCTX->pix_fmt, codecCTX->width, codecCTX->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    do {
        if (av_read_frame(formatCTX, &packet)>=0)
            //throw std::runtime_error("Couldn't read frame data");
            return NULL;
        
        if(packet.stream_index==videoStream) 
            avcodec_decode_video2(codecCTX, frame, &frameFinished, &packet);
    } while(!frameFinished);

    sws_scale(sws_ctx, (uint8_t const * const *)frame->data, frame->linesize, 0, codecCTX->height, frameRGB->data, frameRGB->linesize);
    
    VideoFrame *outFrame = new VideoFrame(codecCTX->width, codecCTX->height);

    for(int y = 0; y < codecCTX->height; y++) 
        for(int x = 0; x < codecCTX->width; x++) 
        {
            int offset = 3 * x + y * c->linesize[0];

			outFrame->Frame[x][y] = new QColor(frameRGB->data[0][offset + 0], frameRGB->data[0][offset + 1], frameRGB->data[0][offset + 2]);
            //outFrame->Frame[x][y].setRed(frameRGB->data[0][offset + 0]);
            //outFrame->Frame[x][y].setGreen(frameRGB->data[0][offset + 1]);
			//outFrame->Frame[x][y].setBlue(frameRGB->data[0][offset + 2]);
        }
    
    return outFrame;
}
*/