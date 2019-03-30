#include <sys/types.h>

#ifndef msg_format_h
#define msg_format_h

#define EMPLOYEE_NAME_LENGTH    12
#define EMPLOYEE_DEPT_LENGTH    12
#define EMPLOYEE_NUM_LENGTH     12


enum request_type {
    REQUEST_INSERT,
    REQUEST_CHECK_NAME,
    REQUEST_CHECK_DEPT,
    REQUEST_CHECK_SALARY,
    REQUEST_CHECK_EMPLOYEE_NUM,
    REQUEST_LIST_DEPT,
    REQUEST_DELETE
};

enum server_response_type {
    REQUEST_FAILURE,
    REQUEST_CONTINUE,
    REQUEST_END
};

struct employee_t {
    char name[EMPLOYEE_NAME_LENGTH];
    char department[EMPLOYEE_DEPT_LENGTH];
    char number[EMPLOYEE_NUM_LENGTH];
    int salary;
};

struct message_t {
    pid_t client_pid;
    enum request_type type;
    
    union {
        union {
            struct employee_t request;
            struct {
                enum server_response_type type;
            } response;
        } insert;
        
        union {
            char request[EMPLOYEE_NUM_LENGTH];
            struct {
                char name[EMPLOYEE_NAME_LENGTH];
                enum server_response_type type;
            } response;
        } check_name;
        
        union {
            char request[EMPLOYEE_NUM_LENGTH];
            struct {
                char department[EMPLOYEE_DEPT_LENGTH];
                enum server_response_type type;
            } response;
        } check_dept;
        
        union {
            char request[EMPLOYEE_NUM_LENGTH];
            struct {
                int salary;
                enum server_response_type type;
            } response;
        } check_salary;
        
        union {
            char request[EMPLOYEE_NAME_LENGTH];
            struct {
                char number[EMPLOYEE_NUM_LENGTH];
                enum server_response_type type;
            } response;
        } check_employee_num;
        
        union {
            char request[EMPLOYEE_DEPT_LENGTH];
            struct {
                char number[EMPLOYEE_NUM_LENGTH];
                enum server_response_type type;
            } response;
        } list_dept;
        
        union {
            char request[EMPLOYEE_NUM_LENGTH];
            struct {
                enum server_response_type type;
            } response;
        } delete;
    };
};

#endif /* msg_format_h */
