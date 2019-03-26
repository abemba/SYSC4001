#include "msg_format.h"

/**
 *  Create message queues.
 *
 *  @return 0 if successfull
 */
extern int start_server(void);

/**
 *  Remove message queues.
 */
extern void stop_server(void);

/**
 *  Recieve message from client.
 *
 *  @param msg Pointer to where the received message will be stored
 *
 *  @return 0 if successfull
 */
extern int get_client_request(struct message_t *msg);

/**
 *  Send a message to a client.
 *
 *  @param msg The message to be sent
 *
 *  @return 0 if successfull
 */
extern int send_response_to_client(const struct message_t msg);


/**
 *  Get message queues created by server.
 *
 *  @return 0 if successfull
 */
extern int start_client(void);

/**
 *  Prevent use of message queues from client side.
 */
extern void stop_client(void);

/**
 *  Send a message to the server.
 *
 *  @param msg The message to be sent
 *
 *  @return 0 if successfull
 */
extern int send_message_to_server(const struct message_t msg);

/**
 *  Recieve message from the server.
 *
 *  @param msg Pointer to where the received message will be stored
 *
 *  @return 0 if successfull
 */
extern int get_response_from_server(struct message_t *msg);
