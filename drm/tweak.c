typedef  unsigned int uint32_t;
typedef  unsigned long uint64_t;
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)

/*
struct drm_mode_card_res {
	__u64 fb_id_ptr;
	__u64 crtc_id_ptr;
	__u64 connector_id_ptr;
	__u64 encoder_id_ptr;
	__u32 count_fbs;
	__u32 count_crtcs;
	__u32 count_connectors;
	__u32 count_encoders;
	__u32 min_width, max_width;
	__u32 min_height, max_height;
};
*/
int main (int argc, char *argv[])
{

	int fd = -1;
	
	fd = open("/dev/dri/card0", O_RDWR);
	
	if (unlikely( fd < 0 )) {
			perror("opening dev:");
			exit(1);
	}

	if (ioctl(fd, DRM_IOCTL_SET_MASTER, 0)) {
			perror("DRM_IOCTL_SET_MASTER:");
			exit(1);
	}

	struct drm_mode_card_res cons;
	
	memset(&cons, 0, sizeof(cons));
	
	if (ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &cons)) {
			perror("DRM_IOCTL_MODE_GETRESOURCES:");
			exit(1);
	}
	
	printf (" number of connectors are %d\n", cons.count_connectors);


	if (cons.count_fbs) {
        cons.fb_id_ptr = malloc(cons.count_fbs*sizeof(uint32_t));
        if (!cons.fb_id_ptr) {
			perror("allocation of memory");
			exit(1);
		}
    }
    if (cons.count_crtcs) {
        cons.crtc_id_ptr = malloc(cons.count_crtcs*sizeof(uint32_t));
        if (!cons.crtc_id_ptr) {
			perror("allocation of memory");
			exit(1);
		}
    }
    if (cons.count_connectors) {
        cons.connector_id_ptr = malloc(cons.count_connectors*sizeof(uint32_t));
        if (!cons.connector_id_ptr) {
			perror("allocation of memory");
			exit(1);
		}
    }
    if (cons.count_encoders) {
        cons.encoder_id_ptr = malloc(cons.count_encoders*sizeof(uint32_t));
        if (!cons.encoder_id_ptr) {
			perror("allocation of memory");
			exit(1);
		}
    }
	
	if (ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &cons)) {
			perror("DRM_IOCTL_MODE_GETRESOURCES:");
			exit(1);
	}
	
	int l = 0;
	for(; l< cons.count_connectors; l++) {

			struct drm_mode_modeinfo conn_mode_buf[20]={0};

			uint64_t	conn_prop_buf[20]={0},
						conn_propval_buf[20]={0},
						conn_enc_buf[20]={0};

			struct drm_mode_get_connector conn={0};

			conn.connector_id=res_conn_buf[i];

			ioctl(dri_fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);	//get connector resource counts
			conn.modes_ptr=(uint64_t)conn_mode_buf;
			conn.props_ptr=(uint64_t)conn_prop_buf;
			conn.prop_values_ptr=(uint64_t)conn_propval_buf;
			conn.encoders_ptr=(uint64_t)conn_enc_buf;
			ioctl(dri_fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);	//get connector resources

			//Check if the connector is OK to use (connected to something)
			if (conn.count_encoders<1 || conn.count_modes<1 || !conn.encoder_id || !conn.connection)
			{
					printf("Not connected\n");
					continue;
			}

			//------------------------------------------------------------------------------
			//Creating a dumb buffer
			//------------------------------------------------------------------------------
			struct drm_mode_create_dumb create_dumb={0};
			struct drm_mode_map_dumb map_dumb={0};
			struct drm_mode_fb_cmd cmd_dumb={0};

			//If we create the buffer later, we can get the size of the screen first.
			//This must be a valid mode, so it's probably best to do this after we find
			//a valid crtc with modes.
			create_dumb.width = conn_mode_buf[0].hdisplay;
			create_dumb.height = conn_mode_buf[0].vdisplay;
			create_dumb.bpp = 32;
			create_dumb.flags = 0;
			create_dumb.pitch = 0;
			create_dumb.size = 0;
			create_dumb.handle = 0;
			ioctl(dri_fd, DRM_IOCTL_MODE_CREATE_DUMB, &create_dumb);

			cmd_dumb.width=create_dumb.width;
			cmd_dumb.height=create_dumb.height;
			cmd_dumb.bpp=create_dumb.bpp;
			cmd_dumb.pitch=create_dumb.pitch;
			cmd_dumb.depth=24;
			cmd_dumb.handle=create_dumb.handle;
			ioctl(dri_fd,DRM_IOCTL_MODE_ADDFB,&cmd_dumb);

			map_dumb.handle=create_dumb.handle;
			ioctl(dri_fd,DRM_IOCTL_MODE_MAP_DUMB,&map_dumb);

			fb_base[i] = mmap(0, create_dumb.size, PROT_READ | PROT_WRITE, MAP_SHARED, dri_fd, map_dumb.offset);
			fb_w[i]=create_dumb.width;
			fb_h[i]=create_dumb.height;

			//------------------------------------------------------------------------------
			//Kernel Mode Setting (KMS)
			//------------------------------------------------------------------------------

			printf("%d : mode: %d, prop: %d, enc: %d\n",conn.connection,conn.count_modes,conn.count_props,conn.count_encoders);
			printf("modes: %dx%d FB: %d\n",conn_mode_buf[0].hdisplay,conn_mode_buf[0].vdisplay,fb_base[i]);

			struct drm_mode_get_encoder enc={0};

			enc.encoder_id=conn.encoder_id;
			ioctl(dri_fd, DRM_IOCTL_MODE_GETENCODER, &enc);	//get encoder

			struct drm_mode_crtc crtc={0};

			crtc.crtc_id=enc.crtc_id;
			ioctl(dri_fd, DRM_IOCTL_MODE_GETCRTC, &crtc);

			crtc.fb_id=cmd_dumb.fb_id;
			crtc.set_connectors_ptr=(uint64_t)&res_conn_buf[i];
			crtc.count_connectors=1;
			crtc.mode=conn_mode_buf[0];
			crtc.mode_valid=1;
			ioctl(dri_fd, DRM_IOCTL_MODE_SETCRTC, &crtc);
	}

	
	 if (ioctl(fd, DRM_IOCTL_DROP_MASTER, 0)) {
            perror("DRM_IOCTL_DROP_MASTER:");
            exit(1);
    }

	close (fd);
	return 0;
}

