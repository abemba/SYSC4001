#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "array_list.h"

#include "msg_format.h"
#include "msg_queue.h"

/** Descriptor for command */
struct command_t {
    enum request_type request_type;
    void (*function)(struct message_t msg);
};

#define NUM_COMMANDS 7
static struct command_t commands[];

/** Atomic int type to syncronize between signal handler and main loop */
static volatile sig_atomic_t running;

/** Backing data structure */
static struct array_list_t data;

/**
 *  Signal handler to stop the server on sigint
 */
static void sigint_handler(int _)
{
    (void)_;
    running = 0;
    stop_server();
}

int main (int argc, char **argv)
{
    running = 1;
    // Register handler for SIGINT
    signal(SIGINT, sigint_handler);
    
    // Create list for data
    if (array_list_create(&data, 2)) {
        fprintf(stderr, "Failed to create array list.\n");
        exit(1);
    }
    
    // Create message queues
    if (start_server()) {
        fprintf(stderr, "Failed to create message queues.\n");
        exit(1);
    }
    
    // Main loop
    struct message_t msg;
    
    while (running) {
        // Get a message from a client
        if (get_client_request(&msg) != 0) {
            break;
        }
        
        // Find the command which the client wants and run it
        int i;
        for (i = 0; i < NUM_COMMANDS; i++) {
            if (commands[i].request_type == msg.type) {
                commands[i].function(msg);
                break;
            }
        }
        
        // If no command was found, print an error message
        if (i == NUM_COMMANDS) {
            printf("Received unkown command %jd\n", (intmax_t) msg.type);
        }
    }
    
    // Remove message queues
    stop_server();
    
    // Free data
    array_list_free(&data);
}

/**
 *  Function to handle insert command.
 *
 *  @param msg The message which was received from the client
 */
void insert_func (struct message_t msg)
{
    // Allocate a new record
    void *new = malloc(sizeof(struct employee_t));
    
    if (new) {
        // Copy the record from the client into the new record
        memcpy(new, &msg.insert.request, sizeof(struct employee_t));
        
        // Add the new record to the list
        if (array_list_append(&data, new)) {
            msg.insert.response.type = REQUEST_FAILURE;
        } else {
            msg.insert.response.type = REQUEST_END;
        }
    } else {
        msg.insert.response.type = REQUEST_FAILURE;
    }
    
    // Send a response to the client
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

/**
 *  Function to handle check name command.
 *
 *  @param msg The message which was received from the client
 */
void check_name_func (struct message_t msg)
{
    // Find the employee who's number matches the request
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.check_name.request)) {
            // Matching employee found, copy employee name to response
            strncpy(msg.check_name.response.name, employee->name,
                    EMPLOYEE_NAME_LENGTH);
            break;
        }
    }
    
    // Send a failure if no matching employee was found
    if (i >= data.length) {
        msg.check_name.response.type = REQUEST_FAILURE;
    } else {
        msg.check_name.response.type = REQUEST_END;
    }
    
    // Send a response to the client
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

/**
 *  Function to handle check department command.
 *
 *  @param msg The message which was received from the client
 */
void check_dept_func (struct message_t msg)
{
    // Find the employee who's number matches the request
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.check_dept.request)) {
            // Matching employee found, copy department name to response
            strncpy(msg.check_dept.response.department, employee->department,
                    EMPLOYEE_DEPT_LENGTH);
            break;
        }
    }
    
    // Send a failure if no matching employee was found
    if (i >= data.length) {
        msg.check_dept.response.type = REQUEST_FAILURE;
    } else {
        msg.check_dept.response.type = REQUEST_END;
    }
    
    // Send a response to the client
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

/**
 *  Function to handle check salary command.
 *
 *  @param msg The message which was received from the client
 */
void check_salary_func (struct message_t msg)
{
    // Find the employee who's number matches the request
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.check_salary.request)) {
            msg.check_salary.response.salary = employee->salary;
            break;
        }
    }
    
    // Send a failure if no matching employee was found
    if (i >= data.length) {
        msg.check_salary.response.type = REQUEST_FAILURE;
    } else {
        msg.check_salary.response.type = REQUEST_END;
    }
    
    // Send a response to the client
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

/**
 *  Function to handle check employee number command.
 *
 *  @param msg The message which was received from the client
 */
void check_num_func (struct message_t msg)
{
    // Find the employee who's name matches the request
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->name, msg.check_employee_num.request)) {
            strncpy(msg.check_employee_num.response.number, employee->number,
                    EMPLOYEE_NUM_LENGTH);
            break;
        }
    }
    
    // Send a failure if no matching employee was found
    if (i >= data.length) {
        msg.check_employee_num.response.type = REQUEST_FAILURE;
    } else {
        msg.check_employee_num.response.type = REQUEST_END;
    }
    
    // Send a response to the client
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

/**
 *  Function to handle list department command.
 *
 *  @param msg The message which was received from the client
 */
void list_dept_func (struct message_t msg)
{
    // Create seperate response structure as the reponse must be modified before
    // we are done with the origional request
    struct message_t response;
    response.client_pid = msg.client_pid;
    response.type = msg.type;
    
    // Find all emplpyee's with matching departments
    for (int i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->department, msg.list_dept.request)) {
            // Department matches, send response to client with type continue to
            // indicate that there may still be more matching records
            response.list_dept.response.type = REQUEST_CONTINUE;
            strncpy(response.list_dept.response.number, employee->number,
                    EMPLOYEE_NUM_LENGTH);
            if (send_response_to_client(response)) {
                fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                        (intmax_t) msg.client_pid);
            }
        }
    }
    
    // Send a response to the client which indicates that there are no more
    // matching records
    msg.list_dept.response.type = REQUEST_END;
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

/**
 *  Function to handle delete command.
 *
 *  @param msg The message which was received from the client
 */
void delete_func (struct message_t msg)
{
    // Find the employee who's number matches the request
    int i;
    int removed = 0;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.delete.request)) {
            // Remove matching employee
            array_list_remove(&data, i);
            removed = 1;
            break;
        }
    }
    
    // Send a failure if no matching employee was found
    if (!removed) {
        msg.delete.response.type = REQUEST_FAILURE;
    } else {
        msg.delete.response.type = REQUEST_END;
    }
    
    // Send a response to the client
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

/** List of command descriptors */
static struct command_t commands[NUM_COMMANDS] = {
    {.request_type = REQUEST_INSERT, .function = insert_func},
    {.request_type = REQUEST_CHECK_NAME, .function = check_name_func},
    {.request_type = REQUEST_CHECK_DEPT, .function = check_dept_func},
    {.request_type = REQUEST_CHECK_SALARY, .function = check_salary_func},
    {.request_type = REQUEST_CHECK_EMPLOYEE_NUM, .function = check_num_func},
    {.request_type = REQUEST_LIST_DEPT, .function = list_dept_func},
    {.request_type = REQUEST_DELETE, .function = delete_func}
};
