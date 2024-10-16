/* https://workingwithruby.com/wwtcps/clients */

/* sender -> truck -> address -> port -> receiver */
/* Socket is like a moving stockpile, it carries data from one place to another. */
/* Address is like a destination of the moving stockpile. */
/* Port is like a door number of the destination location. */
/* Sender(client) prepare its moving stockpile(socket) */
/* Sender(client) work is to send the payload to the specific door(port) of the destination(address)
 */
/* Sender(clinet) doesn't receive any data, so don't need to bind its stockpile(socket) with address
 */
/* Sender(clinet) need to connect its moving stockpile(socket) with the destination(address) and
 * door(port) */
/* to deliver the payload properly */
/* `send()` is used to hand over the payload to the stockpile(socket) of receiver(server) */
/* After sending the payload, `shutdown()` close the way between sender(client) and receiver(server)
 */
/* `close()` is used to close the moving stockpile(socket) and reclaim the resources */

#include <arpa/inet.h>  /* for inet_ntoa */
#include <netdb.h>      /* for gethostbyname */
#include <netinet/in.h> /* for sockaddr_in, in_addr */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h> /* for close */

int main(void) {
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket() failed");
        return EXIT_FAILURE;
    };

    struct sockaddr_in server_address;
    /**
     * https://www.gnu.org/software/libc/manual/html_node/Internet-Address-Formats.html
     * struct sockaddr_in {
     * __uint8_t       sin_len;
     * // Address family or format of the socket address. Should be AF_INET.
     * sa_family_t     sin_family;
     * in_port_t       sin_port;     // Port number. network byte order.
     * struct  in_addr sin_addr;     // IPv4 address. network byte order.
     * char            sin_zero[8];
     * };
     */

    server_address.sin_family = AF_INET;

    /* Searching for the IP address of the host */
    /* https://www.gnu.org/software/libc/manual/html_node/Host-Names.html */
    /* struct hostent *gethostbyname(const char *name) */
    struct hostent *host_db = gethostbyname("localhost");
    /* hostent */
    /* https://www.gnu.org/software/libc/manual/html_node/Host-Names.html */
    /*
     * Structures returned by network data base library.  All addresses are
     * supplied in host order, and returned in network order (suitable for
     * use in system calls).
     */
    /*
     * struct hostent {
     *  char    *h_name;        // official name of host
     *  char    **h_aliases;    // alias list
     *  int     h_addrtype;     // host address type
     *  int     h_length;       // length of address
     *  char    **h_addr_list;  // list of addresses from name server
     *
     *  #if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
     *  #define h_addr  h_addr_list[0]  // address, for backward compatibility
     *  #endif // (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE)
     * }
     */

    if (host_db == NULL) {
        perror("gethostbyname() failed");
        return EXIT_FAILURE;
    }
    server_address.sin_addr = *(struct in_addr *)host_db->h_addr_list[0];
    /* inet_ntoa() accepts an Internet address expressed as a 32-bit quantity in network byte order
     * and returns a string expressed in dotted-decimal notation.
     */
    printf("IP address found: %s\n", inet_ntoa(server_address.sin_addr));

    const int client_port = 8080;
    server_address.sin_port = htons(client_port);

    /* Connecting to the server */
    int connect_return =
        connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_return == -1) {
        perror("connect() failed");
        return EXIT_FAILURE;
    }
    printf("Connected to the server\n");

    /* Sending data to the server */
    /* ssize_t send(int socket, const void *buffer, size_t length, int flags) */
    if (send(socket_fd, "Hello, server!", 14, 0) == -1) {
        perror("send() failed");
        return EXIT_FAILURE;
    }
    printf("Data sent to the server\n");

    /* Closing the socket */
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
    printf("Socket closed\n");

    return EXIT_SUCCESS;
}
