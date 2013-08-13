#ifdef __gnu_linux__
#ifndef VIDEOINPUTV4L2_H
#define VIDEOINPUTV4L2_H


#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "VideoInputGeneric.h"
#include <stdint.h>
#include <linux/videodev2.h>


class VideoInputV4L2 : public VideoInputGeneric
{
private:
    int fd;

    int queryControl(u_int32_t controlID, v4l2_queryctrl *qc);
    int getControl(u_int32_t controlID, u_int32_t &val);
    int setControl(u_int32_t controlID, u_int32_t val);
    int setControlNormalized(u_int32_t controlID, u_int32_t valnorm);
    int normailze(v4l2_queryctrl *qc, int value);

public:
    VideoInputV4L2(int camID);
    ~VideoInputV4L2();

    virtual int IncrementCameraParameter(int param);
    virtual int DecrementCameraParameter(int param);
    virtual int SetNormalizedGain(int value);
    virtual int SetNormalizedExposure(int value);
    virtual int SetNormalizedWhiteBalance(int r, int g, int b);
    virtual int SetFrameSize(unsigned width, unsigned height);

};

#endif // VIDEOINPUTV4L2_H
#endif // __gnu_linux__
