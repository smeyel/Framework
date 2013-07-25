#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/videodev2.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "USAGE: " << argv[0] << " camera_device" << endl;
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

    return 0;
}

