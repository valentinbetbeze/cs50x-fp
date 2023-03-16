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

// Maximum number of characters allowed for a single command line 
#define SIZE_INPUT 100


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
 * The function `get_input()` accepts a pointer @p ptr as input
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
 * @retval			`Token` pointer on success.
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


int main(void)
{
	printf("**** To exit the program, type 'exit' ****\n");

	// Allocate memory for the input buffer
	char *input = malloc(SIZE_INPUT);
	if (input == NULL)
	{
		printf("Error: 'input' memory allocation failed.\n");
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
				printf("Error: Parsing failed.\n");
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
				/// @todo statement
			}
			else if (!strcmp(command, "ls"))
			{
				/// @todo statement
			}
			else if (!strcmp(command, "cd"))
			{
				/// @todo statement
			}
			else if (!strcmp(command, "touch"))
			{
				/// @todo statement
			}
			else if (!strcmp(command, "rm"))
			{
				/// @todo statement
			}
			else if (!strcmp(command, "mkdir"))
			{
				/// @todo statement
			}
			else if (!strcmp(command, "rmdir"))
			{
				/// @todo statement
			}
			else if (!strcmp(command, "mv"))
			{
				/// @todo statement
			}
			else if (!strcmp(command, "cat"))
			{
				/// @todo statement
			}
			else if (strcmp(command, "exit"))
			{
				printf("Error: %s: unknown command\n", command);
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
	int marks = 0, index_buffer = 0;

	// This buffer will be used during parsing
	char *buffer = calloc(SIZE_INPUT, sizeof(char));
	if (buffer == NULL)
	{
		printf("Error: 'buffer' memory allocation failed.\n");
		return NULL;
	}

	// Create head of linked-list
	Token *head = calloc(1, sizeof(Token));
	if (head == NULL)
	{
		printf("Error: 'head' token creation failed.\n");
		return NULL;
	}

	// Parsing loop
	for (int i = 0; i < strlen(ptr); i++)
	{
		// Argument found
		if ((ptr[i] == ' ' && !marks) || (i == strlen(ptr) - 1))
		{	
			// Include character if it is the last argument of the command
			if ((i == strlen(ptr) - 1) && ptr[i] != '"')
			{
				buffer[index_buffer] = ptr[i];
			}

			// Add the first parsed argument to the head of the linked list
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
					printf("Error: 'new' token creation failed.\n");
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
			buffer[index_buffer] = ptr[i];
			index_buffer++;
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
			printf("Error: index out of range.\n");
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


void echo(Token *head, int argc)
{
	for (int i = 1; i < argc; i++)
	{
		printf("%s ", get_argv(head, i));
	}
	printf("\n");
}