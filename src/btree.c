/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * btree.c
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

#include "btree.h"


/**
 * Print the tree (for debug porpouses)
 *
 * @param bt The binary tree
 * @param l For recursivity (it starts with 0)
 */
void btree_print(btree *bt, int l) {

#ifdef DEBUG

    int i;
	if (bt == NULL)
		return;

	btree_print(bt->right, l+1);
	for(i = 0; i < l; i++) {
	     printf("      ");
	}
	printf("%s\n", bt->element);
	btree_print(bt->left, l+1);

#endif
}


/**
 * Create a empty binary tree
 * 
 * @author Renê de Souza Pinto
 * @param btree** The tree
 */
void btree_create(btree **tree) {
	*tree = NULL;
}


/**
 * Inserts the command (or pipe) into the binary tree
 * 
 * @author Renê de Souza Pinto
 * @param tree Binary tree
 * @param element The command to be inserted
 * @return 1 On success, 0 otherwise
 */
int btree_insert(btree **tree, char *element) {

	btree *temp;
	btree *_tree = *tree;
	char *new_element;

	if(_tree == NULL) {
		/* Insert the element */
		_tree = (btree*)malloc( sizeof(btree) );
		if(_tree == NULL)
			return(0);

		new_element = (char*)malloc(sizeof(char) * strlen(element));
		if(new_element == NULL) {
			free(_tree);
			return(0);
		} else {
			strcpy(new_element, element);
		}

		_tree->element = new_element;
		_tree->left    = NULL;
		_tree->right   = NULL;

		*tree = _tree;
		return(1);
	} else if( is_pipe(element) ){
		temp = _tree;

		_tree = (btree*)malloc( sizeof(btree) );
		if(_tree == NULL)
			return(0);

		new_element = (char*)malloc(sizeof(char) * strlen(element));
		if(new_element == NULL) {
			free(_tree);
			return(0);
		} else {
			strcpy(new_element, element);
		}

		_tree->element = new_element;
		_tree->left    = temp;
		_tree->right   = NULL;

		*tree = _tree;
		return(1);
	} else {
		btree *n0 = _tree;
		btree *n1 = _tree->left;
		btree *n2 = _tree->right;

		if(n0 != NULL)
			if( is_pipe(n0->element) )
				btree_insert(&(_tree->right), element);

		if(n1 != NULL)
			if( is_pipe(n1->element) )
				btree_insert(&(_tree->left), element);

		if(n2 != NULL)
			if( is_pipe(n2->element) )
				btree_insert(&(_tree->right), element);
	}
}


/**
 * Destroy a binary tree
 *
 * @author Renê de Souza Pinto
 * @param tree The binary tree
 * @return int 1 On success, 0 otherwise
 */
int btree_destroy(btree **tree) {

	btree *_tree = *tree;

	if(_tree != NULL) {
		if(_tree->left == NULL && _tree->right == NULL) {
			free(_tree->element);
			free(_tree);
			_tree = NULL;
			return(1);
		} else {
			btree_destroy( &(_tree->left)  );
			btree_destroy( &(_tree->right) );
		}
	}

	return(1);
}


/**
 * Check if is a pipe
 * 
 * @author Renê de Souza Pinto
 * @param str String to be checked
 * @return int 1 If it's a pipe, 0 otherwise
 */
int is_pipe(char *str) {

	if(strcmp(str, ">")   == 0 ||
		strcmp(str, ">>") == 0 ||
		strcmp(str, "<")  == 0 ||
		strcmp(str, "<<") == 0 ||
		strcmp(str, "2>") == 0 ||
		strcmp(str, "|")  == 0 ||
		strcmp(str, "||") == 0 ||
		strcmp(str, "&")  == 0 ||
		strcmp(str, "&&") == 0 ||
		strcmp(str, ";")  == 0 ) {

			return(1);
	} else {
		return(0);
	}

}

