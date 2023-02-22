#include "shell.h"

/**
 * get_history_file - concatenates the HOME directory 
 * path with the history file name
 * @info: a pointer to the info_t struct containing the HOME directory path
 *
 * Return: a pointer to a string containing the path 
 * to the history file, or NULL on failure
 */
char *get_history_file(info_t *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME="); /* get the HOME directory path */
	if (!dir)
		return (NULL);
        
        /* allocate memory for the concatenated string */
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);

	buf[0] = 0; /* set the first character to NULL terminator */
	_strcpy(buf, dir); /* copy the directory path to the buffer */
	_strcat(buf, "/"); /* append a forward slash */
	_strcat(buf, HIST_FILE); /* append the history file name */

	return (buf); /* return the buffer */
}

/**
 * write_history - writes the contents of the history list to a file
 * @info: a pointer to the info_t struct containing the history list
 *
 * Return: 1 on success, -1 on failure
 */
int write_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644); /* open the file for writing */
	free(filename); /* free the allocated filename string */
	if (fd == -1)
		return (-1);

	for (node = info->history; node; node = node->next) /* iterate through the history list */
	{
		_putsfd(node->str, fd); /* write the command string to the file */
		_putfd('\n', fd); /* add a newline character */
	}
	_putfd(BUF_FLUSH, fd); /* flush the output buffer */
	close(fd); /* close the file */

	return (1);
}

/**
 * read_history - read history from a file and build a linked list
 * @info: shell info struct
 *
 * Return: number of history nodes built
 */
int read_history(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat file_stat;
	char *file_buf = NULL, *filename = get_history_file(info);

	/* check if history file is present */
	if (!filename)
		return (0);

	/* open history file */
	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);

	/* get file size */
	if (!fstat(fd, &file_stat))
		fsize = file_stat.st_size;

	/* read file into buffer */
	if (fsize < 2)
		return (0);
	file_buf = malloc(sizeof(char) * (fsize + 1));
	if (!file_buf)
		return (0);
	rdlen = read(fd, file_buf, fsize);
	file_buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(file_buf), 0);
	close(fd);

	/* build history list from buffer */
	for (i = 0; i < fsize; i++)
	{
		if (file_buf[i] == '\n')
		{
			file_buf[i] = 0;
			build_history_list(info, file_buf + last, linecount++);
			last = i + 1;
		}
	}
	if (last != i)
		build_history_list(info, file_buf + last, linecount++);
	free(file_buf);

	/* trim history to HIST_MAX and renumber nodes */
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);
	remember_history(info);

	return (info->histcount);
}

/**
 * build_history_list - Add a new node to the end of the history list
 *
 * @info: pointer to info_t struct containing history list
 * @buf: string containing new line of history
 * @linecount: integer count of the current line
 *
 * Return: 0 on success
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
    list_t *new_node = NULL;

    /* If history list already exists, set node to end of existing list */
    if (info->history)
        new_node = info->history;

    /* Add new node to end of list */
    add_node_end(&new_node, buf, linecount);

    /* If history list was empty, set it to the newly created node */
    if (!info->history)
        info->history = new_node;

    return (0);
}

/**
 * remember_history - Remember the history list starting from 0
 *
 * @info: pointer to info_t struct containing history list
 *
 * Return: new count of history items
 */
int remember_history(info_t *info)
{
    list_t *current_node = info->history;
    int new_count = 0;

    /* Traverse the list and renumber each node */
    while (current_node)
    {
        current_node->num = new_count++; /* Set node number and increment count */
        current_node = current_node->next; /* Move to next node */
    }

    /* Update the count of history items in the info_t struct */
    info->histcount = new_count;

    return (new_count);
}
