/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * interpreter.c
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

#include "interpreter.h"


/**
 * Get and add tokens in the token list
 *
 * @author Renê de Souza Pinto
 * @param string String
 * @param tokenlist List of tokens
 * @return int Length of the List of tokens, -1 on error
 *
 */
size_t gettokens(char *string, char ***tokenlist){

	size_t i, j, p, s_str, list_s;
	char *buffer;
	char **tlist;

	i	   = 0;
	j      = 0;
	p      = 1;
	list_s = TOKENLIST_LENGTH;
	s_str  = strlen(string);
	buffer = (char*)malloc(sizeof(char) * s_str);
	tlist  = (char**)malloc(sizeof(char*) * TOKENLIST_LENGTH);
	if(buffer == NULL || tlist == NULL)
			return(-1);
	else
			strcpy(buffer, string);

	do {
			char c1, c2, c3;
			char *s1, *s2;
			int x;

			c1 = buffer[p-1];
			c2 = buffer[p];
			c3 = buffer[p+1];

			if((j+1) >= list_s) {
				char **newl = realloc(tlist, sizeof(char*) * (list_s + TOKENLIST_LENGTH));
				if(newl == NULL)
					return(-1);
				list_s += TOKENLIST_LENGTH;
				tlist   = newl;
			}


			if(c2 == '>' || c2 == '<' || c2 == '|' || c2 == '&' || c2 == '`') {

				s2 = (char*)malloc(sizeof(char) * 3);
				if(s2 == NULL) {
						free(buffer);
						return(-1);
				}

				if(c1 == '2' && c2 == '>') {
						s2[0] = c1;
						s2[1] = c2;
						s2[2] = '\0';

						buffer[p-1] = '\0';
						p += 2;
				} else if( (c2 == '>' && c3 == '>') ||
								(c2 == '&' && c3 == '&') ||
								(c2 == '|' && c3 == '|') ||
								(c2 == '<' && c3 == '<')) {

						s2[0] = c2;
						s2[1] = c3;
						s2[2] = '\0';

						buffer[p-1] = '\0';
						p += 2;
				} else {
						s2[0] = c2;
						s2[1] = '\0';

						buffer[p] = '\0';
						p++;
				}
				s1 = clean_string(&buffer[i]);
				i  = p;

				tlist[j] = s1;
				j++;
				tlist[j] = s2;
				j++;

			} else if(p >= (s_str - 1)) {
					buffer[s_str] = '\0';
					s1 = clean_string(&buffer[i]);
					tlist[j] = s1;
					j++;
			}

			p++;
	} while(p < s_str);

	*tokenlist = tlist;
	free(buffer);
	return(j);
}


/**
 * Clean string (remove unecessary spaces)
 *
 * @author Renê de Souza Pinto
 * @param char* String
 * @return char* The new string, NULL on error
 */
char *clean_string(char *string) {

	size_t p, i, s_str;
	char *buff;

	i     = 0;
	p     = 0;
	s_str = strlen(string);
	buff  = (char*)malloc(sizeof(char) * s_str);
	if(buff == NULL)
			return(NULL);
	else
			buff = memset(buff, '\0', s_str);

	/* Remove the initial spaces */
	while(string[p] == ' ')
			p++;

	/* Remove the rest */
	while(p < s_str) {

			if(string[p] == '\\' && string[p+1] == ' ') {
					strcat(buff, "\\ ");
					p += 2;
					i += 2;
			} else  if(string[p] == '\"') {
							do {
									buff[i] = string[p];
									i++;
									p++;
							} while(p < s_str && string[p] != '\"');
			} else {
					if(string[p] == ' ' && string[p+1] == ' ') {
							while(p < s_str && string[p+1] == ' ')
									p++;
				 	} else {
							buff[i] = string[p];
							i++;
							p++;
				    }
			}
	}
	if(buff[p-1] == ' ')
			buff[p-1] = '\0';
	else
			buff[p] = '\0';

	return(buff);
}


/**
 * Free memory allocated to the list of tokens
 *
 *@author Thiago Camargo Fernandes
 *@param tokenlist Pointer to a list of tokens
 *@param length Length of the list of tokens
 *@return int 1 on success
 *
 */
int destroy_tokenlist(char ***tokenlist, int length){
	char **temp;
	int i = 0;

	temp = *tokenlist;

	/* Free the allocated memory of the tokens */
	for(i = 0; i < length; i++)
		if(temp[i] != NULL)
				free(temp[i]);

	/* Free the allocated memory of token list */
	free(temp);

	return 1;
}

