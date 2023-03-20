// Implement the input command functions

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/wait.h>

#include "commands.h"


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
			for (int j = 1; j < (int) strlen(argument); j++)
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
		 * 	user execute:	on
		 * 	group read:		on
		 * 	others read:	on
		*/
		creat(get_argv(head, i), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
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
			for (int j = 1; j < (int) strlen(argument); j++)
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


void make(Token *head, int argc)
{
	if (argc < 2)
	{
		printf("Error: Missing operand\n");
		return;
	}

	char *extension = malloc(SIZE_INPUT * sizeof(char));
	if (extension == NULL)
	{
		printf("Error: Memory allocation failed\n");
		return;
	}

	// Compile each source file
	for (int i = 1; i < argc; i++)
	{
		memset(extension, 0, SIZE_INPUT);
		/**
		 * This pointer will be dynamic. It will help store each
		 * character of the extension while not losing the original
		 * adress.
		*/
		char *ptr = extension;

		bool ext_exist = false;
		char name[SIZE_INPUT] = {0};

		char *full_name = get_argv(head, i);
		int full_name_len = (int) strlen(full_name);
		
		// Check if .c file
		for (int j = 0; j < full_name_len; j++)
		{
			if (full_name[j] == '.' || ext_exist)
			{
				ext_exist = true;
				*ptr = full_name[j];
				ptr++;
			}
			else
			{
				// Get the file name only (will be the executable's name)
				name[j] = full_name[j];
			}
		}

		if (strcmp(extension, ".c"))
		{
			printf("Error: %s is not a C source file\n", full_name);
			continue;
		}

		char command[PATH_MAX] = {0};
		snprintf(command, sizeof(command), "gcc -o %s %s\n", name, full_name);

		if (system(command) == -1)
		{
			free(extension);
			perror("Error: ");
			return;
		}
	}
	free(extension);
}


void run(Token *head, int argc)
{
	char *command = get_argv(head, 0);
	if (command == NULL)
	{
		printf("Error: Cannot access command\n");
		return;
	}

	// Create array of string for command-line arguments
	char **argv = calloc(argc + 1, sizeof(char *));
	if (argv == NULL)
	{
		printf("Error: Memory allocation failed\n");
		return;
	}

	// Get command-line arguments
	for (int i = 0; i < argc; i++)
	{
		argv[i] = (char *) calloc(SIZE_INPUT, 1);
		if (argv[i] == NULL)
		{
			// If memory alloc failed, free all previous memory
			printf("Error: Memory allocation failed\n");
			for (int j = 0; j < i; j++)
			{
				free(argv[j]);
			}
			free(argv);
			return;
		}
		strcpy(argv[i], get_argv(head, i));
	}
	// Last argument must be NULL for execv() to work
	argv[argc] = NULL;

	// Run the executable
	pid_t pid = fork();
	if (pid < 0)
	{
		// Forking failed
		perror("Error: fork: ");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		// Child process
		if (execv(command, argv) == -1)
		{
			perror("Error: execv: ");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		// Wait for the child process to complete
		pid = wait(NULL);
	}

	// Free all memory
	for (int i = 0; i < argc; i++)
	{
		free(argv[i]);
	}
	free(argv);
}

