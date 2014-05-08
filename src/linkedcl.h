/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * linkedcl.h
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

#ifndef __LINKED_CIRCULAR_LIST__

	#define __LINKED_CIRCULAR_LIST__

	#include <stdlib.h>
	#include <string.h>
	#include <sys/types.h>

	typedef struct __linked_clist {
		void *element;   			 	/* Element of node list  	*/
		struct __linked_clist *prev;	/* Previous element on list */
		struct __linked_clist *next;  	/* Next element on list  	*/
	} linked_clist;


	/* Prototypes */
	int clist_create(linked_clist **list);
	int clist_add(linked_clist **list, void *element);
	int clist_remove(linked_clist **list, unsigned long int pos);
	int clist_destroy(linked_clist **list);
	unsigned long int clist_get_size(linked_clist *list);
	unsigned long int clist_search(linked_clist *list, void *element, size_t size);

#endif /* __LISTA_ENC__ */

