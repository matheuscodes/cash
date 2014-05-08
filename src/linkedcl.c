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

#include "linkedcl.h"


/**
 * Create a circular linked list <p>
 * Must be executed before any operation on list
 *
 * @author Renê de Souza Pinto
 * @param list Circular linked list
 * @return int 1 On success, 0 on failure
 */
int clist_create(linked_clist **list) {

	linked_clist *_list;

	/* Start the list with null */
	_list = NULL;
	*list = _list;
	return(1);
}


/**
 * Add element on list
 *
 * @author Renê de Souza Pinto
 * @param list Circular linked list
 * @param element Element to be added
 * @return int 1 If the element was added successfully, 0 on failure
 */
int clist_add(linked_clist **list, void *element) {

	linked_clist *_list;
	linked_clist *new_node;
	linked_clist *temp;

	_list = *list;

	/* Allocate memory */
	new_node = (linked_clist*)malloc(sizeof(linked_clist));
	if(new_node == NULL) {
		return(0);
	} else {
		/* Add on list */
		new_node->element = element;

		if(_list == NULL) {
				/* Create a head's list */
				new_node->next = new_node;
				new_node->prev = new_node;
				_list		   = new_node;
		} else {
				/* Add the new element */
				new_node->prev    = _list->prev;
				new_node->next 	  = _list;
				_list->prev->next = new_node;
				_list->prev       = new_node;
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
 * @param list Circular linked list
 * @param pos Element's position to be removed
 * @return int 1 If the element was removed successfully, 0 on failure
 */
int clist_remove(linked_clist **list, unsigned long int pos) {

	linked_clist *_list;
	linked_clist *temp;
	unsigned long int size, i;

	_list = *list;
	temp  = _list;
	size  = clist_get_size(temp);

	if(pos >= size) return(0);

	if(_list != NULL) {
			/* Go to element */
			for(i=0;i<pos;i++)
					temp = temp->next;

			/* Ajust the pointers */
			if(size == 1) {
					free(temp);
					_list = NULL;
			} else {
					if(pos == 0) {
							linked_clist *new_head;

							new_head             = _list->next;
							new_head->prev		 = temp->prev;
							new_head->prev->next = new_head;

							_list = new_head;
					} else {
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
					}

					/* Free memory */
					free(temp);
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
 * @param list Circular linked list
 * @return 1 If the list was destroyed successfully, 0 on failure
 */
int clist_destroy(linked_clist **list) {

	linked_clist *_list;
	unsigned long int size;
	unsigned long int pos;

	_list = *list;
	size  = clist_get_size(_list);
	for(pos=0; pos<size; pos++)
			clist_remove(&_list, 0);

	return(1);
}


/**
 * Return the size of list (number of elements)
 * 
 * @author Renê de Souza Pinto
 * @param list Circular linked list
 * @return unsigned long int Size of list
 */
unsigned long int clist_get_size(linked_clist *list) {

	linked_clist *_list;
	linked_clist *head;
	unsigned int size;

	_list = list;
	head  = _list;
	size  = 0;

	if(_list != NULL) {
		do {
				head = head->next;
				size++;
		} while(memcmp(_list, head, sizeof(linked_clist)) != 0);
	}

	return(size);
}


/**
 * Search element on list
 *
 * @author Renê de Souza Pinto
 * @param list Circular linked list
 * @param element Searched Element
 * @param size Size (in bytes) of element
 * @return unsigned long int Position of found element, 
 * 		<p> or -1, if any element was found
*/
unsigned long int clist_search(linked_clist *list, void *element, size_t size) {

	linked_clist *temp;
	linked_clist *head;
	unsigned long int pos = 0;

	temp = list;
	head = temp;

	if(temp != NULL) {
			do {
					if(memcmp(temp->element, element, size) == 0) {
							return(pos);
					} else {
							temp = temp->next;
							pos++;
					}
			} while(memcmp(temp, head, sizeof(linked_clist)) != 0);
	}

	return(-1);
}

