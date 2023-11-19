#include "../KERN/SYSCALL.H"
#include "../TYPES.H"
#include "USRPRG.H"

const Vector prog[] = {
	shell,
	hello,
	user_reboot,
	user_program_3,
	user_program_4
};

void shell()
{
	char buf[SHELL_BUF_LEN];
	int command;

	while (1)
	{
		write("\r\n$ ", 4);
		read(buf, SHELL_BUF_LEN);

		command = *buf - '0';

		if (command > 0 && command < 5)
		{
			create_process(command, 0);
			yield(); /* [TO DO] wait */
		}
	}
}

void hello()
{
	char pid = '0' + (char)get_pid();

	write("\r\nhello, world from ", 20);
	write(&pid, 1);
	write("!\r\n", 3);
	exit();
}

void user_reboot()
{
	reboot();
}

void user_program_3()
{
	volatile UINT32 i;
	int printed = 0;

	while (printed < 40)
	{
		write("C", 1);
		printed++;

		for (i = 0; i < 10000L; i++)
			;
	}

	exit();
}

void user_program_4()
{
	volatile UINT32 i;
	int printed = 0;

	while (1)
	{
		for (i = 0; i < 500000L; i++)
			;

		write("D", 1);

		if (++printed == 10) {
			printed/0;
		}
	}
}

