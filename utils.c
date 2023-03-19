// Utility functions defintions

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include "utils.h"


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