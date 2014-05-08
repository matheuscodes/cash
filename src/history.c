/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * history.c
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

#include "history.h"

/**
 *Read the history file and loads it on the hash table
 *
 *@author Thiago Camargo Fernandes
 *@author Renê de Souza Pinto (added the linked circular list)
 *@param filename The history file name
 *@param hashtable Linked list
 *@param clist Linked circular list
 *@return int 1 on success
 */
int load_history_file(char *filename, linked_list ***hashtable, linked_clist **clist) {

	char *buffer, *str;
	FILE *fp;
	int filelength;
	int i, j;
	linked_list **htable;
	linked_clist *_clist;

	htable = *hashtable;
	_clist = *clist;

	/* Open the cash history file for READ*/
	if ((fp=fopen(filename, "r")) == NULL)
		return 1;

	/* Save the file length */
	fseek(fp, 0, SEEK_END);
	filelength = ftell(fp);

	/* Allocate a buffer of the file's size */
	buffer = (char*)malloc(sizeof(char) * (filelength + 1));
	if(buffer == NULL) {
			fclose(fp);
			return 1;
	}

	/* Returns the pointer to the start of the file */
	fseek(fp, 0, SEEK_SET);

	/* Add each line of the file, as a string, in the hash table*/
	i = 0;
	j = 0;
	while ( !feof(fp) ){

		buffer[i] = (char)fgetc(fp);
		if (buffer[i] == EOF) {
			break;
		} else {
			if (buffer[i] == '\n') {
				buffer[i] = '\0';
				add_string(&htable, &buffer[j]);

				str = malloc(sizeof(char) * strlen(&buffer[j]));
				if(str != NULL) {
					strcpy(str, &buffer[j]);
					clist_add(&_clist, str);
				}
				j = i + 1;
			}
		}
		i++;
	}

	*hashtable = htable;
	*clist     = _clist;
	free(buffer);
	fclose(fp);
	return 1;
}


/**
 * Record the history to a file
 *
 * @author Renê de Souza Pinto
 * @param filename File name to record the history
 * @param clist Linked circular list with history
 * @return int 1 on success, 0 on failure
 */
int write_history_file(char *filename, linked_clist *clist) {

	FILE *fp;
	linked_clist *_clist;
	unsigned long int i, list_size;

	_clist = clist;

	/* Create/Open file */
	if( (fp = fopen(filename, "w+")) == NULL)
			return(0);

	/* Write the history */
	if(_clist == NULL)
			return(0);
	list_size = clist_get_size(_clist);
	for(i=0; i<list_size; i++) {
			fprintf(fp, "%s\n", _clist->element);
			_clist = _clist->next;
	}


	/* Close file and return */
	fclose(fp);
	return(1);
}


/**
 *Return the string's position on the hash table
 *
 *@author Thiago Camargo Fernandes
 *@param string String to be found the position of
 *@return int position 
 */
int string_position(char *string){

	int temp = 0;
	int i = 0;
	int length = strlen(string);
	int position = 0;

	/* Hashing function */
	/* Concatenate the ASCII code of the characters by pairs and sum the results */
	for (i = 0; i < length; i += 2) {
		if ((int)string[i+1] < 100) {
			temp += (int)string[i]*100 + (int)string[i+1];
		} else {
			temp += (int)string[i]*1000 + (int)string[i+1];
		}
	}

	position = temp % TABLELENGTH;

	return position;
}


/**
 * Add a string in a hash table
 * 
 *@author Thiago Camargo Fernandes
 *@author Renê de Souza Pinto (added some security code)
 *@param hashtable Linked list
 *@param string String to be added
 *@return int 1 on success, 0 on failure
 */
int add_string(linked_list ***hashtable, char *string){

	char *str;

	/* Calculate the insert position */
	int insertposition = string_position(string);
	linked_list **_list;

	_list  = *hashtable;
	if(_list == NULL)
			return(0);

	/* Create a linked list if it don't exists */
	if ( _list[insertposition] == NULL ){
		if ( !llist_create(&(_list[insertposition])) )
				return 0;
	}

	/* Add the string in the list */
	str = (char*)malloc(sizeof(char) * strlen(string));
	if(str != NULL) {
			strcpy(str, string);
			llist_add(&_list[insertposition], str);
	} else {
			return(0);
	}

	*hashtable = _list;
	return 1;
}


/**
 * Remove a string from hash table
 * 
 * @author Renê de Souza Pinto
 * @param hashtable The hashtable
 * @param string String to be removed
 * @return int 1 on success, 0 otherwise
 */
int remove_string(linked_list ***hashtable, char *string) {

	int pos;
	linked_list **_list;

	_list = *hashtable;
	pos   = string_position(string);
	if(_list != NULL && string != NULL && pos > 0) {
			if(_list[pos] != NULL) {
					/* Search and remove the string */
					unsigned long int pos_str;

					pos_str = llist_search(_list[pos], string, strlen(string));
					if(pos_str != -1) {
							return( llist_remove(&_list[pos], pos_str) );
					} else {
							return(0);
					}
			}
	} else {
			return(0);
	}

	*hashtable = _list;
	return(1);
}


/**
 * Create and inicialize a hash table
 *
 *@author Thiago Camargo Fernandes
 *@param hashtable Linked list
 *@return int 1 on success
 */
int create_hash_table(linked_list ***hashtable){

	int i = 0;
	linked_list **list;

	/* Allocate memory for the hash table */
	list = (linked_list**)malloc(sizeof(linked_list*) * TABLELENGTH);
	if(list == NULL)
			return(1);

	/* Inicialize the hash table */
	for (i = 0; i < TABLELENGTH; i++) {
		list[i] = NULL;
	}

	/* Return */
	*hashtable = list;

	return 1;
}


/**
 * Free the memory used by the hash table
 * 
 * @author Thiago Camargo Fernandes
 * @author Renê de Souza Pinto (added free memory for elements of list)
 * @param hashtable Linked list
 * @return int 1 on success
 */
int destroy_hash_table(linked_list*** hashtable){

	int i = 0;
	linked_list **list;

	list = *hashtable;

	/* Destroy all linked lists on the hash table */
	for (i = 0; i < TABLELENGTH; i++ )
			if(list[i] != NULL) {
					free(list[i]->element);
					llist_destroy(&(list[i]));
			}

	/* Disallocate the hash table */
	free(list);

	return 1;
}

