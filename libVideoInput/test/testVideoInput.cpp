#include "VideoInputFactory.h"
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    cv::Mat frame(480,640,CV_8UC3);
    VideoInput *vi = VideoInputFactory::CreateVideoInput(VIDEOINPUTTYPE_V4L2, 1);
    vi->SetFrameSize(640,480);

    vi->SetNormalizedExposure(0);
    for (int i = 0; i < 10; ++i) vi->captureFrame(frame);
    cv::imwrite("low_exp.jpg", frame);
    vi->SetNormalizedExposure(90);
    for (int i = 0; i < 10; ++i) vi->captureFrame(frame);
    cv::imwrite("high_exp.jpg", frame);
    delete vi;
}
