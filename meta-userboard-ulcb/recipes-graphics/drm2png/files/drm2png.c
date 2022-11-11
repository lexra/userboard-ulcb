#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>
#include <assert.h>
#include <regex.h>

#ifdef __cplusplus
extern  "C" {
#endif // __cplusplus

#include <drm.h>
#include <xf86drmMode.h>
#include <xf86drm.h>
#include <drm_fourcc.h>
#include <png.h>
#include <zlib.h>

#ifdef __cplusplus
}
#endif // __cplusplus

#include <pthread.h>
#include <setjmp.h>

#define DRI_DRM_DEVICE    "/dev/dri/card0"
#define MAX_FBS 16

#define MODESET_PREPARE(fd)    \
    do { if (0 != modeset_prepare(fd))    printf("(%s %d) modeset_prepare() fail\n", __FILE__, __LINE__), close(fd), fd = -1, exit(1); } while(0)

struct modeset_dev {
    struct modeset_dev *next;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t size;
    uint32_t handle;
    uint8_t *map;
    struct _drmModeModeInfo mode;
    uint32_t fb;
    uint32_t conn;
    uint32_t crtc;
    struct _drmModeCrtc *saved_crtc;
    int dma_buf_fd;
};
static struct modeset_dev *modeset_list = NULL;

static char *stripCrlf(char *line) {
    int len = 0;

    if(line == 0)        return 0;
    len = strlen(line);
    if(len > 1000)        return line;
    if(len <= 0)        return line;
    if (line[len - 1] == '\n' || line[len - 1] == '\r')
        line[len - 1] = 0;
    len = strlen(line);
    if (len > 0)
        if (line[len - 1] == '\n' || line[len - 1] == '\r')
            line[len - 1] = 0;
    return line;
}
static char *FileName(const char *path) {
    int i, j = -1, len;
    char szTmp[1024] = {0};
    static char name[256] = {0};

    if(!path)                        return 0;
    strcpy(szTmp, path);
    len = strlen(szTmp);
    if(0 >= len || 255 <= len)        return 0;
    if(szTmp[len - 1] == '/') {
        szTmp[len - 1] = 0;
        len = strlen(szTmp);
    }
    for (i = 0; i < len; i++) {
        if(szTmp[i] == '/')
            j = i;
    }
    strcpy(name, &szTmp[j + 1]);
    return name;
}
static char *FileExtension(const char *path) {
    int i, j = -1, len;
    char szTmp[1024] = {0};
    static char ext[256] = {0};
    char *name;

    if(!path)        return 0;
    name = FileName(path);
    if(!name)        return 0;
    strcpy(szTmp, name);
    len = strlen(szTmp);
    if(0 > len || 255 <= len)
        return 0;
    for (i = 0; i < len; i++) {
        if(szTmp[i] == '.')
            j = i;
    }
    if (-1 == j || 0 == j) {
        ext[0] = 0;
        return ext;
    }
    strcpy(ext, &szTmp[j + 1]);
    return ext;
}
static int modeset_find_crtc(int fd, drmModeRes *res, const struct _drmModeConnector *conn, struct modeset_dev *dev) {
    struct _drmModeEncoder *enc = 0;
    unsigned int i, j;
    int32_t crtc;
    struct modeset_dev *iter;

    if (0 == res)    return -ENOENT;
    if (0 == conn)    return -ENOENT;
    if (0 == dev)    return -ENOENT;
    // first try the currently conected encoder+crtc 
    if (conn->encoder_id)
        enc = drmModeGetEncoder(fd, conn->encoder_id);
    if (enc) {
        if (enc->crtc_id) {
            crtc = enc->crtc_id;
            for (iter = modeset_list; iter; iter = iter->next) {
                if (iter->crtc == crtc) {
                    crtc = -1;
                    break;
                }
            }
            if (crtc >= 0) {
                drmModeFreeEncoder(enc);
                dev->crtc = crtc;
                return 0;
            }
        }
        drmModeFreeEncoder(enc);
    }
    // If the connector is not currently bound to an encoder or if the
    // encoder+crtc is already used by another connector (actually unlikely
    // but lets be safe), iterate all other available encoders to find a matching CRTC. 
    for (i = 0; i < conn->count_encoders; ++i) {
        enc = drmModeGetEncoder(fd, conn->encoders[i]);
        if (!enc) {
            printf("(%s %d) cannot retrieve encoder %u:%u (%d): %m\n", __FILE__, __LINE__, i, conn->encoders[i], errno);
            continue;
        }
        // iterate all global CRTCs
        for (j = 0; j < res->count_crtcs; ++j) {
            // check whether this CRTC works with the encoder
            if (!(enc->possible_crtcs & (1 << j)))
                continue;
            // check that no other device already uses this CRTC
            crtc = res->crtcs[j];
            for (iter = modeset_list; iter; iter = iter->next) {
                if (iter->crtc == crtc) {
                    crtc = -1;
                    break;
                }
            }
            // we have found a CRTC, so save it and return
            if (crtc >= 0) {
                drmModeFreeEncoder(enc);
                dev->crtc = crtc;
                return 0;
            }
        }
        drmModeFreeEncoder(enc);
    }
    printf("(%s %d) cannot find suitable CRTC for connector %u\n", __FILE__, __LINE__, conn->connector_id);
    return -ENOENT;
}
static int modeset_setup_dev(int fd, drmModeRes *res, const struct _drmModeConnector *conn, struct modeset_dev *dev) {
	int ret, found = 0, count_fbs = 0, dma_buf_fd, I, len, i, j;
	drmModeFBPtr fb;
	drmModePlaneResPtr planes;
	drmModePlanePtr plane;
	uint32_t fbs[MAX_FBS] = {0};
	regex_t Regx;
	char Pattern[128] = {0}, line[128] = {0}, num[64] = {0};
	regmatch_t Match[64];
	FILE *f = 0;
	struct stat lbuf;
	struct _drmModeModeInfo *mode = 0;
	uint16_t w = 0, h = 0;

	if (0 == res)		return -ENOENT;
	if (0 == conn)		return -ENOENT;
	if (0 == dev)		return -ENOENT;
	// check if a monitor is connected
	if (DRM_MODE_CONNECTED != conn->connection) {
		printf("(%s %d) ignoring unused connector %u\n", __FILE__, __LINE__, conn->connector_id);
		return -ENOENT;
	}
	// check if there is at least one valid mode
	if (0 == conn->count_modes) {
		printf("(%s %d) no valid mode for connector %u\n", __FILE__, __LINE__, conn->connector_id);
		return -EFAULT;
    	}
	strcpy(Pattern, "^([0-9]{1,})x([0-9]{1,})p([0-9]{1,})$"), regcomp(&Regx, Pattern, REG_EXTENDED);
	if (0 == lstat("/sys/class/drm/card0-HDMI-A-1/mode", &lbuf))
		f = fopen("/sys/class/drm/card0-HDMI-A-1/mode", "r");
	if (0 != f) {
		fgets(line, sizeof(line), f);
		stripCrlf(line);
		if (0 == regexec(&Regx, line, sizeof(Match) / sizeof(regmatch_t), Match, 0)) {
			I = 1, len = Match[I].rm_eo - Match[I].rm_so, memcpy(num, line + Match[I].rm_so, len), num[len] = 0;
			w = atoi(num);
			I = 2, len = Match[I].rm_eo - Match[I].rm_so, memcpy(num, line + Match[I].rm_so, len), num[len] = 0;
			h = atoi(num);
		}
		fclose(f), f = 0;
	}
	regfree(&Regx);
	mode = &conn->modes[0];
	if (0 == w && 0 == h) {
		//printf("(%s %d) 0 == w && 0 == h\n", __FILE__, __LINE__);
	} else {
		for (i = 0; i < conn->count_modes; i++)
		if (w == conn->modes[i].hdisplay && h == conn->modes[i].vdisplay) {
			mode = &conn->modes[i];
			break;
		}
	}
	memcpy(&dev->mode, mode, sizeof(dev->mode));
	dev->width = mode->hdisplay;
	dev->height = mode->vdisplay;
	printf("mode for connector %u is %ux%u\n", conn->connector_id, dev->width, dev->height);
	// find a crtc for this connector
	ret = modeset_find_crtc(fd, res, conn, dev);
	if (0 != ret) {
		printf("(%s %d) no valid crtc for connector %u\n", __FILE__, __LINE__, conn->connector_id);
		return ret;
	}
	planes = drmModeGetPlaneResources(fd);
	if (planes) {
		for (i = 0; i < planes->count_planes; i++) {
			plane = drmModeGetPlane(fd, planes->planes[i]);
			if (0 == plane)		continue;
			if (plane->fb_id) {
				found = 0;
				for (j = 0; j < count_fbs; j++) {
					if (fbs[j] == plane->fb_id) {
						found = 1;
						break;
					}
				}
				if (0 == found) {
					if (count_fbs == MAX_FBS) {
					} else
						fbs[count_fbs++] = plane->fb_id;
				}
			}
			drmModeFreePlane(plane);
		}
		drmModeFreePlaneResources(planes);
	}
	dev->fb = fbs[0];    // plane->fb_id
	fb = drmModeGetFB(fd, fbs[0]);
	if (0 == fb) {
		printf("(%s %d) drmModeGetFB() == 0\n", __FILE__, __LINE__);
		return -EFAULT;
	}
	dev->height = fb->height, dev->stride = fb->pitch, dev->size = fb->height * fb->pitch, dev->handle = fb->handle;
	drmPrimeHandleToFD(fd, fb->handle, O_RDONLY, &dev->dma_buf_fd);
	dev->size = fb->height * fb->pitch;
	dev->map = mmap(0, dev->size, PROT_READ, MAP_SHARED, dev->dma_buf_fd, 0);
	if (MAP_FAILED == dev->map) {
		printf("(%s %d) MAP_FAILED\n", __FILE__, __LINE__);
		drmModeFreeFB(fb), fb = 0;
		return -EFAULT;
	}
	drmModeFreeFB(fb);
	return 0;
}
static int modeset_prepare(int fd) {
    struct _drmModeRes *res;
    struct _drmModeConnector *conn;
    struct modeset_dev *dev;
    int i, ret;

    // retrieve resources
    res = drmModeGetResources(fd);
    if (0 == res) {
        printf("(%s %d) cannot retrieve DRM resources (%d): %m\n", __FILE__, __LINE__, errno);
        return -errno;
    }
    // iterate all connectors
    for (i = 0; i < res->count_connectors; ++i) {
        // get information for each connector
        conn = drmModeGetConnector(fd, res->connectors[i]);
        if (!conn) {
            printf("(%s %d) cannot retrieve DRM connector %u:%u (%d): %m\n", __FILE__, __LINE__, i, res->connectors[i], errno);
            continue;
        }
        // create a device structure
        dev = malloc(sizeof(*dev));
        memset(dev, 0, sizeof(*dev));
        dev->conn = conn->connector_id;
        // call helper function to prepare this connector
        ret = modeset_setup_dev(fd, res, conn, dev);
        if (ret) {
            if (ret != -ENOENT) {
                errno = -ret;
                printf("(%s %d) cannot setup device for connector %u:%u (%d): %m\n", __FILE__, __LINE__, i, res->connectors[i], errno);
            }
            free(dev);
            drmModeFreeConnector(conn);
            continue;
        }
        // free connector data and link device into global list
        drmModeFreeConnector(conn);
        dev->next = modeset_list;
        modeset_list = dev;
    }
    // free resources again
    drmModeFreeResources(res);
    return 0;
}
static void modeset_cleanup(int fd) {
    struct modeset_dev *iter;
    struct drm_mode_destroy_dumb dreq;

    while (modeset_list) {
        // remove from global list
        iter = modeset_list;
        modeset_list = iter->next;
        // restore saved CRTC configuration
        //drmModeSetCrtc(fd, iter->saved_crtc->crtc_id,iter->saved_crtc->buffer_id, 
        //    iter->saved_crtc->x, iter->saved_crtc->y, &iter->conn, 1, &iter->saved_crtc->mode);
        drmModeFreeCrtc(iter->saved_crtc);
        // unmap buffer
        munmap(iter->map, iter->size), free(iter);
    }
}
static int BGRA2RGBA(png_bytep buffer, int length) {
    uint32_t *p;
    uint8_t swp[4] = {0}, tmp = 0;
    int i = 0;

    if (0 == length)                    return -1;
    if (0 != (length % 4))                return -1;
    if (0 == (p = (uint32_t *)buffer))    return -1;
    for (; i < length / 4; i++) {
        memcpy(swp, p, 4);
        tmp = swp[0], swp[0] = swp[2], swp[2] = tmp;
        memcpy(p, swp, 4);
        p++;
    }
    return 0;
}
int main(int argc, char*argv[]) {
    char path[256] = {0};
    const char *card = DRI_DRM_DEVICE;
    int fd, i, N = 0, row_stride, y, color_type = PNG_COLOR_TYPE_RGB;
    struct _drmStats stats;
    struct modeset_dev *iter, *front;
    FILE *outfile = 0;
    png_bytep png_buffer = 0, png_bufferp = 0;
    png_structp png_ptr;
    png_infop info_ptr;

    if(2 != argc)                    printf("(%s %d) usage: drm2png [PNG-FILE]\n", __FILE__, __LINE__), exit(1);
    if(0 != strncasecmp("png", FileExtension(argv[1]), 3))
        printf("(%s %d) File extension not png. \n", __FILE__, __LINE__), exit(1);
    strcpy(path, argv[1]);
    if (!drmAvailable())            printf("(%s %d) DRM driver is currently not loaded\n", __FILE__, __LINE__), exit(1);
    assert(-1 != (fd = open(card, O_RDWR | O_CLOEXEC)));
    drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1), drmGetStats(fd, &stats);
    // prepare all connectors and CRTCs
    MODESET_PREPARE(fd);
    // perform actual modesetting on each found connector+CRTC
    for (iter = modeset_list; iter; iter = iter->next)        iter->saved_crtc = drmModeGetCrtc(fd, iter->crtc);
    N = 0;
    for (iter = modeset_list; iter; iter = iter->next)        N++;
    if (0 == N)                        printf("(%s %d) 0 == N\n", __FILE__, __LINE__), close(fd), fd = -1, exit(1);
    front = modeset_list;
    assert(0 != (png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)));
    assert(0 != (info_ptr = png_create_info_struct(png_ptr)));
    assert(0 == setjmp(png_jmpbuf(png_ptr)));
    assert(0 != (outfile = fopen(path, "wb")));
    png_init_io(png_ptr, outfile);
    if (4 == (front->stride / front->width))    color_type = PNG_COLOR_TYPE_RGBA;
    png_set_IHDR(png_ptr, info_ptr, front->width, front->height, 8, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);
    png_buffer = (png_bytep)malloc(front->size); pthread_cleanup_push((void (*)(void *))free, (void *)png_buffer); memcpy(png_buffer, front->map, front->size);
    if (4 == (front->stride / front->width))
        BGRA2RGBA(png_buffer, front->size);
    png_bufferp = png_buffer, row_stride = front->stride;
    for (y = 0; y < front->height; y++)
        png_write_row(png_ptr, png_bufferp), png_bufferp += row_stride;
    pthread_cleanup_pop(1);
    png_write_end(png_ptr, NULL), png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(outfile);
    modeset_cleanup(fd), close(fd), fd = -1;
    printf("'%s' successfully saved. \n", path);
    return 0;
}
