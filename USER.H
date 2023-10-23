#ifndef USER_H
#define USER_H

#include "types.h"

#define SHELL_BUF_LEN 200

void shell();
void hello();
void user_program_2();
void user_program_3();
void user_program_4();

const Vector prog[] =
{
	shell,
	hello,
	user_program_2,
	user_program_3,
	user_program_4
};

#endif /* USER_H */

