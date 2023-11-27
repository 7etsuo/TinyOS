#ifndef SYSCALL_H
#define SYSCALL_H

/* [TODO] Delete */

/**
 * @defgroup SystemCalls System Call Vectors
 * 
 * @brief Exception vectors corresponding to system calls.
 * 
 * The table outlines the exception vectors used for various system calls.
 * 
 * | Vector Number | System Call Number | Name               | Details                                 |
 * |---------------|--------------------|--------------------|-----------------------------------------|
 * | 32            | 0                  | reboot             |                                         |
 * | 33            | 1                  | exit               |                                         |
 * | 34            | 2                  | create_process     |                                         |
 * | 35            | 3                  | write              |                                         |
 * | 36            | 4                  | read               |                                         |
 * | 37            | 5                  | get_pid            |                                         |
 * | 38            | 6                  | (reserved)         | reserved for SCI expansion adjust priority? |
 * | 39            | 7                  | yield              |                                         |
 * | 40            | 8                  | wait               | not implemented                          |
 * | 41            | 9                  | (reserved)         | reserved for SCI expansion               |
 * | 42            | 10                 | semaphore operation| not implemented                          |
 * | 43            | 11                 | (reserved)         | reserved for SCI expansion               |
 * | 44            | 12                 | open               | not implemented                          |
 * | 45            | 13                 | close              | not implemented                          |
 * | 46            | 14                 | create_file        | not implemented                          |
 * | 47            | 15                 | delete_file        | not implemented                          |
 * 
 */

#endif /* SYSCALL_H */

