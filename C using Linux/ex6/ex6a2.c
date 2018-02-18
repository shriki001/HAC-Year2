/******************************************************************************
* Exr. #6a2: This Program used as the APPLICATIONS SERVER.
*	     conect to REGISTRY SERVER socket, checks the process id-s that 
*	     connected to the register server via file descriptor database  
*            this server create a socket and allows 2 operations:
*            1) check a primery of a number
*            2) check if a string is palindrome
*
*            If the process exist on th database, if the result is "true"
*            returns 1, if "false" - returns 0. to add, returns 0.
*            If the process isn't exist on th database, returns -1
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
* Output: Nothing.
*
* =============================================================================
******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>            // for memset
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
// ============================== Defines ================================== //
#define QUEUE_SIZE  10          // size of the queue of waiting clients
#define BUFLEN      50          // size of the buffer message (response)

// ============================== Globals ================================== //
int main_socket,            // main socket number
    serving_socket,         // serving socket number
    reg_socket;             // registry server's socket number

// ============================= Prototypes ================================ //
void catch_intr(int s);
void m_memset(struct addrinfo *con_kind);
void m_getaddrinfo(char *server_ip, char *server_port,
                   struct addrinfo *con_kind,
                   struct addrinfo **addr_info_res);

void open_socket(int *socket_name, struct addrinfo **addr_info_res);
void connect_socket(const int *socket_name, struct addrinfo **addr_info_res);
void bind_listen(const int *socket_name, struct addrinfo **addr_info_res);
bool is_prime(int n);
bool is_palidrom(char s[]);

// ================================ Main =================================== //
int main(int argc, char *argv[])
{
    int rc,                      // save value of a system call
        rc2,
        fd,                     // file descriptor of client
        operator;               // the operation id that the user asked
    fd_set rfd, c_rfd;
    char reg_buff[BUFLEN],      // the string from the registry server
         app_buff[BUFLEN],      // the string from the applications server
         num[BUFLEN],           // keep number (string format)
         s[BUFLEN] = "";        // keep a string

    pid_t procces_id;           // keep A client process id

    // check if user input has 3 argument
    if (argc < 4) {
        fprintf(stderr, "Missing arguments, Usage: <port server 2> <ip"
                "server 1> <port server 1>  \n");
        exit(EXIT_FAILURE);
    }

    // 2 struct of each socket details (2 sockets - app and reg)
    struct addrinfo con_kind, *addr_info_res;
    struct addrinfo con_kind2, *addr_info_res2;

    signal(SIGINT, catch_intr);     //  signal handler

    m_memset(&con_kind); // reset the linked list of reg socket

    // reset the linked list of apps socket
    memset(&con_kind2, 0, sizeof(con_kind2));
    con_kind2.ai_family = AF_UNSPEC;
    con_kind2.ai_socktype = SOCK_STREAM;

    //get info from 2 servers
    m_getaddrinfo(NULL, argv[1], &con_kind, &addr_info_res);
    m_getaddrinfo(argv[2], argv[3], &con_kind2, &addr_info_res2);

    // open the the to registry server's ans applications' sockets
    open_socket(&main_socket, &addr_info_res);
    open_socket(&reg_socket, &addr_info_res2);
    
    connect_socket(&reg_socket, &addr_info_res2); // connect to reg socket
    bind_listen(&rc, &main_socket, &addr_info_res);    

    FD_ZERO(&rfd);                  // empties the file descriptors group
    FD_SET(main_socket, &rfd);      // adds the main socket's fd to fd group

    while (1) {
        c_rfd = rfd;            // copy of file descriptor
        // we want to get new request
        rc = select(getdtablesize(), &c_rfd, NULL, NULL,
                    (struct timeval *) NULL);

        if (FD_ISSET(main_socket, &c_rfd))    // exist on group
        {
            serving_socket = accept(main_socket, NULL, NULL);
            if (serving_socket >= 0)        // there is suitable one
                FD_SET(serving_socket, &rfd);
        }

        // going through the group of open file descriptors and search him
        for (fd = main_socket + 1; fd < getdtablesize(); fd++) {
            if (FD_ISSET(fd , &c_rfd)) {
                rc = read(fd , &reg_buff , sizeof(char) * BUFLEN);
                sscanf(reg_buff , "%d %d %s" , &operator , &procces_id , num);
                strcpy(app_buff , "2 ");
                sprintf(s , "%d " , procces_id);
                strcat(app_buff , s);
                //   send and then receive messages from the register server
                write(reg_socket , app_buff , strlen(app_buff) + 1);
                rc2 = read(reg_socket , app_buff , BUFLEN);
                if (rc2 > 0) {
                    if (app_buff[0] != '0') {
                        switch (operator) {
                            case 1:             // case asked a number
                                operator = is_prime(atoi(num));
                                sprintf(reg_buff , "%d" , operator);
                                break;

                            case 2:             // case asked a string
                                operator = is_palidrom(num);
                                sprintf(reg_buff , "%d" , operator);
                                break;

                            default:
                                break;
                        }
                    }
                    else {
                        strcpy(app_buff , "-1");
                        write(fd , app_buff , strlen(reg_buff) + 1);
                    }
                }

                if (rc == 0) {    // if we failed to read from client
                    close(fd);          // close file descriptor of client
                    FD_CLR(fd , &rfd);  // remove from file descriptors group
                }

                else if (rc > 0)    // can read
                    write(fd , reg_buff , strlen(reg_buff) + 1);

                else {   // we failed to read
                    perror("read failed");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    return (EXIT_SUCCESS);
}

// ============================= Functions ================================= //
// Signal handler for catching the SIGINT signal
// close all the sockets used
void catch_intr(int s)
{
    if (s == SIGINT) {
        close(main_socket);
        close(serving_socket);
        close(reg_socket);
    }
    exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------------//
// This function get the struct of socket details
// reset his size and parametes of family group,socket type and flags
void m_memset(struct addrinfo *con_kind)
{
    memset((*&con_kind), 0, sizeof (*con_kind));

    (*con_kind).ai_family = AF_UNSPEC;          // set that any group suitable
    (*con_kind).ai_socktype = SOCK_STREAM;
    (*con_kind).ai_flags = AI_PASSIVE;         // the sockets allocate the ip
}

//---------------------------------------------------------------------------//
// This function get a server ip & port ,and 2 struct of his socket details
// check if it got the info
void m_getaddrinfo(char *server_ip, char *server_port,
                   struct addrinfo *con_kind,
                   struct addrinfo **addr_info_res)
{
    int rc = getaddrinfo(server_ip, server_port, (*&con_kind),
                         (*&addr_info_res));

    if (rc != 0) {
        fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc));
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function get a socket id and the struct of socket details
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
// This function get a socket id the rc size, and the struct of socket details
// Checks if succeeded to connect this socket
void connect_socket(const int *socket_name, struct addrinfo **addr_info_res)
{
    int rc = connect((*socket_name),
                    (*addr_info_res)->ai_addr,    // addr of server: IP+PORT
                    (*addr_info_res)->ai_addrlen);
    if (rc) {
        perror("connect to register server failed:");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function get a socket name ant its details struct
// performs bind and than listen
void bind_listen(const int *socket_name, struct addrinfo **addr_info_res)
{
    if (bind((*socket_name), (*addr_info_res)->ai_addr,
             (*addr_info_res)->ai_addrlen)) {

        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen((*socket_name), QUEUE_SIZE)) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function get an integer number
// Return true if it prime number. otherwise - return false.
bool is_prime(int n) {
    int div;

    if (n == 0 || n == 1)
        return false;

    for (div = 2; div <= n / 2; div++)
        if (n % div == 0)
            return false;
    return true;
}

//---------------------------------------------------------------------------//
// This function get a string
// Return true if it a palindrome. otherwise - return false.
bool is_palidrom(char s[]) {
    int i;          // index for runnig on array

    size_t size = strlen(s);   // size of the string
    for (i = 0; i < size / 2; i++)
        if (s[i] != s[size - 1 - i])
            return false;
    return true;
}
