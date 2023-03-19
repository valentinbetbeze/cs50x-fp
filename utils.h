/**
 * Utility functions declarations
 * Macros, structs and custom types
*/
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

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
bool is_option(const char *arg);


/**
 * void recursive_deletion(char *path)
 * @brief Delete the folder given in path, as well as all of its
 * content.
 * 
 * @param[in] path	Path to the folder to remove.
 * @return			Nothing.
 * 
 * The function recursive_deletion() accepts a character pointer
 * @p path as input. It recursively deletes a folder and all of
 * its content. The function does not have any confirmation prompt 
 * mechanism. Use carefully.
*/
void recursive_deletion(const char *path);


#endif // UTILS_H