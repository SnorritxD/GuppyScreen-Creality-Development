#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main(void)
{
    int fd = open("/dev/fb1", O_RDWR);
    if (fd < 0) {
        perror("open /dev/fb1");
        return 1;
    }

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;

    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) < 0) {
        perror("FBIOGET_FSCREENINFO");
        return 1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("FBIOGET_VSCREENINFO");
        return 1;
    }

    printf("xres          = %u\n", vinfo.xres);
    printf("yres          = %u\n", vinfo.yres);
    printf("xres_virtual  = %u\n", vinfo.xres_virtual);
    printf("yres_virtual  = %u\n", vinfo.yres_virtual);
    printf("xoffset       = %u\n", vinfo.xoffset);
    printf("yoffset       = %u\n", vinfo.yoffset);
    printf("bits_per_pixel= %u\n", vinfo.bits_per_pixel);
    printf("line_length   = %u\n", finfo.line_length);
    printf("smem_len      = %u\n", finfo.smem_len);
    printf("visual        = %u\n", finfo.visual);
    printf("id            = %s\n", finfo.id);

    close(fd);
    return 0;
}
