#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
/* https://www.gnu.org/software/libc/manual/html_node/Sockets.html */
#include <netinet/in.h>
#include <unistd.h> /* for close */

int main() {
    /* Creating a socket */
    /* https://www.gnu.org/software/libc/manual/html_node/Creating-a-Socket.html
     */
    /* int socket(int namespace, int style, int protocol) */
    /* namespace: this also can be called domain, and is sometimes called
     * `protocol family` */
    /* style: communication style */
    /* protocol: The protocol determines what low-level mechanism is used to
     * transmit and receive data. Each protocol is valid for a particular
     * namespace and communication style */
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket failed");
        return EXIT_FAILURE;
    }

    printf("Socket created\n");
    printf("Socket fd: %d\n", socket_fd);

    /* Socket Addresses */
    /* https://www.ibm.com/docs/en/aix/7.3?topic=sockets-socket-addresses */

    /* Socket Address Data Structures */
    /* https://www.ibm.com/docs/en/aix/7.3?topic=files-socket-address-data-structures
     */

    /* Creating a address */
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

    const int server_port = 8080;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* localhost 127.0.0.1 */
    server_address.sin_port = htons(server_port);

    socklen_t server_address_len = sizeof(server_address);

    /* Binding the socket with the address */
    /* int bind(int socket, const struct sockaddr *address, socklen_t
     * address_len) */
    /* If sucessful, bind() returns 0, otherwise -1 */
    int bind_return = bind(socket_fd, (struct sockaddr *)&server_address, server_address_len);
    if (bind_return == -1) {
        perror("socket bind failed");
        return EXIT_FAILURE;
    }

    /* Listening for incoming connections */
    /* https://www.gnu.org/software/libc/manual/html_node/Listening.html */
    /* int listen(int socket, int pending_connections_number) */
    int listen_return = listen(socket_fd, 5);
    if (listen_return == -1) {
        perror("socket listen failed");
        return EXIT_FAILURE;
    }

    /* Accepting a connection */
    /* https://www.gnu.org/software/libc/manual/html_node/Accepting-Connections.html */
    /* int accept(int socket, struct sockaddr *address, socklen_t *address_len) */
    /* Normal return value of accept is the file descriptor for the new socket. */
    /* If an error occurs, -1 is returned */
    // int accept_socket_fd =
    //     accept(socket_fd, (struct sockaddr *)&server_address, &server_address_len);
    // if (accept_socket_fd == -1) {
    //     perror("socket accept failed");
    //     return EXIT_FAILURE;
    // }
    //
    // printf("Connection accepted\n");
    // printf("Accept socket fd: %d\n", accept_socket_fd);
    while (1) {
        int accept_socket_fd =
            accept(socket_fd, (struct sockaddr *)&server_address, &server_address_len);
        if (accept_socket_fd == -1) {
            perror("socket accept failed");
            return EXIT_FAILURE;
        }

        printf("Connection accepted\n");
        printf("Accept socket fd: %d\n", accept_socket_fd);

        /* Shutdown the socket */
        /* https://www.gnu.org/software/libc/manual/html_node/Closing-a-Socket.html */
        /* int shutdown(int socket, int how) */
        /* shutdown tries to fully shut down the communication on the current socket
         * and other copies of it.*/
        /* However, shutdown does not reclaim resources used by the socket. */
        shutdown(accept_socket_fd, 2);

        /* Closing the socket */
        /* https://www.gnu.org/software/libc/manual/html_node/Opening-and-Closing-Files.html */
        /* close(int file_descriptor) */
        /* If the file descriptor is successfully closed, close() returns 0. Otherwise return -1 */
        /* close() reclaim resources that the operating system has allocated to the file descriptor */
        /* But if there are copies of the file descriptor, the resources are not reclaimed */
        close(accept_socket_fd);
    }
}
