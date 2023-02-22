#include "shell.h"

/**
 * add_node - Add a new node to the beginning of a list
 *
 * @head: pointer to the head of the list
 * @str: string to be copied into the new node's str field
 * @num: integer to be stored in the new node's num field
 *
 * Return: pointer to the new node on success, NULL on failure
 */
list_t *add_node(list_t **head, const char *str, int num)
{
    list_t *new_node;

    /* Check for null pointer */
    if (!head)
        return (NULL);

    /* Allocate memory for new node */
    new_node = malloc(sizeof(list_t));
    if (!new_node)
        return (NULL);

    /* Initialize new node's fields */
    _memset((void *)new_node, 0, sizeof(list_t));
    new_node->num = num;

    /* If string is provided, copy it into new node's str field */
    if (str)
    {
        new_node->str = _strdup(str);
        if (!new_node->str)
        {
            free(new_node);
            return (NULL);
        }
    }

    /* Add new node to beginning of list */
    new_node->next = *head;
    *head = new_node;

    return (new_node);
}

/**
 * add_node_end - Adds a new node at the end of a linked list.
 * @head: Pointer to a pointer to the head of the linked list.
 * @str: Pointer to the string to be stored in the new node.
 * @num: Integer to be stored in the new node.
 *
 * Return: Pointer to the new node, or NULL if an error occurred.
 */
list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *current_node;

	if (!head) /* Check for NULL head */
		return (NULL);

	current_node = *head; /* Set current_node to point to the head */
	new_node = malloc(sizeof(list_t)); /* Allocate memory for new_node */
	if (!new_node) /* Check for allocation failure */
		return (NULL);

	/* Initialize new_node to zero */
	_memset((void *)new_node, 0, sizeof(list_t));

	/* Set num field of new_node */
	new_node->num = num;

	if (str) /* Check if str is not NULL */
	{
		/* Duplicate str and store it in the new_node */
		new_node->str = _strdup(str);
		if (!new_node->str) /* Check for allocation failure */
		{
			free(new_node); /* Free new_node memory */
			return (NULL);
		}
	}

	if (current_node) /* Check if list is not empty */
	{
		while (current_node->next) /* Traverse to the last node */
			current_node = current_node->next;

		/* Add the new_node to the end of the list */
		current_node->next = new_node;
	}
	else /* If list is empty */
	{
		*head = new_node; /* Set the new_node as the head */
	}

	return (new_node); /* Return a pointer to the new node */
}

/**
 * print_list_str - Prints all the strings in a linked list.
 * @h: Pointer to the head of the linked list.
 *
 * Return: The number of nodes in the list.
 */
size_t print_list_str(const list_t *h)
{
	size_t node_count = 0;

	while (h) /* Traverse the linked list */
	{
		/* Print the string or "(nil)" if NULL */
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");

		h = h->next; /* Move to the next node */
		node_count++; /* Increment node count */
	}

	return (node_count); /* Return the number of nodes */
}

/**
 * delete_node_at_index - Deletes a node at a given index in a linked list.
 * @head: Pointer to a pointer to the head of the linked list.
 * @index: Index of the node to delete.
 *
 * Return: 1 if successful, 0 if the node was not found or if head is NULL.
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head) /* Check for NULL head or empty list */
		return (0);

	if (!index) /* If index is 0, delete the head node */
	{
		node = *head;
		*head = (*head)->next;
		free(node->str); /* Free the string in the node */
		free(node); /* Free the node */
		return (1);
	}

	node = *head;
	while (node) /* Traverse the linked list */
	{
		if (i == index) /* If the index matches */
		{
			prev_node->next = node->next; /* Remove the node */
			free(node->str); /* Free the string in the node */
			free(node); /* Free the node */
			return (1);
		}

		i++; /* Increment the index */
		prev_node = node; /* Set prev_node to the current node */
		node = node->next; /* Move to the next node */
	}

	return (0); /* Return 0 if the node was not found */
}

/**
 * free_list - Frees a linked list and sets the head pointer to NULL.
 * @head_ptr: Pointer to a pointer to the head of the linked list.
 */
void free_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr) /* Check for NULL head or empty list */
		return;

	head = *head_ptr; /* Save a copy of the head pointer */
	node = head;
	while (node) /* Traverse the linked list */
	{
		next_node = node->next; /* Save a copy of the next node pointer */
		free(node->str); /* Free the string in the node */
		free(node); /* Free the node */
		node = next_node; /* Move to the next node */
	}

	*head_ptr = NULL; /* Set the head pointer to NULL */
}
