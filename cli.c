/**
 * Created on 03/13/2023
 * @author Valentin Betbeze <valentin.betbeze@gmail.com>
 * 
 * This program is a simple bash-like command-line interface.
 * I developed it as my final project for the CS50x 2023 online 
 * course.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "commands.h"


int main(void)
{
	printf("**** To exit the program, type 'exit' ****\n");

	// Allocate memory for the input buffer
	char *input = malloc(SIZE_INPUT);
	if (input == NULL)
	{
		printf("Error: 'input' memory allocation failed\n");
		return 1;
	}

	// Run until the 'exit' command is entered
	do
	{
		printf("£ ");

		// Wait for input
		if (get_input(input))
		{
			Token *head = parse_input(input);
			if (head == NULL)
			{
				printf("Error: Parsing failed\n");
				continue;
			}

			int argc = get_argc(head);
			char *command = get_argv(head, 0);

			/**
			 * @note To improve if more commands are added.
			 * Search time: poor (linear).
			 * Code maintenance: medium (will get worse with more
			 * commands)
			*/ 
			if (!strcmp(command, "echo"))
			{
				echo(head, argc);
			}
			else if (!strcmp(command, "pwd"))
			{
				pwd();
			}
			else if (!strcmp(command, "ls"))
			{
				ls(head, argc);
			}
			else if (!strcmp(command, "cd"))
			{
				cd(head, argc);
			}
			else if (!strcmp(command, "touch"))
			{
				touch(head, argc);
			}
			else if (!strcmp(command, "rm"))
			{
				rm(head, argc);
			}
			else if (!strcmp(command, "mkdir"))
			{
				mkdir_cli(head, argc);
			}
			else if (!strcmp(command, "rmdir"))
			{
				rmdir_cli(head, argc);
			}
			else if (!strcmp(command, "mv"))
			{
				mv(head, argc);
			}
			else if (!strcmp(command, "cat"))
			{
				cat(head, argc);
			}
			else if (!strcmp(command, "make"))
			{
				make(head, argc);
			}
			else if (command[0] == '.')
			{
				run(head, argc);
			}
			else if (strcmp(command, "exit"))
			{
				printf("Error: %s: Unknown command\n", command);
			}
			free_tokens(head);
		}
	} while (strcasecmp(input, "exit"));

	free(input);
    return 0;
}

