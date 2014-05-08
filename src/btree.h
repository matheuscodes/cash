/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * btree.h
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

#ifndef __CASH_BTREE__

	#define __CASH_BTREE__

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#define DEBUG

	typedef struct _btree_node {
			char *element;				/* Element of node 	*/
			struct _btree_node *left;	/* Left child		*/
			struct _btree_node *right;	/* Right child		*/
	} btree;


	/* Protoypes */
	void btree_print(btree *bt, int l);
	void btree_create(btree **tree);
	int  btree_insert(btree **tree, char *element);
	int  btree_destroy(btree **tree);
	int  is_pipe(char *str);

#endif /* __CASH_BTREE__ */

