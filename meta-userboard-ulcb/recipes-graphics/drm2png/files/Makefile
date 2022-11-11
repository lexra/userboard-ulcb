DEPENDENCIES = libdrm libpng
CFLAGS = -Irecipe-sysroot/usr/include/drm
LDFLAGS += -ldrm -lpng16 -lz -lpthread


all: drm2png ntpc fbgrab i2c_read_data

i2c_read_data: i2c_read_data.o
	$(CC) $@.o -o $@ $(LDFLAGS)

fbgrab: fbgrab.o
	$(CC) $@.o -o $@ $(LDFLAGS)

ntpc: ntpc.o
	$(CC) $@.o -o $@ $(LDFLAGS)

drm2png: drm2png.o
	$(CC) $@.o -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o drm2png ntpc fbgrab i2c_read_data
