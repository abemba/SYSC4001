#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg_format.h"
#include "msg_queue.h"

/** Descriptor for command */
struct command_t {
    char *name;
    void (*function)(void);
};

#define NUM_COMMANDS 8
static struct command_t commands[];

/** Message buffer */
static struct message_t msg;


/**
 *  Get a single line from stdin.
 *
 *  @param buffer The buffer in which the line should be stored
 *  @param buf_len The size of the buffer
 *
 *  @return 0 if successfull, a possitive integer otherwise
 */
static int get_line (char *buffer, int buf_len)
{
    char *retval;
    retval = fgets(buffer, buf_len, stdin);
    
    if (!retval) {
        // fgets failed
        return 1;
    }
    
    if ((retval = strchr(buffer, '\n')) == 0) {
        // There is no new line in the input, the buffer was filled
        return 1;
    }
    
    // Discard newline
    *retval = '\0';
    
    return 0;
}


int main (int argc, char **argv)
{
    // Get message queues
    if (start_client()) {
        fprintf(stderr, "Failed to connect to server.\n");
        exit(1);
    }
    
    // Prepare message buffer
    msg.client_pid = getpid();
    
    // Main loop
    char buf[128];
    
    for (;;) {
        printf("> ");
        if (!get_line(buf, 128) && (strlen(buf) > 0)) {
            int i;
            for (i = 0; i < NUM_COMMANDS; i++) {
                if (!strcasecmp(buf, commands[i].name)) {
                    commands[i].function();
                    break;
                }
            }
            if (i == NUM_COMMANDS) {
                printf("Unkown Command\n");
            }
        }
    }
}

/**
 *  Get a string from the user. Re-prompt the user until they enter a valid string.
 *
 *  @param propmt The prompt string
 *  @param result Buffer in which the resulting string should be placed
 *  @param res_length The length of the result buffer
 */
static void get_str (const char *prompt, char *result, int res_length)
{
    char buffer[res_length + 1];
    printf("%s: ", prompt);
    
    for (;;) {
        if (!get_line(buffer, res_length + 1)) {
            if (strlen(buffer)) {
                break;
            } else {
                printf("%s: ", prompt);
            }
        } else {
            printf("%ss can be at most %d characters: ", prompt,
                   (res_length - 1));
            // Flush stdin
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
    
    strncpy(result, buffer, res_length);
}

/**
 *  Function to handle insert command
 */
void insert_func (void)
{
    // Set up message
    msg.type = REQUEST_INSERT;
    
    // Get employee information
    get_str("Name", msg.insert.request.name, EMPLOYEE_NAME_LENGTH);
    get_str("Department", msg.insert.request.department, EMPLOYEE_DEPT_LENGTH);
    get_str("Employee number", msg.insert.request.number, EMPLOYEE_NUM_LENGTH);
    
    char buffer[11];
    printf("Salary: ");
    for (;;) {
        if (!get_line(buffer, sizeof(buffer))  &&
            (sscanf(buffer, " %d", &msg.insert.request.salary) == 1)) {
            break;
        } else {
            printf("Please enter a valid integer: ");
        }
    }
    
    // Send to server and get response
    send_message_to_server(msg);
    get_response_from_server(&msg);
    
    if (msg.insert.response.type == REQUEST_FAILURE) {
        printf("Failed to insert employee record.\n");
    }
}

/**
 *  Function to handle check name command
 */
void check_name_func (void)
{
    // Set up message
    msg.type = REQUEST_CHECK_NAME;
    
    // Get employee information
    get_str("Employee number", msg.check_name.request, EMPLOYEE_NUM_LENGTH);
    
    // Send to server
    send_message_to_server(msg);
    get_response_from_server(&msg);
    
    if (msg.check_name.response.type == REQUEST_FAILURE) {
        printf("No employee with number %s found.\n",
               msg.check_name.request);
    } else {
        printf("%s\n", msg.check_name.response.name);
    }
}

/**
 *  Function to handle check department command
 */
void check_dept_func (void)
{
    // Set up message
    msg.type = REQUEST_CHECK_DEPT;
    
    // Get employee information
    get_str("Employee number", msg.check_dept.request, EMPLOYEE_NUM_LENGTH);
    
    // Send to server
    send_message_to_server(msg);
    get_response_from_server(&msg);
    
    if (msg.check_dept.response.type == REQUEST_FAILURE) {
        printf("No employee with number %s found.\n",
               msg.check_dept.request);
    } else {
        printf("%s\n", msg.check_dept.response.department);
    }
}

/**
 *  Function to handle check salary command
 */
void check_salary_func (void)
{
    // Set up message
    msg.type = REQUEST_CHECK_SALARY;
    
    // Get employee information
    get_str("Employee number", msg.check_salary.request, EMPLOYEE_NUM_LENGTH);
    
    // Send to server
    send_message_to_server(msg);
    get_response_from_server(&msg);
    
    if (msg.check_salary.response.type == REQUEST_FAILURE) {
        printf("No employee with number %s found.\n",
               msg.check_salary.request);
    } else {
        printf("$%d\n", msg.check_salary.response.salary);
    }
}

/**
 *  Function to handle check employee number command
 */
void check_num_func (void)
{
    // Set up message
    msg.type = REQUEST_CHECK_EMPLOYEE_NUM;
    
    // Get employee information
    get_str("Employee name", msg.check_employee_num.request,
            EMPLOYEE_NAME_LENGTH);
    
    // Send to server
    send_message_to_server(msg);
    get_response_from_server(&msg);
    
    if (msg.check_employee_num.response.type == REQUEST_FAILURE) {
        printf("No employee with name \"%s\" found.\n",
               msg.check_employee_num.request);
    } else {
        printf("%s\n", msg.check_employee_num.response.number);
    }
}

/**
 *  Function to handle list department command
 */
void list_dept_func (void)
{
    // Set up message
    msg.type = REQUEST_LIST_DEPT;
    
    // Get employee information
    get_str("Department", msg.list_dept.request, EMPLOYEE_DEPT_LENGTH);
    
    // Send to server
    send_message_to_server(msg);
    get_response_from_server(&msg);
    
    while (msg.list_dept.response.type == REQUEST_CONTINUE) {
        printf("%s\n", msg.list_dept.response.number);
        get_response_from_server(&msg);
    }
    
    if (msg.list_dept.response.type == REQUEST_FAILURE) {
        printf("Could not list employees of department \"%s\".\n",
               msg.list_dept.request);
    }
}

/**
 *  Function to handle delete command
 */
void delete_func (void)
{
    // Set up message
    msg.type = REQUEST_DELETE;
    
    // Get employee information
    get_str("Employee number", msg.delete.request, EMPLOYEE_NUM_LENGTH);
    
    // Send to server
    send_message_to_server(msg);
    get_response_from_server(&msg);
    
    if (msg.insert.response.type == REQUEST_FAILURE) {
        printf("Failed to delete employee record.\n");
    }
}

/**
 *  Function to handle quit command
 */
void quit_func (void)
{
    stop_client();
    exit(0);
}

/** List of command descriptors */
static struct command_t commands[NUM_COMMANDS] = {
    {.name = "insert", .function = insert_func},
    {.name = "check name", .function = check_name_func},
    {.name = "check dept", .function = check_dept_func},
    {.name = "check salary", .function = check_salary_func},
    {.name = "check employee num", .function = check_num_func},
    {.name = "list dept", .function = list_dept_func},
    {.name = "delete", .function = delete_func},
    {.name = "quit", .function = quit_func}
};
