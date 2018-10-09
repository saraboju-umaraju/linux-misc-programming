#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/user.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
 
#ifndef PAGE_SHIFT
	#define PAGE_SHIFT 12
#endif
#ifndef PAGE_SIZE
	#define PAGE_SIZE (1UL << PAGE_SHIFT)
#endif
#ifndef PAGE_MASK
	#define PAGE_MASK (~(PAGE_SIZE - 1))
#endif


void parse( struct fb_var_screeninfo *var )
{

    printf("  %d\n",var->xres);
    printf("  %d\n",var->yres);
    printf("  %d\n",var->xres_virtual);
    printf("  %d\n",var->yres_virtual);
    printf("  %d\n",var->xoffset);
    printf("  %d\n",var->xres);
    printf("  %d\n",var->yoffset);
    printf("  bits per pixel %d\n",var->bits_per_pixel);
    printf("  %d\n",var->grayscale);
    printf("  %d\n",var->nonstd);
    printf("  %d\n",var->activate);
    printf("  height %d\n",var->height);
    printf("  width %d\n",var->width);
    printf("  %d\n",var->accel_flags);
    printf("  %d\n",var->pixclock);
    printf("  %d\n",var->left_margin);
    printf("  %d\n",var->right_margin);
    printf("  %d\n",var->upper_margin);
    printf("  %d\n",var->lower_margin);
    printf("  %d\n",var->hsync_len);
    printf("  %d\n",var->vsync_len);
    printf("  %d\n",var->sync);
    printf("  %d\n",var->vmode);
    printf("  %d\n",var->rotate);
    printf("  %d\n",var->colorspace);
} 
int fb;
struct fb_var_screeninfo fb_var;
struct fb_var_screeninfo fb_screen;
struct fb_fix_screeninfo fb_fix;
unsigned char *fb_mem = NULL;
unsigned char *fb_screenMem = NULL;
 
int fb_init(char *device) {
 
	int fb_mem_offset;
 
	// get current settings (which we have to restore)
	if (-1 == (fb = open(device, O_RDWR))) {
		fprintf(stderr, "Open %s: %s.\n", device, strerror(errno));
		return 0;
	}
	if (-1 == ioctl(fb, FBIOGET_VSCREENINFO, &fb_var)) {
		fprintf(stderr, "Ioctl FBIOGET_VSCREENINFO error.\n");
		return 0;
	}
	parse(&fb_var);
	if (-1 == ioctl(fb, FBIOGET_FSCREENINFO, &fb_fix)) {
		fprintf(stderr, "Ioctl FBIOGET_FSCREENINFO error.\n");
		return 0;
	}
	if (fb_fix.type != FB_TYPE_PACKED_PIXELS) {
		fprintf(stderr, "Can handle only packed pixel frame buffers.\n");
		goto err;
	}
 
	fb_mem_offset = (unsigned long)(fb_fix.smem_start) & (~PAGE_MASK);
	fb_mem = mmap(NULL, fb_fix.smem_len + fb_mem_offset, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	if (-1L == (long)fb_mem) {
		fprintf(stderr, "Mmap error.\n");
		goto err;
	}
	// move viewport to upper left corner
	if (fb_var.xoffset != 0 || fb_var.yoffset != 0) {
		fb_var.xoffset = 0;
		fb_var.yoffset = 0;
		if (-1 == ioctl(fb, FBIOPAN_DISPLAY, &fb_var)) {
			fprintf(stderr, "Ioctl FBIOPAN_DISPLAY error.\n");
			perror("");


			munmap(fb_mem, fb_fix.smem_len);
			goto err;
		}
	}

	fb_screen = fb_var;
	fb_screen.xoffset = 0;
	fb_screen.yoffset = fb_var.yres - 1;
	if (-1 == ioctl(fb, FBIOPAN_DISPLAY, &fb_screen)) {

			printf("\n\n");

			fprintf(stderr, "Ioctl FBIOPAN_DISPLAY error.\n");
			perror("");
			munmap(fb_mem, fb_fix.smem_len);
			goto err;
	}
 
	fb_screenMem = fb_mem + fb_mem_offset + (fb_var.yres * fb_var.xres * (fb_var.bits_per_pixel / 8));
	return 1;
 
err:
	if (-1 == ioctl(fb, FBIOPUT_VSCREENINFO, &fb_var))
		fprintf(stderr, "Ioctl FBIOPUT_VSCREENINFO error.\n");
	if (-1 == ioctl(fb, FBIOGET_FSCREENINFO, &fb_fix))
		fprintf(stderr, "Ioctl FBIOGET_FSCREENINFO.\n");
	return 0;
}
 
void fb_cleanup(void) {
 
	if (-1 == ioctl(fb, FBIOPUT_VSCREENINFO, &fb_var))
		fprintf(stderr, "Ioctl FBIOPUT_VSCREENINFO error.\n");
	if (-1 == ioctl(fb, FBIOGET_FSCREENINFO, &fb_fix))
		fprintf(stderr, "Ioctl FBIOGET_FSCREENINFO.\n");
	munmap(fb_mem, fb_fix.smem_len);
	close(fb);
}
 
void main(void) {
 
	if (!fb_init("/dev/fb0"))
		exit(1);
 
//	<now you can write directly to fb_screenMem>
 
	fb_cleanup();
}
