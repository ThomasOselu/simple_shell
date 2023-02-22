#include "shell.h"

/**
 * clear_info - clears all members of the info struct
 * @info: pointer to the info struct to be cleared
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes an info_t struct with the necessary info
 * @info: a pointer to an info_t struct
 * @av: an array of strings containing the command-line arguments
 *
 * Return: nothing
 */
void set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0]; /* set the first element of `av` as the filename */

	if (info->arg) /* if there is a command to execute */
	{
		info->argv = strtow(info->arg, " \t"); /* split command into arguments */

		if (!info->argv) /* handle error case where strtow fails */
		{
			info->argv = malloc(sizeof(char *) * 2);

			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}

		for (i = 0; info->argv && info->argv[i]; i++) /* count number of arguments */
			;

		info->argc = i; /* set the count as argc in info struct */

		replace_alias(info); /* expand any aliases in the arguments */
		replace_vars(info); /* expand any variables in the arguments */
	}
}

/**
 * free_info - frees memory allocated for the info_t struct
 * @info: a pointer to the info_t struct to free
 * @all: flag indicating whether to free all resources (1) or only some (0)
 *
 * Return: nothing
 */
void free_info(info_t *info, int all)
{
	ffree(info->argv); /* free the argv array */

	info->argv = NULL; /* set to NULL to avoid double free */
	info->path = NULL; /* set to NULL to avoid double free */

	if (all) /* if flag is set to free all resources */
	{
		if (!info->cmd_buf) /* if cmd_buf is not allocated */
			free(info->arg); /* free the arg string */

		if (info->env) /* if env linked list is allocated */
			free_list(&(info->env)); /* free the env linked list */

		if (info->history) /* if history linked list is allocated */
			free_list(&(info->history)); /* free the history linked list */

		if (info->alias) /* if alias linked list is allocated */
			free_list(&(info->alias)); /* free the alias linked list */

		ffree(info->environ); /* free the environ array */
		info->environ = NULL; /* set to NULL to avoid double free */

		bfree((void **)info->cmd_buf); /* free the cmd_buf array */

		if (info->readfd > 2) /* if readfd is open */
			close(info->readfd); /* close the file descriptor */

		_putchar(BUF_FLUSH); /* flush stdout buffer */
	}
}
