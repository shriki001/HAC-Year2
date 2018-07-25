/******************************************************************************
* Exr. #6a1: This Program used as the REGISTRY SERVER.
*	     this server open a socket for clients and allows 2 operations:
*            1) add a new process
*            2) check if a process if registered on system
*          
*	    when get code:
*           1: if successed to add, return 0.
*             if not - return 1 because it alredy exist, and return 2
*             because it not exist at all.
*          2: if exist -return 1, else -return 0.
*	   if the server found out that a client disconnect he removes him
*	   from the file descriptor database
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* login	   	    : kfirma / michaelshr
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
#include <string.h>        // for memset
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

// ============================== Defines ================================== //
#define ARR_SIZE  10          // size of the array of keeping clients ids
#define BUFLEN    50         // size of the buffer message (response)

// ============================== Globals ================================== //
int main_socket,            // the main socket number
    serving_socket;         // the serving socket number

// ============================= Prototypes ================================ //
void catch_intr(int s);
void m_memset(struct addrinfo *con_kind);
void m_getaddrinfo(struct addrinfo *con_kind,struct addrinfo **addr_info_res,
                   char* port);

int exist_pid(pid_t wanted,const pid_t arr[]);
void open_socket(int *socket_name, struct addrinfo **addr_info_res);
void bind_listen(const int *socket_name, struct addrinfo **addr_info_res);
int search_pid(pid_t wanted,const pid_t arr[]);

// ================================ Main =================================== //
int main(int argc, char *argv[])
{
    int rc,                    // save value of a system call
        fd,                    // file descriptor of client
        operator,              // the operation id that the user asked
        first_empty,          // indicate first cell is empty
        is_exist;             // indicate if process exist
    int fd_arr[ARR_SIZE] = {0};    // array of file descriptors
    char rbuf[BUFLEN];             // the string from the client

    struct addrinfo con_kind, *addr_info_res;    // 2 struct of socket details

    // check if user input has one argument (oprt of server)
    if (argc < 2) {
        fprintf(stderr, "Missing server port, Usage: <port server 1> \n");
        exit(EXIT_FAILURE);
    }

    fd_set rfd, c_rfd;               //  2 groups of file descriptors

    pid_t process_id,                       // keep the process id
          procces_arr[ARR_SIZE] = {0};      // process id array (of clients)

    signal(SIGINT, catch_intr);     //  signal handler

    m_memset(&con_kind);             // reset the linked list of socket
    m_getaddrinfo(&con_kind,&addr_info_res,argv[1]); //get info from server

    open_socket(&main_socket, &addr_info_res);       // open the main socket

    // start listen for request and bind a name to socket
    bind_listen(&main_socket, &addr_info_res);

    FD_ZERO(&rfd);                  // empties the file descriptor group
    FD_SET(main_socket, &rfd);      // adds the main socket's fd to fd group

    /// the main loop of the server
    while (1) {
        c_rfd = rfd;
        // we want to get new request
        rc = select(getdtablesize(), &c_rfd, NULL, NULL,
                    (struct timeval *) NULL);

        if (FD_ISSET(main_socket, &c_rfd))  // exist on group
        {
            serving_socket = accept(main_socket, NULL, NULL);
            if (serving_socket >= 0)      // there is suitable one
                FD_SET(serving_socket, &rfd);
        }

        // going through the group of open file descriptors and search him
        for (fd = main_socket + 1; fd < getdtablesize(); fd++) {
            if (FD_ISSET(fd , &c_rfd)) {
                rc = read(fd , &rbuf , sizeof(char) * BUFLEN);
                sscanf(rbuf , "%d %d" , &operator , &process_id);
                switch (operator) {
                    case 1:              // -- adding a process -- //
                        first_empty = search_pid(process_id , procces_arr);
                        if (first_empty == -1)              // process is found
                            strcpy(rbuf , "0 | CANT ADD (process is exist!)");
                        else {       // process is not found
                            if (first_empty == ARR_SIZE)  // dont have an
                                                          // empty place
                                strcpy(rbuf ,
                                       "2 | CANT ADD (array is FULL!)");
                            else {                    // have an empty place
                                procces_arr[first_empty] = process_id;
                                fd_arr[first_empty] = fd;
                                strcpy(rbuf , "1 | The process ADDED!");
                            }
                        }
                        break;

                    case 2:             // -- cheking if a process registered -- //
                        is_exist = exist_pid(process_id , procces_arr);
                        if (is_exist ==
                            -1)         // process is found, its exist
                            strcpy(rbuf , "0 | NOT EXIST");
                        else                        // process not found
                            strcpy(rbuf , "1 | PROCESS EXISTS");
                        break;
                }

                if (rc == 0)    // if we failed to read from client
                {
                    is_exist = exist_pid(fd , fd_arr);
                    if (is_exist != -1) {         // process is found
                        procces_arr[is_exist] = 0;
                        fd_arr[is_exist] = 0;
                    }

                    close(fd);          // close file descriptor of client
                    FD_CLR(fd , &rfd);  // remove from file descriptors group
                }

                else if (rc > 0)        // can read
                    write(fd , rbuf , strlen(rbuf) + 1);

                else {    // we failed to read
                    perror("read failed");
                    exit(EXIT_FAILURE);
                }

                int i;
                puts("\n ##### connected current  view #####");
                for (i = 0; i < ARR_SIZE; i++)
                    printf("%d," , procces_arr[i]);
                puts("");
            }
        }
    }
    return (EXIT_SUCCESS);
}

// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// Signal handler for catching the SIGINT signal
// close all the sockets used
void catch_intr(int s)
{
    if (s == SIGINT) {
        close(main_socket);
        close(serving_socket);
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
void m_getaddrinfo(struct addrinfo *con_kind,
                   struct addrinfo **addr_info_res,char* port)
{
    int rc = getaddrinfo(NULL, port, (*&con_kind), (*&addr_info_res));

    if (rc != 0) {       // failed to get info
        fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc));
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
// This function get a socket name ant its details struct
// performs bind and than listen
void bind_listen(const int *socket_name, struct addrinfo **addr_info_res)
{
    if (bind((*socket_name), (*addr_info_res)->ai_addr,
             (*addr_info_res)->ai_addrlen)) {

        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen((*socket_name), ARR_SIZE)) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function gets a desired pid and the array of pids
// Checks if ut exist on the array, if so - return his index , else returns -1
int search_pid(pid_t wanted,const pid_t arr[])
{
    int i;                      // index for running on array
    int empty = ARR_SIZE;      // in the worst case - there is no empty cells
    bool found_empty = false;      // check if we found an empty place already

    for (i = 0; i < ARR_SIZE; i++) {
        if (arr[i] == wanted)
            return -1;
        if (!found_empty && arr[i] == 0) {   // found an empty cell
            empty = i;
            found_empty = true;
        }
    }

    return empty;       // return first empty
}

//---------------------------------------------------------------------------//
// This function get a process id and a array of process' id
// Searching if the desired process exist in the array.
// return his index if found, otherwise - returns (-1)
int exist_pid(pid_t wanted,const pid_t arr[])
{
    int i;           // index for running on array

    for (i = 0; i < ARR_SIZE; i++)
        if (arr[i] == wanted)
            return i;

    return -1;
}
