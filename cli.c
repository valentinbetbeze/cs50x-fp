/**
 * Created on 03/13/2023
 * @author Valentin Betbeze <valentin.betbeze@gmail.com>
 * 
 * This program is a simple bash-like command line interface.
 * I developed it as my final project for the CS50x 2023 online 
 * course.
 * 
 * The program allows the user to enter command lines to operate various 
 * actions, such as listing the content of a directory, moving around
 * the directory structure, modifying the directory structure and much
 * more.
 * To do so, the program first retrieves, parses, and stores the input
 * into a linked list. It then goes through a switch case statement and 
 * checks the head's argument. If valid, the program will call and 
 * execute the command along with other arguments present in the linked
 * list.
 * @todo To complete.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// Maximum number of characters allowed for a single command line 
#define SIZE_INPUT 100
#define PATH_MAX 4096

/**
 * @brief @struct type to store parsed arguments within a linked list.
 * 
 * Each argument of a command line is parsed from the input and
 * stored in a token. The tokens are linked to each other using
 * a pointer referencing the next token.
*/
typedef struct token
{
	char argument[SIZE_INPUT];
	struct token *next;
} Token;


/**
 * int get_input(char *ptr)
 * @brief Retrieve data from the buffer and stores it in a string.
 * 
 * @param[out] ptr	Memory area to store the data to.
 * @return			An integer stating the outcome of the function.
 * @retval			0 on success.
 * 					1 on failure.
 * 
 * The function 'get_input()' accepts a pointer @p ptr as input
 * and returns an integer indicating success or failure. If the
 * input command is too long, the function will print an error
 * message and return 1.
*/
int get_input(char *ptr);

/**
 * Token *parse_input(char *ptr)
 * @brief Parse all arguments from the input using spaces as delimiters.
 * 
 * @param[in] ptr	Memory area with the data to be parsed.
 * @return			A pointer to the head of a linked list.
 * @retval			'Token' pointer on success.
 * 					NULL pointer on failure.
 * 
 * The function parse_input() accepts a pointer @p ptr as input and
 * returns a pointer of type `Token` if successful, NULL if not. The
 * purpose of the function is to parse the command line given as
 * argument, using spaces as delimiters. One exception is the presence
 * of double quotation marks, enabling the use of spaces within an
 * argument. To do so, @p marks is set to 1 when a double quotation
 * marks is found, and set back to 0 when the second one is reached.
 * During the parsing process, the function creates a linked list and
 * stores each argument in an individual `Token` variable.
*/
Token *parse_input(char *ptr);

/**
 * int get_argc(Token *head)
 * @brief Get the number of arguments for the given command line.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @return			Number of arguments.
 * 
 * The function get_argc() accepts a pointer @p head as input and
 * returns the number of arguments of the parsed command line. To do
 * so, it counts the number of non-null tokens within the linked
 * list.
*/
int get_argc(Token *head);

/**
 * char *get_argv(Token *head, int index)
 * @brief Get the value of the n-th arguments.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] index	Index of the argument.
 * @return			Character pointer of the n-th argument.
 * 
 * The function get_argv() accepts a pointer @p head as input and
 * returns the character pointer from the token of index @p n .
 * To do so, it moves through the linked list by @p n increments,
 * starting from the head.
*/
char *get_argv(Token *head, int index);

/**
 * void free_tokens(Token *head)
 * @brief Free the allocated memory of the linked list.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @return			Nothing.
 * 
 * The function free_tokens() accepts a pointer @p head as input. 
 * It frees all previously allocated memory used by the linked list
 * by going to the end of the list, and recursively freeing each
 * token as it goes back to the head.
*/
void free_tokens(Token *head);

/**
 * int is_option(char *arg)
 * @brief Check if the argument is an option.
 * 
 * @param[in] arg	Pointer to an array of characters.
 * @return			An integer stating the outcome of the function.
 * @retval			1 if the argument is an option.
 * 					0 if not.
 * 
 * The function is_option() accepts a character pointer @p arg as
 * input. It checks whether the argument is an option or not, by
 * looking for an hyphen at the beginning of the given string.
*/
int is_option(char *arg);

/**
 * void echo(Token *head, int argc)
 * @brief Display the argument(s) given as input.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function echo() accepts a pointer @p head and an integer 
 * @p argc as input. 
 * It displays the arguments following the command. The use of
 * double quotation mark is not required to print text with spaces.
*/
void echo(Token *head, int argc);

/**
 * void pwd(void)
 * @brief Print the current working directory.
 * 
 * @return			Nothing.
 * 
 * The function pwd() takes no input parameter and returns no
 * output. It displays the path of the current working directory
 * on stdout . If the path length is larger than PATH_MAX, an
 * error message is displayed instead if the path length.
*/
void pwd();

/**
 * void touch(Token *head, int argc)
 * @brief Create one or multiple files.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function touch() accepts a pointer @p head and an integer 
 * @p argc as input. It creates as many files as given arguments
 * in the current working directory. The function also sets the 
 * file(s) as read and write for 'user', and read only for
 * 'group' and 'others'.
*/
void touch(Token *head, int argc);

/**
 * void rm(Token *head, int argc)
 * @brief Remove files or folders.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function rm() accepts a pointer @p head and an integer 
 * @p argc as input. By default, it removes file(s) from the 
 * directory structure without confirmation prompt. It also
 * accepts different options as input:
 * 		-d: Enables the deletion of empty directories.
 * 		-i: Enables a confirmation prompt before deletion.
 * An error message is instead displayed on stderr if the file
 * or folder does not exist.
 * @todo update with -r once implemented.
*/
void rm(Token *head, int argc);

/**
 * void rmdir_cli(Token *head, int argc)
 * @brief Remove empty folders.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function rmdir_cli() accepts a pointer @p head and 
 * an integer @p argc as input. It removes empty-only folders
 * from the current directory. An error message is instead
 * displayed on stderr if the given folder does not exist or
 * is not empty.
*/
void rmdir_cli(Token *head, int argc);


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
		if (!get_input(input))
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
				// todo statement
			}
			else if (!strcmp(command, "cd"))
			{
				// todo statement
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
				// todo statement
			}
			else if (!strcmp(command, "rmdir"))
			{
				rmdir_cli(head, argc);
			}
			else if (!strcmp(command, "mv"))
			{
				// todo statement
			}
			else if (!strcmp(command, "cat"))
			{
				// todo statement
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


int get_input(char *ptr)
{
	char character = '\0';
	int index = 0;
	memset(ptr, 0, SIZE_INPUT);

    while ((character = getchar()) != '\n')
    {
		// The input is too long
		if (index > SIZE_INPUT - 1)
		{
			// Empty the buffer
			while (getchar() != '\n');
			printf("Error: Command size exceeded (%i characters max.)\n", SIZE_INPUT);
			return 1;
		}
		ptr[index] = character;
		index++;
    }
	// If no input
	if (!strlen(ptr))
	{
		return 1;
	}
	// Remove whitespaces (if any)
	for (int i = 0; i < strlen(ptr); i ++)
	{
		if (ptr[i] != ' ') 
		{
			break;
		}
		ptr[i] = '\0';
	}
	for (int i = strlen(ptr) - 1; i >= 0; i--)
	{
		if (ptr[i] != ' ') 
		{
			break;
		}
		ptr[i] = '\0';
	}
	return 0;
}


Token *parse_input(char *ptr)
{
	int marks = 0, parsing = 0, index_buffer = 0;

	// This buffer will be used during parsing
	char *buffer = calloc(SIZE_INPUT, sizeof(char));
	if (buffer == NULL)
	{
		printf("Error: parse_input(): Buffer memory allocation failed\n");
		return NULL;
	}

	// Create head of linked-list
	Token *head = calloc(1, sizeof(Token));
	if (head == NULL)
	{
		printf("Error: parse_input(): 'head' token creation failed\n");
		return NULL;
	}

	// Parsing loop
	for (int i = 0; i < strlen(ptr); i++)
	{
		/**
		 * If we have a space, if we were not parsing, and if we're
		 * not within quotation marks: argument found.
		*/
		if ((ptr[i] == ' ' && !parsing && !marks) ||
			(i == strlen(ptr) - 1))
		{	
			parsing = 1;
			/**
			 * Several possibilities, in order:
			 * 	1. This is the last argument of the command line
			 * 	2. This is the first argument of the command line
			 * 	3. Nor the first, nor the last argument.
			*/
			if ((i == strlen(ptr) - 1) && ptr[i] != '"')
			{
				buffer[index_buffer] = ptr[i];
			}
			/**
			 * No 'else if' here, as an argument can be both the 
			 * first and last of the command line.
			*/
			if (head->argument[0] == '\0')
			{
				strcpy(head->argument, buffer);
			}
			else
			{
				// Create a new token for the argument
				Token *new = calloc(1, sizeof(Token));
				if (new == NULL)
				{
					printf("Error: parse_input(): 'new' token creation failed\n");
					return NULL;
				}
				strcpy(new->argument, buffer);

				// Go at the end of the linked list
				Token *current = head;
				while (current->next != NULL)
				{
					current = current->next;
				}

				// Link the new token to the chain
				current->next = new;		
			}
			memset(buffer, 0, SIZE_INPUT);
			index_buffer = 0;
		}
		// Enter a double quotation mark input
		else if (ptr[i] == '"' && !marks)
		{
			marks = 1;
		}
		// Exit a double quotation mark input
		else if (ptr[i] == '"' && marks)
		{
			marks = 0;
		}
		else
		{
			// Ignore spaces except if within double quotation marks
			if (ptr[i] != ' ' || marks)
			{
				buffer[index_buffer] = ptr[i];
				index_buffer++;
				parsing = 0;
			}
		}
	}
	free(buffer);
	return head;
}


int get_argc(Token *head)
{
	int argc = 0;
	while (head != NULL)
	{
		head = head->next;
		argc++;
	}
	return argc;
}


char *get_argv(Token *head, int index)
{
	for (int i = 0; i < index; i++)
	{
		if (head->next == NULL)
		{
			printf("Error: get_argv(): index out of range\n");
			return 0;
		}
		head = head->next;
	}
	// Return the n-th argument
	return head->argument;
}


void free_tokens(Token *head)
{
	// Go to the tail
	if (head->next != NULL)
	{
		free_tokens(head->next);
	}
	free(head);
}


int is_option(char *arg)
{
	if (arg == NULL)
	{
		printf("Error: is_option(): Null pointer\n");
		return 0;
	}

	if (arg[0] == '-' && strlen(arg) > 1)
	{
		return 1;
	}
	return 0;
}


void echo(Token *head, int argc)
{
	for (int i = 1; i < argc; i++)
	{
		printf("%s ", get_argv(head, i));
	}
	printf("\n");
}


void pwd()
{
	char buf[PATH_MAX] = {0};
	// https://man7.org/linux/man-pages/man3/getcwd.3.html
	if (!getcwd(buf, PATH_MAX))
	{
		perror("Error: pwd: getcwd()");
		return;
	}
	printf("%s\n", buf);
}


void touch(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: touch: Missing operand\n");
		return;
	}
	for (int i = 1; i < argc; i++)
	{
		/**
		 * Set default rights to:
		 * 	user read:		on
		 * 	user write:		on
		 * 	group read:		on
		 * 	others read:	on
		*/
		creat(get_argv(head, i), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
}


void rm(Token *head, int argc)
{
	int confirmation = 0, directory = 0, remove_all = 0;

	if (argc < 2)
	{
		printf("Error: rm: Missing operand\n");
		return;
	}
	// Check options
	for (int i = 1; i < argc; i++)
	{
		char *argument = get_argv(head, i);
		if (is_option(argument))
		{
			for (int j = 1; j < strlen(argument); j++)
			{
				switch (argument[j])
				{
					case 'i':
						confirmation = 1;
						break;
					case 'd':
						directory = 1;
						break;
					case 'r':
						remove_all = 1;
						break;
					default:
						printf("Error: '%s': Invalid option\n", argument);
						return;
				}
			}
		}
	}

	if (confirmation)
	{
		for (int i = 1; i < argc; i++)
		{
			char *argument = get_argv(head, i);
			if (!is_option(argument))
			{
				if (directory)
				{
					printf("Warning: Remove folder\t'%s'?\n", argument);
				}
				else
				{
					printf("Warning: Remove file\t'%s'?\n", argument);
				}
			}
		}
		printf("->[y/N] ");
		// Wait for confirmation
		if (getchar() != 'y')
		{
			return;
		}
	}

	if (remove_all)
	{
		// Recursively remove all files and subdirectories
		// todo
		// need ls, getcwd
		return;
	}

	for (int i = 1; i < argc; i++)
	{
		char *argument = get_argv(head, i);
		if (!is_option(argument))
		{
			if (directory)
			{
				char path[PATH_MAX] = {0};
				snprintf(path, sizeof(path), "./%s", argument);
				// Remove folder
				if (rmdir(path) == -1)
				{
					fprintf(stderr, "Error: Failed to remove '%s': ", argument);
					perror("");
					return;
				}
			}
			// Remove file
			else if (unlink(argument) == -1)
			{
				fprintf(stderr, "Error: Failed to remove '%s': ", argument);
				perror("");
				return;
			}
		}
	}
	return;
}


void rmdir_cli(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: rmdir: Missing operand\n");
		return;
	}
	for (int i = 1; i < argc; i++)
	{
		char path[PATH_MAX] = {0};
		char *argument = get_argv(head, i);
		snprintf(path, sizeof(path), "./%s", argument);
		if (rmdir(path) == -1)
		{
			fprintf(stderr, "Error: Failed to remove '%s': ", argument);
			perror("");
			return;
		}
	}
	return;
}

