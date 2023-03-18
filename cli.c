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
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/types.h>

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
 * bool get_input(char *ptr)
 * @brief Retrieve data from the buffer and stores it in a string.
 * 
 * @param[out] ptr	Memory area to store the data to.
 * @return			A boolean stating the outcome of the function.
 * @retval			true on success.
 * 					false on failure.
 * 
 * The function 'get_input()' accepts a pointer @p ptr as input
 * and returns an integer indicating success or failure. If the
 * input command is too long, the function will print an error
 * message and return false.
*/
bool get_input(char *ptr);

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
 * bool is_option(char *arg)
 * @brief Check if the argument is an option.
 * 
 * @param[in] arg	Pointer to an array of characters.
 * @return			A boolean stating the outcome of the function.
 * @retval			true if the argument is an option.
 * 					false if not.
 * 
 * The function is_option() accepts a character pointer @p arg as
 * input. It checks whether the argument is an option or not, by
 * looking for an hyphen at the beginning of the given string.
*/
bool is_option(char *arg);

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
 * void ls(Token *head, int argc)
 * @brief Print the content of the working directory.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function touch() accepts a pointer @p head and an integer 
 * @p argc as input. It displays the content of the current
 * working directory. By default, it does not show hidden files.
 * The function allows the input of 2 options:
 * 		-a: Enables the display of hidden files.
 * 		-l: Enables the display of extra data.
*/
void ls(Token *head, int argc);

/**
 * void cd(Token *head, int argc)
 * @brief Change the current working directory.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function cd() accepts a pointer @p head and an integer 
 * @p argc as input. It changes the current working directory
 * to the one embedded in the second token. Both absolute and
 * relative path can be given. An error message is displayed
 * if the desired working directory does not exist or is
 * unreachable. 
*/
void cd(Token *head, int argc);

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
 * void recursive_deletion(char *path)
 * @brief Delete the folder as well as all of its content.
 * 
 * @param[in] path	Path to the folder to remove
 * @return			Nothing.
 * 
 * The function recursive_deletion() accepts a character pointer
 * @p path as input. It recursively deletes a folder and all of
 * its content. The function does not have any confirmation prompt 
 * mechanism. Use carefully.
*/
void recursive_deletion(char *path);

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
 * 		-r: Enables to deletion of a folder and its content.
 * An error message is instead displayed on stderr if the file
 * or folder does not exist.
*/
void rm(Token *head, int argc);

/**
 * void mkdir_cli(Token *head, int argc)
 * @brief Create an empty folder.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function mkdir_cli() accepts a pointer @p head and 
 * an integer @p argc as input. It creates an empty folder
 * from the current directory. An error message is instead
 * displayed on stderr if the folder cannot be created.
*/
void mkdir_cli(Token *head, int argc);

/**
 * void rmdir_cli(Token *head, int argc)
 * @brief Remove an empty folder.
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

/**
 * void mv(Token *head, int argc)
 * @brief Rename a file or change its location.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function mv() accepts a pointer @p head and an integer
 * @p argc as input. It will either rename or move the 
 * location of a file depending on the arguments found in the
 * linked list. mv() makes use of the function rename() from
 * <stdio.h> to execute the command and ensure error handling.
*/
void mv(Token *head, int argc);

/**
 * void cat(Token *head, int argc)
 * @brief Display the content of a file.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function cat() accepts a pointer @p head and an integer
 * @p argc as input. It displays the content of a file on the
 * standard output. Several filenames can be given as arguments.
 * An error message is instead displayed if the file cannot be
 * found or open.
*/
void cat(Token *head, int argc);


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


bool get_input(char *ptr)
{
	char character = '\0';
	int index = 0;
	memset(ptr, 0, SIZE_INPUT);

	// Fetch input
    while ((character = getchar()) != '\n')
    {
		// The input is too long
		if (index > SIZE_INPUT - 1)
		{
			// Empty the buffer
			while (getchar() != '\n');
			printf("Error: Command size exceeded (%i characters max.)\n", SIZE_INPUT);

			return false;
		}
		ptr[index] = character;
		index++;
    }
	int len = strlen(ptr);

	// If no input
	if (!len)
	{
		return false;
	}

	/**
	 * Input post-process.
	 * 1st 'for' loop: Getting first character position
	 * 2nd 'for' loop: Sorting all characters to the left of the array
	 * 3rd 'for' loop: Removing trailing whitespaces
	*/
	int start_index = 0;
	for (int i = 0; i < len; i++)
	{
		if (ptr[i] != ' ') 
		{
			start_index = i;
			break;
		}
	}
	for (int i = start_index; i < len + start_index; i++)
	{
		if (i > len + 1)
		{
			ptr[i - start_index] = ' ';
			continue;
		}
		ptr[i - start_index] = ptr[i];
	}
	for (int i = strlen(ptr) - 1; i >= 0; i--)
	{
		if (ptr[i] != ' ') 
		{
			break;
		}
		ptr[i] = '\0';
	}
	return true;
}


Token *parse_input(char *ptr)
{
	bool marks = false, parsing = false;
	int index_buffer = 0;

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
			parsing = true;
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
			marks = true;
		}
		// Exit a double quotation mark input
		else if (ptr[i] == '"' && marks)
		{
			marks = false;
		}
		else
		{
			// Ignore spaces except if within double quotation marks
			if (ptr[i] != ' ' || marks)
			{
				buffer[index_buffer] = ptr[i];
				index_buffer++;
				parsing = false;
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


bool is_option(char *arg)
{
	if (arg == NULL)
	{
		printf("Error: is_option(): Null pointer\n");
		return false;
	}

	if (arg[0] == '-' && strlen(arg) > 1)
	{
		return true;
	}
	return false;
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
	char curdir[PATH_MAX] = {0};
	// https://man7.org/linux/man-pages/man3/getcwd.3.html
	if (!getcwd(curdir, PATH_MAX))
	{
		perror("Error: getcwd()");
		return;
	}
	printf("%s\n", curdir);
}


void ls(Token *head, int argc)
{
	bool invisible = false, details = false, flag = false;

	for (int i = 1; i < argc; i++)
	{
		char *argument = get_argv(head, i);
		if (is_option(argument))
		{
			for (int j = 1; j < strlen(argument); j++)
			{
				switch (argument[j])
				{
					case 'l':
						details = true;
						break;
					case 'a':
						invisible = true;
						break;
				}
			}
		}
	}

	DIR *dir = opendir("./");
	if (dir == NULL)
	{
		printf("Error: Cannot open directory\n");
		return;
	}

	struct dirent *entry = readdir(dir);
	if (entry == NULL)
	{
		printf("Error: Cannot read directory\n");
		return;
	}

	struct stat buf;
	while (entry != NULL)
	{
		if (!invisible && (entry->d_name)[0] == '.')
		{
			entry = readdir(dir);
			continue;
		}

		if (details)
		{
			// Flag to display header once only
			if (!flag)
			{
				printf("mode\t\tsize\tname\n");
				flag = true;
			}
			/// @note More information could be displayed
			stat(entry->d_name, &buf);
			printf("%c%c%c%c%c%c%c%c%c\t",
				(S_ISDIR(buf.st_mode)) ? 'd' : '-',
				(buf.st_mode & S_IRUSR) ? 'r' : '-',
				(buf.st_mode & S_IWUSR) ? 'w' : '-',
				(buf.st_mode & S_IXUSR) ? 'x' : '-',
				(buf.st_mode & S_IRGRP) ? 'r' : '-',
				(buf.st_mode & S_IWGRP) ? 'w' : '-',
				(buf.st_mode & S_IXGRP) ? 'x' : '-',
				(buf.st_mode & S_IROTH) ? 'r' : '-',
				(buf.st_mode & S_IWOTH) ? 'w' : '-');
			printf("%li\t", buf.st_size);
		}
		printf("%s\n", entry->d_name);
		entry = readdir(dir);
    }
	closedir(dir);
}


void cd(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: Missing operand\n");
		return;
	}
	else if (argc > 2)
	{
		printf("Error: Too many arguments\n");
		return;
	}

	char *path = get_argv(head, 1);

	// If an absolute path is given
	if (path[0] == '/')
	{
		if (chdir(path))
		{
			perror("Error: chdir()");
			return;
		}
	}
	else
	{
		char new_path[PATH_MAX] = {0};
		snprintf(new_path, sizeof(new_path), "./%s", path);
		if (chdir(new_path))
		{
			fprintf(stderr, "Error: %s: ", path);
			perror("");
			return;
		}
	}
}


void touch(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: Missing operand\n");
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
		creat(get_argv(head, i), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
	}
}


void recursive_deletion(char *path)
{
	DIR *dir = opendir(path);
	if (dir == NULL)
	{
		printf("Error: Cannot open directory: %s\n", path);
		return;
	}

	struct dirent *entry = readdir(dir);
	if (entry == NULL)
	{
		printf("Error: Cannot read directory\n");
		return;
	}

	// For each element of the top folder
	struct stat buf;
	while (entry != NULL)
	{
		/* Concatenate the path with the file name to have the full path of the file/folder */ 
		char new_path[PATH_MAX];
		snprintf(new_path, PATH_MAX, "%s/%s", path, entry->d_name);
		stat(new_path, &buf);
		// If the element is a directory, except the system-specific directories '.' and '..'
		if (S_ISDIR(buf.st_mode)
			&& strcmp(entry->d_name, ".")
			&& strcmp(entry->d_name, ".."))
		{
			// Delete by recursion
			recursive_deletion(new_path);
		}
		else if (!S_ISDIR(buf.st_mode))
		{
			if (unlink(new_path) == -1)
			{
				fprintf(stderr, "Error: Failed to remove '%s': ", entry->d_name);
				perror("");
				return;
			}
		}
		entry = readdir(dir);
    }

	// Finally, delete the top directory
	closedir(dir);
	if (rmdir(path) == -1)
	{
		perror("");
		return;
	}
}


void rm(Token *head, int argc)
{
	bool confirmation = false, directory = false, remove_all = false;

	if (argc < 2)
	{
		printf("Error: Missing operand\n");
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
						confirmation = true;
						break;
					case 'd':
						directory = true;
						break;
					case 'r':
						remove_all = true;
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
				printf("Warning: Remove \t'%s'?\n", argument);
			}
		}
		printf("->[y/N] ");
		// Wait for confirmation
		if (getchar() != 'y')
		{
			return;
		}
	}

	for (int i = 1; i < argc; i++)
	{
		char *argument = get_argv(head, i);
		if (!is_option(argument))
		{
			char path[PATH_MAX] = {0};
			snprintf(path, sizeof(path), "./%s", argument);

			if (remove_all)
			{
				recursive_deletion(path);
				continue;
			}

			if (directory)
			{
				if (rmdir(path) == -1)
				{
					fprintf(stderr, "Error: Failed to remove '%s': ", argument);
					perror("");
					return;
				}
				continue;
			}

			if (unlink(argument) == -1)
			{
				fprintf(stderr, "Error: Failed to remove '%s': ", argument);
				perror("");
				return;
			}
		}
	}
}


void mkdir_cli(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: Missing operand\n");
		return;
	}
	// Create folders given as argument
	for (int i = 1; i < argc; i++)
	{
		char path[PATH_MAX] = {0};
		char *argument = get_argv(head, i);
		snprintf(path, sizeof(path), "./%s", argument);
		if (mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH) == -1)
		{
			fprintf(stderr, "Error: Failed to create '%s': ", argument);
			perror("");
			return;
		}
	}
}


void rmdir_cli(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: Missing operand\n");
		return;
	}
	// Remove folders given as argument
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
}


void mv(Token *head, int argc)
{
	if (argc < 3)
	{
		printf("Error: Missing operand\n");
		return;
	}
	else if (argc > 3)
	{
		printf("Error: Too many arguments\n");
		return;
	}
	
	char *old_name = get_argv(head, 1);
	char *new_name = get_argv(head, 2);

	if(rename(old_name, new_name))
	{
		fprintf(stderr, "Error: '%s': ", old_name);
		perror("");
		return;
	}
}


void cat(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: Missing operand\n");
		return;
	}

	for (int i = 1; i < argc; i++)
	{
		char *argument = get_argv(head, i);

		FILE *file = fopen(argument, "r");
		if (file == NULL)
		{
			printf("Error: Could not open %s\n", argument);
			return;
		}

		// Transfer data from stream to stdout
		char character = '\0';
		while((character = fgetc(file)) != EOF)
		{
			if (putchar(character) == EOF)
			{
				printf("Error: Could not write to standard output\n");
				fclose(file);
				return;
			}
		}
		putchar('\n');
		fclose(file);
	}
}

