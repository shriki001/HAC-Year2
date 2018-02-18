/******************************************************************************
* Exr. #4a3: This Program us as CLIENT process.
*            the client connects to both servers, if it can't registered.
*            if it sucseeded:
*            reads a char from user, if it 'n' referring to operation on
*            number, if it 's' referring to operation on string.
*            (to the application server)
*            when reads 'e' - the client process ends and calls the register
*            server to be removed.
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
* =============================================================================
* Input: Nothing.
* -----------------------------------------------------------------------------
* Output: 0/1 by the answer the (application) server
*         Informative messages for the user
* =============================================================================
******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdbool.h>
// ============================== Defines ================================== //
#define MAX_LEN 30                  // size of text message
// ============================== Structs ================================== //
struct msgbuf1 {
    long mtype;
    char m_text[MAX_LEN];
};
struct Data {
   pid_t _id;
    char _num[MAX_LEN];
    bool _ans;  // used as ans
};
struct msgbuf2 {
    long mtype;
    struct Data _data;
};
// ============================= Prototypes ================================ //
void connect_queue(key_t* _key, int* _msqid, char _type);
// ================================ Main =================================== //
int main() {
    // declaration on structs for using them
    struct msgbuf1 msg1;
    struct msgbuf2 msg2;
    int msqid1,         // id of message queue of server 1 (registry)
            msqid2;         // id of message queue of server 2 (apps)
    char code;          // keep code of msg from registry server
    char s[] = "";
    char ans;     // keep user answer of the operation he willing to
    key_t key1,         // key of message queue of server 1 (registry)
            key2;         // key of message queue of server 2 (apps)
    // connecting client into both 2 servers
    connect_queue(&key1, &msqid1, 'a');
    connect_queue(&key2, &msqid2, 'b');
    sprintf(s, "%d", (int) (getpid()));
    /// register to server 1 (registry) ///
    msg1.mtype = 1;             // 1 = register, add a new
    strcpy(msg1.m_text, s);
    // send the message to registry server
    if (msgsnd(msqid1, &msg1, MAX_LEN, 0) == -1) {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }
    // recive the message from registry server
    if (msgrcv(msqid1, &msg1, MAX_LEN, 0, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    code = msg1.m_text[0];      // msg code stored in the first cell
    if (code == '1')          // register succeeded
        printf("%s\n", msg1.m_text);
    else {
        puts("fail to register");
        exit(EXIT_FAILURE);
    }
// now we can try applications from the applications server ///
    do {
        puts("Enter a char please: ( s - string / n - number / e - exit)");
        scanf(" %c", &ans);
        switch (ans) {
            case 'n':
                puts("Enter a number please:");
                scanf("%s", msg2._data._num);
                msg2.mtype = 1;             // working on a number
                break;
            case 's':
                puts("Enter a string please:");
                scanf("%s", msg2._data._num);
                msg2.mtype = 2;         // working on a string
                break;
            case 'e':               //- ask to exit -
                msg1.mtype = 3;
                strcpy(msg1.m_text, s);
                if (msgsnd(msqid1, &msg1, MAX_LEN, 0) == -1) {
                    perror("msgsnd failed");
                    exit(EXIT_FAILURE);
                }exit(EXIT_SUCCESS);
            default:            ///- Unknown request -
                continue;
        }msg2._data._id = (int)getpid();          // save pid to ask for approve
        // send the message to applications server
        if (msgsnd(msqid2, &msg2, sizeof(struct Data), 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
        // recive a message from applications server
        if (msgrcv(msqid2, &msg2, sizeof(struct Data), (int) getpid(), 0)
            == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        // print the answer to the request
        printf("%d\n", msg2._data._ans);
    } while (1);
    return EXIT_SUCCESS;
}
// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// This function get a key, a message queue id and a char
// Checks if can open the queue using the char as a 'type'
void connect_queue(key_t* _key, int* _msqid, char _type) {
    if (((*_key) = ftok("/.", _type)) == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }if (((*_msqid) = msgget((*_key), 0)) == -1) { // connect to an exist queue
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
}