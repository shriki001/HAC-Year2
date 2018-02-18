/******************************************************************************
* Exr. #6a3: This Program us as CLIENT process.
*            the client connects to both servers sockets, 
*	     if it can't registered.
*	     if it sucseeded:
*            reads a char from user, if it 'n' referring to operation on
*            number, if it 's' referring to operation on string.
*            (to the application server)
*            when reads 'e' - the client process exit.
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input: Nothing.
*
* -----------------------------------------------------------------------------
* Output: 0/1 by the answer the (application) server
*         Informative messages for the user
*
* =============================================================================
******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                     // for read/write/close
#include <sys/types.h>                  // standard system types
#include <netinet/in.h>                 // Internet address structures
#include <sys/socket.h>                 // socket interface functions
#include <netdb.h>                      // host to IP resolution

// ============================== Defines ================================== //
#define  BUFLEN        50         // maximum response size

// ============================= Prototypes ================================ //
void m_memset(struct addrinfo *con_kind);
void open_socket(int *socket_name, struct addrinfo **addr_info_res);
void connect_socket(const int *socket_name,
                   ssize_t *rc,
                   struct addrinfo **addr_info_res);
void m_getaddrinfo(ssize_t *rc, char *server_ip, char *server_port,
                   struct addrinfo *con_kind,
                   struct addrinfo **addr_info_res);

// ================================ Main =================================== //
int main(int argc, char *argv[])
{
    ssize_t rc, rc2;         // save value of a system call(s)
    int reg_socket,             // registry server's socket number
        app_socket;             // applications server's socket number
    char rbuf[BUFLEN],         // the string from the client
         num[BUFLEN],          // keep number (string format)
         s[BUFLEN] = "",       // keep a string
         ans;                  // keep the ans from the server ( 0 or 1)


    // 2 struct of each socket details (2 sockets - app and reg)
    struct addrinfo con_kind, *addr_info_res;
    struct addrinfo con_kind2, *addr_info_res2;

    // check if user input has 4 argument (2 ips, 2 ports)
    if (argc < 5) {
        fprintf(stderr, "Missing arguments, Usage: <port server 1> <ip"
                 "server 1> <port server 2> <ip server 2>  \n");
        exit(EXIT_FAILURE);
    }

    // reset the linked list of sockets
    m_memset(&con_kind);
    m_memset(&con_kind2);

    // get info from servers
    m_getaddrinfo(&rc, argv[1], argv[2], &con_kind, &addr_info_res);
    m_getaddrinfo(&rc2, argv[3], argv[4], &con_kind2, &addr_info_res2);

    // open the 2 sockets( registry and applications servers sockets) and
    // connecting to them
    open_socket(&reg_socket, &addr_info_res);
    open_socket(&app_socket, &addr_info_res);
    connect_socket(&reg_socket, &rc, &addr_info_res);
    connect_socket(&app_socket, &rc2, &addr_info_res2);

    strcpy(rbuf, "1 ");
    sprintf(s, "%d ", getpid());
    strcat(rbuf, s);

    // send and then receive messages from the server
    write(reg_socket, rbuf, strlen(rbuf) + 1);
    rc = read(reg_socket, rbuf, BUFLEN);

    if (rc > 0) {     // read succeeded
        printf("%s\n", rbuf);
        if (rbuf[0] != '1')         // if the ...
            exit(EXIT_FAILURE);
    }

    else {
        perror("read() failed");
        exit(EXIT_FAILURE);
    }

    // the main loop of client method
    do {
        puts("Enter a char please: ( s - string / n - number / e - exit)");
        scanf(" %c", &ans);
        switch (ans) {
            case 'n':
                puts("Enter a number please:");
                strcpy(rbuf, "1 ");
                break;
            case 's':
                puts("Enter a string please:");
                strcpy(rbuf, "2 ");       // working on a string
                break;
            case 'e':           // exit from client
                close(reg_socket);
                freeaddrinfo(addr_info_res);
                close(app_socket);
                freeaddrinfo(addr_info_res2);
                exit(EXIT_SUCCESS);
            default:            ///- Unknown request -
                continue;
        }

        scanf("%s", num);
        sprintf(s, "%d ", getpid());
        strcat(rbuf, s);
        strcat(rbuf, num);
        write(app_socket, rbuf, strlen(rbuf) + 1);
        rc = read(app_socket, rbuf, BUFLEN);
        printf("%s\n", rbuf); // print the answer to the request

    } while (1);

    return EXIT_SUCCESS;
}
// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// This function get the struct of socket details
// reset his size and parametes of family group,socket type and flags
void m_memset(struct addrinfo *con_kind)
{
    memset((*&con_kind), 0, sizeof (*con_kind));
    (*con_kind).ai_family = AF_UNSPEC;          // set that any group suitable
    (*con_kind).ai_socktype = SOCK_STREAM;
}

//---------------------------------------------------------------------------//
// This function get a server ip & port ,and 2 struct of his socket details
// check if it got the info
void m_getaddrinfo(ssize_t *rc, char *server_ip, char *server_port,
                   struct addrinfo *con_kind,
                   struct addrinfo **addr_info_res)
{
    if (((*rc) = getaddrinfo(server_ip, server_port,
                             (*&con_kind),
                             (*&addr_info_res)) != 0))
    {
        fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror((int)(*rc)));
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function get a socket id and the struct of
// Checks if the socket opened successfuly
void open_socket(int *socket_name, struct addrinfo **addr_info_res)
{
    (*socket_name) = socket((*addr_info_res)->ai_family,
                            (*addr_info_res)->ai_socktype,
                            (*addr_info_res)->ai_protocol);

    if ((*socket_name) < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function get a socket id the rc size, and the struct of
// Checks if succeeded to connect this socket
void connect_socket(const int *socket_name, ssize_t *rc,
                   struct addrinfo **addr_info_res)
{
    (*rc) = connect((*socket_name),
                    (*addr_info_res)->ai_addr,    // addr of server: IP+PORT
                    (*addr_info_res)->ai_addrlen);

    if ((*rc)) {
        perror("connect to register server failed:");
        exit(EXIT_FAILURE);
    }
}
