/*
￭ command-line I2C routines

for S in `seq 0 127`; do ./i2c_read_data -d /dev/i2c-4 -s ${S} -r 0x00 -l 0x08; done
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <netinet/tcp.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <asm/errno.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#if 0
int i2c_write_data(int fd_i2c, int bus_adr, int sub_adr, const unsigned char *data, int length) {
	int res;
	char buf[256];
	int i;

	if (0 == data || 0 >= fd_i2c)						return -1;
	res = ioctl(fd_i2c, I2C_SLAVE_FORCE, bus_adr);
	if(res < 0)											return -1;
	buf[0] = sub_adr;
	for(i = 1; i <= length; i++)
		buf[i] = data[i -1];
	if(write(fd_i2c, buf, length + 1) != (length + 1))	return -1;
	return length;
}
int i2c_read_data(int fd_i2c, int bus_adr, int sub_adr, unsigned char *data, int length) {
	int res;

	if (0 == data || 0 >= fd_i2c)						return -1;
	res = ioctl(fd_i2c, I2C_SLAVE_FORCE, bus_adr);
	if(res < 0)											return -1;
	if(write(fd_i2c, &sub_adr, 1) != 1)					return -1;
	return read(fd_i2c, data, length);
}
#else
static int i2c_write_data(int fd_i2c, int chip_addr, int offset, const unsigned char *data, int length) {
	int res = -1;
	unsigned char buf[1025] = {0};
	struct i2c_msg msg[2] = {0};
	struct i2c_rdwr_ioctl_data queue = {0};

	if (0 == data || 0 >= fd_i2c)						return -1;
	if (length <= 0)									return -1;
	if (length > 1024)									return -1;
	msg[0].addr = chip_addr;
	msg[0].len = (length + 1);
	msg[0].buf = &buf[0];
	buf[0] = (unsigned char)offset;
	if (data && length > 0)		memcpy(&buf[1], data, length);
	queue.msgs = msg, queue.nmsgs = 1;
	res = ioctl(fd_i2c, I2C_RDWR, &queue);
	if (-1 == res)	printf("i2c_write_data(0x%02x) ERROR\n", chip_addr);
	return res;
}
static int i2c_read_data(int fd_i2c, int chip_addr, int offset, unsigned char *data, int length) {
	int res = -1;
	unsigned char buf[1025] = {0};
	struct i2c_msg msg[2] = {0};
	struct i2c_rdwr_ioctl_data queue = {0};

	if (0 == data || 0 >= fd_i2c)						return -1;
	if (length <= 0)									return -1;
	if (length > 1024)									return -1;
	buf[0] = (unsigned char)offset;
	msg[0].addr = chip_addr;
	msg[0].len = 1;
	msg[0].buf = &buf[0];
	msg[1].addr = chip_addr;
	msg[1].flags = I2C_M_RD | I2C_M_NOSTART;
	msg[1].len = length;
	msg[1].buf = &buf[1];
	queue.msgs = msg, queue.nmsgs = 2;
	res = ioctl(fd_i2c, I2C_RDWR, &queue);
	if (-1 == res) {
		printf("i2c_read_data(0x%02x) ERROR\n", chip_addr);
		return -1;
	}
	if (data && length > 0)		memcpy(data, &buf[1], length);
	return res;
}
#endif // 0

static void PrintUsage(void) {
	char file[64];
	int i = 0;

	memset(file, 0, sizeof(file));
	strcpy(file, __FILE__);
	for (; i < 128; i++)		if ('.' == file[i])	file[i] = 0;
	printf("Usage: %s -d /dev/i2c-0 -s [CHIP-ADDR] -r [REGISTER] -l [LEN]\n", file);
	exit(0);
}
int main(int argc, char *argv[]) {
	int res, c, i;
	int slave = 0x5d, reg = 0x10, len = 1;
	char dev[64] ={0};
	unsigned char data[1024] ={0};
	static int fd_i2c = -1;
	struct stat lbuf;
	int ten_bit = 0;

	setbuf(stdout, 0);
	strcpy(dev, "/dev/i2c-5");
	if (argc < 2)							PrintUsage();
	while ((c = getopt(argc, argv, "t:T:s:S:d:D:r:R:l:L:")) != -1) {
		switch (c) {
		default:
			PrintUsage();
			break;
		case '?':
			PrintUsage();
			break;
		case 'T':
		case 't':
			if (1 != strlen(optarg))		printf("PARAM [TEN-BIT] ERROR \n\n"), PrintUsage();
			if ('0' == optarg[0])			ten_bit = 0;
			else if ('1' == optarg[0])		ten_bit = 1;
			else							printf("PARAM [TEN-BIT] ERROR \n\n"), PrintUsage();
			break;
		case 'D':
		case 'd':
			strcpy(dev, optarg);
			break;
		case 'L':
		case 'l':
			if (strlen(optarg) > 4)			printf("PARAM [LEN] ERROR \n\n"), PrintUsage();
			if ('0' == optarg[0] && ('X' == optarg[1] || 'x' == optarg[1])) {
				if ('X' == optarg[1])		res = sscanf(optarg, "%X", &len);
				else						res = sscanf(optarg, "%x", &len);
				if (-1 == res)				printf("PARAM [LEN] ERROR \n\n"), PrintUsage();
			}
			else							len = atoi(optarg);
			len &= 0XFF;
			break;
		case 'R':
		case 'r':
			if (strlen(optarg) > 4)			printf("PARAM [REG] ERROR \n\n"), PrintUsage();
			if ('0' == optarg[0] && ('X' == optarg[1] || 'x' == optarg[1])) {
				if ('X' == optarg[1])		res = sscanf(optarg, "%X", &reg);
				else						res = sscanf(optarg, "%x", &reg);
				if (-1 == res)				printf("PARAM [REG] ERROR \n\n"), PrintUsage();
			}
			else							reg = atoi(optarg);
			reg &= 0XFF;
			break;
		case 'S':
		case 's':
			if (strlen(optarg) > 4)			printf("PARAM [SLAVE] ERROR \n\n"), PrintUsage();
			if ('0' == optarg[0] && ('X' == optarg[1] || 'x' == optarg[1])) {
				if ('X' == optarg[1])		res = sscanf(optarg, "%X", &slave);
				else						res = sscanf(optarg, "%x", &slave);
				if (-1 == res)				printf("PARAM [SLAVE] ERROR \n\n"), PrintUsage();
			}
			else							slave = atoi(optarg);
			if (ten_bit) 					slave &= 0x01ff;
			else							slave &= 0x007f;
			break;
		}
	}
	if (len > 256 - reg)					len = 256 - reg;
	if(0 >= len)							printf("PARAM 0 >= LEN \n\n"), PrintUsage();
	if(0 > stat(dev, &lbuf))				printf("PARAM [DEV] ERROR, FILE NOT FOUND \n\n"), PrintUsage();
	if(!S_ISCHR(lbuf.st_mode))				printf("PARAM [DEV] ERROR, NOT CHARACTOR DEV \n\n"), PrintUsage();
	fd_i2c = open(dev, O_RDWR);
	if (fd_i2c < 0)							printf("OPEN [%s] ERROR \n\n", dev), exit(1);
	res = ioctl(fd_i2c, I2C_TENBIT, ten_bit);
	if (-1 == i2c_read_data(fd_i2c, slave, reg, data, len))			close(fd_i2c), exit(1);
	close(fd_i2c);
	for (i = 0; i < len; i++) {
		if (0 == i) {
			if (ten_bit)					printf("\nSLAVE-ADDR=0x%03x\n==============================\n", slave);
			else							printf("\nSLAVE-ADDR=0x%02x\n==============================\n", slave);
		}
		printf("[%02x]=0x%02x\n", reg + i, data[i]);
	}
	return 0;
}

