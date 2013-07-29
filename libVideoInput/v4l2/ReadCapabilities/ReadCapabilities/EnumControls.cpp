#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <errno.h>

struct v4l2_queryctrl queryctrl;
struct v4l2_querymenu querymenu;

static void enumerate_menu (int fd)
{
    printf ("\tMenu items:\n");

    memset (&querymenu, 0, sizeof (querymenu));
    querymenu.id = queryctrl.id;

    for (querymenu.index = queryctrl.minimum;
         querymenu.index <= (__u32)queryctrl.maximum;
         querymenu.index++) {
        if (0 == ioctl (fd, VIDIOC_QUERYMENU, &querymenu)) {
            printf ("\t%s\n", querymenu.name);
        }
    }
}

void EnumControls(int fd)
{
    memset (&queryctrl, 0, sizeof (queryctrl));

    for (queryctrl.id = V4L2_CID_BASE;
         queryctrl.id < V4L2_CID_LASTP1;
         queryctrl.id++) {
        if (0 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
            if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
                continue;

            printf ("Control %s\n", queryctrl.name);
            switch (queryctrl.type) {
            case V4L2_CTRL_TYPE_MENU:
                enumerate_menu (fd);
                break;
            case V4L2_CTRL_TYPE_INTEGER:
                printf("\tInteger, minimum=%d, maximum=%d, step=%d\n",
                       queryctrl.minimum, queryctrl.maximum, queryctrl.step);
                break;
            case V4L2_CTRL_TYPE_BOOLEAN:
                printf("\tBoolean\n");
                break;
            case V4L2_CTRL_TYPE_INTEGER_MENU:
                printf("\tInteger menu, maximum=%d\n", queryctrl.maximum);
                break;
            case V4L2_CTRL_TYPE_BITMASK:
                printf("\tBitmask\n");
                break;
            case V4L2_CTRL_TYPE_INTEGER64:
                printf("\tInteger64\n");
                break;
            case V4L2_CTRL_TYPE_STRING:
                printf("\tString\n");
                break;
            case V4L2_CTRL_TYPE_CTRL_CLASS:
                printf("\tCTRL_CLASS\n");
                break;
            default:
                printf("\tUnknown type: %d\n", queryctrl.type);
                break;
            }
        } else {
            if (errno == EINVAL)
                continue;

            perror ("VIDIOC_QUERYCTRL");
            exit (EXIT_FAILURE);
        }
    }

    for (queryctrl.id = V4L2_CID_PRIVATE_BASE;;
         queryctrl.id++) {
        if (0 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
            if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
                continue;

            printf ("Controllll %s\n", queryctrl.name);
            switch (queryctrl.type) {
            case V4L2_CTRL_TYPE_MENU:
                enumerate_menu (fd);
                break;
            case V4L2_CTRL_TYPE_INTEGER:
                printf("\tInteger, minimum=%d, maximum=%d, step=%d\n",
                       queryctrl.minimum, queryctrl.maximum, queryctrl.step);
                break;
            case V4L2_CTRL_TYPE_BOOLEAN:
                printf("\tBoolean\n");
                break;
            case V4L2_CTRL_TYPE_INTEGER_MENU:
                printf("\tInteger menu, maximum=%d\n", queryctrl.maximum);
                break;
            case V4L2_CTRL_TYPE_BITMASK:
                printf("\tBitmask\n");
                break;
            case V4L2_CTRL_TYPE_INTEGER64:
                printf("\tInteger64\n");
                break;
            case V4L2_CTRL_TYPE_STRING:
                printf("\tString\n");
                break;
            case V4L2_CTRL_TYPE_CTRL_CLASS:
                printf("\tCTRL_CLASS\n");
                break;
            default:
                printf("\tUnknown type: %d\n", queryctrl.type);
                break;
            }

        } else {
            if (errno == EINVAL)
                break;

            perror ("VIDIOC_QUERYCTRL");
            exit (EXIT_FAILURE);
        }
    }
}
