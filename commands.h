// Input commands for the CLI

#ifndef COMMANDS_H
#define COMMANDS_H

#include "utils.h"


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

/**
 * void make(Token *head, int argc)
 * @brief Create the executable of a .c source file.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function make() accepts a pointer @p head and an integer
 * @p argc as input. It compiles a source code file in c language
 * using the GCC compiler and creates the executable. Several
 * source code files can be given as arguments, as long as they're
 * in c language.
*/
void make(Token *head, int argc);

/**
 * void run(Token *head, int argc)
 * @brief Execute a program file.
 * 
 * @param[in] head	Memory area where the parsed data is.
 * @param[in] argc	Number of arguments.
 * @return			Nothing.
 * 
 * The function run() accepts a pointer @p head and an integer
 * @p argc as input. It runs a program file as long as the file
 * is an executable. The function use the first argument as path
 * to the file, and the other arguments, if any, as arguments
 * themselves to the given executable file.
*/
void run(Token *head, int argc);


#endif // COMMANDS_H