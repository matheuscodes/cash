/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br) 
 *
 * linkedl.c
 *
 * This file is part of CASH, CASH is The C Again SHell.
 *
 * CASH is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CASH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "linkedl.h"


/**
 * Create a linked list <p>
 * Must be executed before any operation on list
 *
 * @author Renê de Souza Pinto
 * @param list Linked list
 * @return int 1 On success, 0 on failure
 */
int llist_create(linked_list **list) {

	linked_list *_list;

	/* Start the list with null */
	_list = NULL;
	*list = _list;
	return(1);
}


/**
 * Add element on list
 *
 * @author Renê de Souza Pinto
 * @param list Linked list
 * @param element Element to be added
 * @return int 1 If the element was added successfully, 0 on failure
 */
int llist_add(linked_list **list, void *element) {

	linked_list *_list;
	linked_list *new_node;
	linked_list *temp;

	_list = *list;

	/* Allocate memory */
	new_node = (linked_list*)malloc(sizeof(linked_list));
	if(new_node == NULL) {
		return(0);
	} else {
		/* Add on list */
		new_node->element = element;
		new_node->next    = NULL;

		temp = _list;
		if(temp != NULL) {
				while(temp->next != NULL)
						temp = temp->next;
				temp->next = new_node;
		} else {
			_list = new_node;
		}
	}

	/* Return */
	*list = _list;
	return(1);
}


/**
 * Remove element from list
 *
 * @author Renê de Souza Pinto
 * @param list Linked list
 * @param pos Element's position to be removed
 * @return int 1 If the element was removed successfully, 0 on failure
 */
int llist_remove(linked_list **list, unsigned long int pos) {

	linked_list *prev;
	linked_list *next;
	linked_list *_list;
	linked_list *temp;
	unsigned long int size;

	_list = *list;
	size = llist_get_size(_list);

	if(pos >= size) return(0);

	if(_list != NULL) {
		if(pos == 0) {
			/* Element at the top of list */
			next   = _list->next;
			free(_list);
			_list  = next;
		} else {
			if(pos > 0 && pos < (size-1)) {
				/* Element at the half of list */
				unsigned long int p;

				temp = _list;
				for(p=1;p<pos;p++)
					temp = temp->next;

				prev = temp;
				next = (temp->next)->next;

				free(temp->next);
				prev->next = next;

			} else {
				/* Element at the end of list */
				temp = _list;
				unsigned long int p;

				for(p=1;p<pos;p++)
						temp = temp->next;

				prev       = temp;
				prev->next = NULL;
				free(temp->next);
			}
		}
	}

	/* Retorna */
	*list = _list;
	return(1);
}


/**
 * Destroy list (remove all elements)
 *
 * @author Renê de Souza Pinto
 * @param list Linked list
 * @return 1 If the list was destroyed successfully, 0 on failure
 */
int llist_destroy(linked_list **list) {

	linked_list *_list;
	unsigned long int size;
	unsigned long int pos;

	_list = *list;
	size = llist_get_size(_list);
	for(pos=0; pos<size; pos++)
			llist_remove(&_list, 0);

	return(1);
}


/**
 * Return the size of list (number of elements)
 * 
 * @author Renê de Souza Pinto
 * @param list Linked list
 * @return unsigned long int Size of list
 */
unsigned long int llist_get_size(linked_list *list) {

	linked_list *_list;
	unsigned int size;

	_list = list;
	size = 0;

	while(_list != NULL) {
		size++;
		_list = _list->next;
	}

	return(size);
}


/**
 * Search element on list
 *
 * @author Renê de Souza Pinto
 * @param list Linked list
 * @param element Searched Element
 * @param size Size (in bytes) of element
 * @return unsigned long int Position of found element, 
 * 		<p> or -1, if any element was found
*/
unsigned long int llist_search(linked_list *list, void *element, size_t size) {

	linked_list *temp;
	unsigned long int pos = 0;

	temp = list;
	if(temp != NULL) {
		while(temp != NULL) {
			if(memcmp(temp->element,element,size) == 0) {
				return(pos);
			}
			temp = temp->next;
			pos++;
		}
	}

	return(-1);
}

