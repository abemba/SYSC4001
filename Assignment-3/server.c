#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "array_list.h"

#include "msg_format.h"
#include "msg_queue.h"

struct command_t {
    enum request_type request_type;
    void (*function)(struct message_t msg);
};

#define NUM_COMMANDS 7
static struct command_t commands[];

static volatile sig_atomic_t running;

static struct array_list_t data;

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
        if (get_client_request(&msg) != 0) {
            break;
        }
        
        int i;
        for (i = 0; i < NUM_COMMANDS; i++) {
            if (commands[i].request_type == msg.type) {
                commands[i].function(msg);
                break;
            }
        }
        
        if (i == NUM_COMMANDS) {
            printf("Received unkown command %jd\n", (intmax_t) msg.type);
        }
    }
    
    // Remove message queues
    stop_server();
    
    // Free data
    array_list_free(&data);
}

void insert_func (struct message_t msg)
{
    void *new = malloc(sizeof(struct employee_t));
    memcpy(new, &msg.insert.request, sizeof(struct employee_t));
    
    if (array_list_append(&data, new)) {
        msg.insert.response.type = REQUEST_FAILURE;
    } else {
        msg.insert.response.type = REQUEST_END;
    }
    
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

void check_name_func (struct message_t msg)
{
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.check_name.request)) {
            strncpy(msg.check_name.response.name, employee->name,
                    EMPLOYEE_NAME_LENGTH);
            break;
        }
    }
    
    if (i >= data.length) {
        msg.check_name.response.type = REQUEST_FAILURE;
    } else {
        msg.check_name.response.type = REQUEST_END;
    }
    
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

void check_dept_func (struct message_t msg)
{
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.check_dept.request)) {
            strncpy(msg.check_dept.response.department, employee->department,
                    EMPLOYEE_DEPT_LENGTH);
            break;
        }
    }
    
    if (i >= data.length) {
        msg.check_dept.response.type = REQUEST_FAILURE;
    } else {
        msg.check_dept.response.type = REQUEST_END;
    }
    
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

void check_salary_func (struct message_t msg)
{
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.check_salary.request)) {
            msg.check_salary.response.salary = employee->salary;
            break;
        }
    }
    
    if (i >= data.length) {
        msg.check_salary.response.type = REQUEST_FAILURE;
    } else {
        msg.check_salary.response.type = REQUEST_END;
    }
    
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

void check_num_func (struct message_t msg)
{
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->name, msg.check_employee_num.request)) {
            strncpy(msg.check_employee_num.response.number, employee->number,
                    EMPLOYEE_NUM_LENGTH);
            break;
        }
    }
    
    if (i >= data.length) {
        msg.check_employee_num.response.type = REQUEST_FAILURE;
    } else {
        msg.check_employee_num.response.type = REQUEST_END;
    }
    
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

void list_dept_func (struct message_t msg)
{
    for (int i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->department, msg.list_dept.request)) {
            msg.list_dept.response.type = REQUEST_CONTINUE;
            strncpy(msg.list_dept.response.number, employee->number,
                    EMPLOYEE_NUM_LENGTH);
            if (send_response_to_client(msg)) {
                fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                        (intmax_t) msg.client_pid);
            }
        }
    }
    
    msg.list_dept.response.type = REQUEST_END;
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}

void delete_func (struct message_t msg)
{
    int i;
    for (i = 0; i < data.length; i++) {
        struct employee_t *employee = array_list_get(&data, i);
        if (!strcmp(employee->number, msg.delete.request)) {
            array_list_remove(&data, i);
            break;
        }
    }
    
    if (i >= data.length) {
        msg.check_employee_num.response.type = REQUEST_FAILURE;
    } else {
        msg.check_employee_num.response.type = REQUEST_END;
    }
    
    if (send_response_to_client(msg)) {
        fprintf(stderr, "Failed to send reponse to client with pid %jd\n",
                (intmax_t) msg.client_pid);
    }
}


static struct command_t commands[NUM_COMMANDS] = {
    {.request_type = REQUEST_INSERT, .function = insert_func},
    {.request_type = REQUEST_CHECK_NAME, .function = check_name_func},
    {.request_type = REQUEST_CHECK_DEPT, .function = check_dept_func},
    {.request_type = REQUEST_CHECK_SALARY, .function = check_salary_func},
    {.request_type = REQUEST_CHECK_EMPLOYEE_NUM, .function = check_num_func},
    {.request_type = REQUEST_LIST_DEPT, .function = list_dept_func},
    {.request_type = REQUEST_DELETE, .function = delete_func}
};
