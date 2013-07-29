#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/videodev2.h>
#include "EnumControls.h"

using namespace std;


void printcaps(__u32 cap)
{
    /* generated with
     * $ grep V4L2_CAP_ /usr/include/linux/videodev2.h |
         awk '{printf "case %s: cout << \"%s\" << endl; break;\n", $3, $2}' */
    for (__u32 bit = 0x80000000; bit > 0; bit>>=1) {
        switch (cap & bit) {
        case 0x00000001: cout << "V4L2_CAP_VIDEO_CAPTURE" << endl; break;
        case 0x00000002: cout << "V4L2_CAP_VIDEO_OUTPUT" << endl; break;
        case 0x00000004: cout << "V4L2_CAP_VIDEO_OVERLAY" << endl; break;
        case 0x00000010: cout << "V4L2_CAP_VBI_CAPTURE" << endl; break;
        case 0x00000020: cout << "V4L2_CAP_VBI_OUTPUT" << endl; break;
        case 0x00000040: cout << "V4L2_CAP_SLICED_VBI_CAPTURE" << endl; break;
        case 0x00000080: cout << "V4L2_CAP_SLICED_VBI_OUTPUT" << endl; break;
        case 0x00000100: cout << "V4L2_CAP_RDS_CAPTURE" << endl; break;
        case 0x00000200: cout << "V4L2_CAP_VIDEO_OUTPUT_OVERLAY" << endl; break;
        case 0x00000400: cout << "V4L2_CAP_HW_FREQ_SEEK" << endl; break;
        case 0x00000800: cout << "V4L2_CAP_RDS_OUTPUT" << endl; break;
        case 0x00001000: cout << "V4L2_CAP_VIDEO_CAPTURE_MPLANE" << endl; break;
        case 0x00002000: cout << "V4L2_CAP_VIDEO_OUTPUT_MPLANE" << endl; break;
        case 0x00004000: cout << "V4L2_CAP_VIDEO_M2M_MPLANE" << endl; break;
        case 0x00008000: cout << "V4L2_CAP_VIDEO_M2M" << endl; break;
        case 0x00010000: cout << "V4L2_CAP_TUNER" << endl; break;
        case 0x00020000: cout << "V4L2_CAP_AUDIO" << endl; break;
        case 0x00040000: cout << "V4L2_CAP_RADIO" << endl; break;
        case 0x00080000: cout << "V4L2_CAP_MODULATOR" << endl; break;
        case 0x01000000: cout << "V4L2_CAP_READWRITE" << endl; break;
        case 0x02000000: cout << "V4L2_CAP_ASYNCIO" << endl; break;
        case 0x04000000: cout << "V4L2_CAP_STREAMING" << endl; break;
        case 0x80000000: cout << "V4L2_CAP_DEVICE_CAPS" << endl; break;
        }
    }
}

void setControl(int fd, __u32 controlID, const char *controlIDString, __u32 val)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;

    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = controlID;

    if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
        if (errno != EINVAL) {
            perror ("VIDIOC_QUERYCTRL");
            exit (EXIT_FAILURE);
        } else {
            printf ("%s is not supported\n", controlIDString);
        }
    } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        printf ("%s is not supported\n", controlIDString);
    } else {
        memset (&control, 0, sizeof (control));
        control.id = controlID;
        control.value = val;

        if (-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
            perror ("VIDIOC_S_CTRL");
            exit (EXIT_FAILURE);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "USAGE: " << argv[0] << " v4l_device" << endl;
        exit(1);
    }

    int fd;
    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("Cannot open file.");
        exit(1);
    }

    struct v4l2_capability cap;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        perror("ioctl");
        exit(1);
    }

    cout << "driver: " << cap.driver << endl;
    cout << "card: " << cap.card << endl;
    cout << "businfo: " << cap.bus_info << endl;
    cout << "version: " << (cap.version>>16 & 0xFF) << "." <<  (cap.version>>8 & 0xFF)
         << "." << (cap.version & 0xFF) << endl;

    cout << "Capabilities:" << endl;
    printcaps(cap.capabilities);
    cout << endl;

    cout << "Device capabilities:" << endl;
    printcaps(cap.device_caps);
    cout << endl;

    EnumControls(fd);

    setControl(fd, V4L2_CID_EXPOSURE_AUTO, "V4L2_CID_EXPOSURE_AUTO", 1);
    setControl(fd, V4L2_CID_EXPOSURE, "V4L2_CID_EXPOSURE", 10);


    return 0;
}

