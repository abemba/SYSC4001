#include "msg_queue.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVER_MSQ_QUEUE_KEY 56923
#define CLIENT_MSQ_QUEUE_KEY 56924


struct msg_queue_msg_t {
    long int mtype; /* client pid */
    struct message_t mtext;
};

/* Message queue identifiers */
static int server_id = -1;
static int client_id = -1;


int start_server(void)
{
    // Create message queue to be used for sending messages to the server
    server_id = msgget((key_t)SERVER_MSQ_QUEUE_KEY,
                       0666 | IPC_CREAT | IPC_EXCL);
    if (server_id == -1) {
        // Failed to create message queue!
        fprintf(stderr, "Could not create server message queue: %s\n",
                strerror(errno));
        return 1;
    }
    
    // Create message queue to be used for sending messages to clients
    client_id = msgget((key_t)CLIENT_MSQ_QUEUE_KEY,
                       0666 | IPC_CREAT | IPC_EXCL);
    if (server_id == -1) {
        // Failed to create message queue!
        fprintf(stderr, "Could not create client message queue: %s\n",
                strerror(errno));
        return 1;
    }
    
    return 0;
}

void stop_server(void)
{
    if ((server_id != -1) && (msgctl(server_id, IPC_RMID, 0) == -1)) {
        fprintf(stderr, "Could not delete server message queue: %s\n",
                strerror(errno));
    }
    if ((client_id != -1) && (msgctl(client_id, IPC_RMID, 0) == -1)) {
        fprintf(stderr, "Could not delete client message queue: %s\n",
                strerror(errno));
    }
    
    server_id = -1;
    client_id = -1;
}

int get_client_request(struct message_t *msg)
{
    struct msg_queue_msg_t recieved_msg;
    
    if (msgrcv(server_id, (void *)&recieved_msg, sizeof(*msg), 0, 0) == -1) {
        fprintf(stderr, "Failed to receive from client message queue: %s\n",
                strerror(errno));
        return 1;
    }
    
    *msg = recieved_msg.mtext;
    return 0;
}

int send_response_to_client(const struct message_t msg)
{
    struct msg_queue_msg_t outgoing_msg;
    
    outgoing_msg.mtype = msg.client_pid;
    outgoing_msg.mtext = msg;
    
    if (msgsnd(client_id, (void *)&outgoing_msg, sizeof(msg), 0) == -1) {
        fprintf(stderr, "Failed to send message to client: %s\n",
                strerror(errno));
        return 1;
    }
    
    return 0;
}


int start_client(void)
{
    // Get the message queues created by the server
    server_id = msgget((key_t)SERVER_MSQ_QUEUE_KEY, 0666);
    if (server_id == -1) {
        // Failed to create message queue!
        fprintf(stderr, "Could not get server message queue: %s\n",
                strerror(errno));
        return 1;
    }
    
    client_id = msgget((key_t)CLIENT_MSQ_QUEUE_KEY, 0666);
    if (server_id == -1) {
        // Failed to create message queue!
        fprintf(stderr, "Could not get client message queue: %s\n",
                strerror(errno));
        return 1;
    }
    
    return 0;
}

void stop_client(void)
{
    server_id = -1;
    client_id = -1;
}

int send_message_to_server(const struct message_t msg)
{
    struct msg_queue_msg_t outgoing_msg;
    
    outgoing_msg.mtype = msg.client_pid;
    outgoing_msg.mtext = msg;
    
    if (msgsnd(server_id, (void *)&outgoing_msg, sizeof(msg), 0) == -1) {
        fprintf(stderr, "Failed to send message to server: %s\n",
                strerror(errno));
        return 1;
    }
    
    return 0;
}

int get_response_from_server(struct message_t *msg)
{
    struct msg_queue_msg_t recieved_msg;
    
    if (msgrcv(client_id, (void *)&recieved_msg, sizeof(*msg), 0, 0) == -1) {
        fprintf(stderr, "Failed to receive from server message queue: %s\n",
                strerror(errno));
        return 1;
    }
    
    *msg = recieved_msg.mtext;
    return 0;
}
