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
#include <sys/time.h>

#define TIMEOUT         10              // seconds
#define JAN_1970        0x83aa7e80      // 2208988800 1970 - 1900 in seconds
#define NTP_PORT        (123)
typedef uint32_t __u32;

/* How to multiply by 4294.967296 quickly (and not quite exactly)
 * without using floating point or greater than 32-bit integers.
 * If you want to fix the last 12 microseconds of error, add in
 * (2911*(x))>>28)
 */
#define NTPFRAC(x) ( 4294*(x) + ( (1981*(x))>>11 ) )

/* The reverse of the above, needed if we want to set our microsecond
 * clock (via settimeofday) based on the incoming time in NTP format.
 * Basically exact.
 */
#define USEC(x) ( ( (x) >> 12 ) - 759 * ( ( ( (x) >> 10 ) + 32768 ) >> 16 ) )

// you'd better be root, or ntpclient will crash!
int settime(uint32_t *data) {
    struct timeval tv_set;
    unsigned int coarse;
    unsigned int fine;

#define Data(i) ntohl(((uint32_t *)data)[i])
    coarse = Data(10);
    fine   = Data(11);
#undef Data

    // it would be even better to subtract half the slop
    tv_set.tv_sec  = coarse - JAN_1970;
    // divide xmttime.fine by 4294.967296
    tv_set.tv_usec = USEC(fine);
    if (settimeofday(&tv_set,NULL)<0)
        return -1;
    return 0;
}

void send_packet(int usd) {
    __u32 data[12];
    struct timeval now;
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

    if (sizeof(data) != 48) {
        fprintf(stderr,"size error\n");
        return;
    }
    bzero((char *) data,sizeof(data));
    data[0] = htonl (
        ( LI << 30 ) | ( VN << 27 ) | ( MODE << 24 ) |
        ( STRATUM << 16) | ( POLL << 8 ) | ( PREC & 0xff ) );
    data[1] = htonl(1<<16);  /* Root Delay (seconds) */
    data[2] = htonl(1<<16);  /* Root Dispersion (seconds) */
    gettimeofday(&now,NULL);
    data[10] = htonl(now.tv_sec + JAN_1970); // Transmit Timestamp coarse
    data[11] = htonl(NTPFRAC(now.tv_usec));  // Transmit Timestamp fine
    send(usd,data,48,0);
}

int ntpHandle(int usd) {
    static uint32_t incoming_word[20];
    #define incoming ((char *) incoming_word)
    #define sizeof_incoming (sizeof(incoming_word)*sizeof(uint32_t))

    fd_set fds;
    struct sockaddr sa_xmit;
    int sa_xmit_len=sizeof(sa_xmit);
    int i, pack_len;
    struct timeval to;

    send_packet(usd);
    to.tv_sec=TIMEOUT;
    to.tv_usec=0;
    for (;;) {
        FD_ZERO(&fds);
        FD_SET(usd,&fds);
        i=select(usd+1,&fds,NULL,NULL,&to);  // Wait on read or error
        if ((i!=1)||(!FD_ISSET(usd,&fds))) {
            if (errno==EINTR) {
                fprintf(stderr,"interrupt by signal\n");
                continue;
            }
            if (i<0) {
                fprintf(stderr,"select error\n");
                return -1;
            }
            fprintf(stderr,"timeout\n");
            return -1;
        }
        pack_len=recvfrom(usd,incoming,sizeof_incoming,0,&sa_xmit,&sa_xmit_len);
        if (pack_len<0) {
            fprintf(stderr,"recvfrom error\n");
            return -1;
        //} else if (pack_len >0 && (unsigned)pack_len)
        } else if (pack_len > 0) {
            fprintf(stderr,"settime\n");
            return settime(incoming_word);
        }
        return -1;
    }
    return -1;
}

int ntpclient(char *hostname) {
    struct hostent *ntpserver;
    struct sockaddr_in sa_dest;
    struct sockaddr_in sa_rcvr;
    int usd;  // socket
    int err;

    // Startup sequence
    if ((usd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1) {
        fprintf(stderr,"could not open the sockey\n");
        return -1;
    }
    // setup receive
    bzero((char *) &sa_rcvr, sizeof(sa_rcvr));
    sa_rcvr.sin_family=AF_INET;
    sa_rcvr.sin_addr.s_addr=htonl(INADDR_ANY);
    //sa_rcvr.sin_port=htons(0);  // udp_local_port, default of 0 means kernel chooses
    sa_rcvr.sin_port=htons(5301);  // udp_local_port, default of 0 means kernel chooses
    if(bind(usd,(struct sockaddr *) &sa_rcvr,sizeof(sa_rcvr)) == -1) {
        fprintf(stderr,"could not bind to udp port\n");
        close(usd);
        return -1;
    }
    listen(usd, 3);

    // setup transmit
    // get host name
    bzero((char *) &sa_dest, sizeof(sa_dest));
    ntpserver=gethostbyname(hostname);
    if (ntpserver == NULL) {
        fprintf(stderr,"could not resolve hostname %s\n", hostname);
        close(usd);
        return -1;
    }
    if (ntpserver->h_length != 4) {
        fprintf(stderr,"oops %d\n",ntpserver->h_length);
        close(usd);
        return -1;
    }
    memcpy(&(sa_dest.sin_addr.s_addr),ntpserver->h_addr_list[0],4);
    // connect ntp server
    sa_dest.sin_family=AF_INET;
    sa_dest.sin_port=htons(NTP_PORT);
    if (connect(usd,(struct sockaddr *)&sa_dest,sizeof(sa_dest))==-1) {
        fprintf(stderr,"could not connect to udp port %d\n", NTP_PORT);
        close(usd);
        return -1;
    }
    // ntp handle
    err = ntpHandle(usd);
    // exit
    close(usd);
    return err;
}

int main(int argc, char *argv[]) {
    if(argc<2) {
        fprintf(stderr, "Usage: ntpc tick.stdtime.gov.tw\n");
        exit(-1);
    }
    return ntpclient(argv[1]);
}
