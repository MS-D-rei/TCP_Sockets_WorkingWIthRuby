#include <stdio.h>
#include <stdlib.h>

/* Network byte order is big endian */
/* Endianess is a problem when we are transferring data between different
 * machines */
/* We need to convert the data to network byte order before sending it over the
 * network */

/* Ref */
/* http://cms.phys.s.u-tokyo.ac.jp/~naoki/CIPINTRO/NETWORK/endian.html */

/* htonl() converts the unsigned integer hostlong from host byte order to
 * network byte order */
/* https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-htonl-translate-address-host-network-long
 */
/* htons() converts the unsigned short integer hostshort from host byte order
 * to network byte order */
/* https://www.ibm.com/docs/en/zos/2.4.0?topic=lf-htons-translate-unsigned-short-integer-into-network-byte-order
 */

int main(void) {
    unsigned int x = 0x12345678;

    printf("x = 0x%x\n", x); // 12345678 (hexadecimal)

    unsigned char *ptr = (unsigned char *)&x;

    /* this probably prints 78:56:34:12 and it means little endian is used in
     * CPU */
    printf("%02x:%02x:%02x:%02x\n", ptr[0], ptr[1], ptr[2], ptr[3]);

    unsigned long lp = htonl(x);
    printf("%lu\n", lp);

    unsigned char *ptr2 = (unsigned char *)&lp;

    /* this will prints 12:34:56:78 in big endian order */
    printf("%02x:%02x:%02x:%02x\n", ptr2[0], ptr2[1], ptr2[2], ptr2[3]);

    return EXIT_SUCCESS;
}
