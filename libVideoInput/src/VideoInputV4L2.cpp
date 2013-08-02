#ifdef __gnu_linux__
#include "VideoInputV4L2.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

VideoInputV4L2::VideoInputV4L2(int camID) : VideoInputGeneric(camID)
{
    char devFileName[13];
    sprintf(devFileName, "/dev/video%d", camID);
    fd = open(devFileName, O_RDWR);
    if (fd < 0) {
        //TODO exception?
    }
}

int VideoInputV4L2::queryControl(u_int32_t controlID, struct v4l2_queryctrl *qc)
{
    //TODO: it might be possible to optimalize the caos of if structures
    struct v4l2_queryctrl queryctrl;

    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = controlID;

    if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            //TODO: ioctl failed -> something is terribly wrong, at this point an exception should be thrown.
            return -1;
        } else {
            // This feature is simply not supported by the camera -- not sure
            return -1;
        }
    }
    if (qc != NULL)
        (*qc)=queryctrl;
    if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        // This feature is simply not supported by the camera
        return -1;
    }
    return 0;
}

int VideoInputV4L2::getControl(u_int32_t controlID, u_int32_t &val)
{
    struct v4l2_control control;

    if (queryControl(controlID, NULL) != -1) {
        //feature is supported
        memset (&control, 0, sizeof (control));
        control.id = controlID;

        if (-1 == ioctl (fd, VIDIOC_G_CTRL, &control)) {
            //TODO: ioctl failed -> something is terribly wrong, at this point an exception should be thrown.
            return -1;
        }
    }
    val=control.value;
    return 0;

}

int VideoInputV4L2::setControl(u_int32_t controlID, u_int32_t val)
{
    struct v4l2_control control;
    struct v4l2_queryctrl queryctrl;

    if (queryControl(controlID, &queryctrl) != -1) {
        //feature is supported
        memset (&control, 0, sizeof (control));
        control.id = controlID;
        control.value = val;

        if (-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
            //TODO: ioctl failed -> something is terribly wrong, at this point an exception should be thrown.
            return -1;
        }
    }
    return 0;
}

int VideoInputV4L2::IncrementCameraParameter(int param)
{
    return -1;
}

int VideoInputV4L2::DecrementCameraParameter(int param)
{
    return -1;
}

int VideoInputV4L2::normailze(v4l2_queryctrl *qc, int value)
{
    if (qc != NULL) {
        double min = qc->minimum;
        double max = qc->maximum;
        if (min==max) return min;
        return min+((max-min)*value)/100;
    }
    return 0;
}

int VideoInputV4L2::setControlNormalized(u_int32_t controlID, u_int32_t valnorm)
{
    struct v4l2_queryctrl qc;
    if (queryControl(controlID, &qc) != -1) {
        int val = normailze(&qc, valnorm);
        return setControl(controlID, val);
    }
    return -1;
}


int VideoInputV4L2::SetNormalizedGain(int value)
{
    if (value == -1) {
        return setControl(V4L2_CID_AUTOGAIN, 0);
    } else {
        if (setControl(V4L2_CID_AUTOGAIN, 1) != -1)
            return setControlNormalized(V4L2_CID_GAIN, value);
    }
    return -1;
}

int VideoInputV4L2::SetNormalizedExposure(int value)
{
    if (value == -1) {
        return setControl(V4L2_CID_EXPOSURE_AUTO, 0);
    } else {
        if (setControl(V4L2_CID_EXPOSURE_AUTO, 1) != -1) {
            return setControlNormalized(V4L2_CID_EXPOSURE, value);
        }
    }
    return -1;
}

int VideoInputV4L2::SetNormalizedWhiteBalance(int r, int g, int b)
{
    return -1;
}

VideoInputV4L2::~VideoInputV4L2()
{
    if (fd >= 0)
        (void)close(fd);
}

#endif //__gnu_linux__
