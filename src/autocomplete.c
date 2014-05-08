/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * autocomplete.c
 *
 * Contents: Contains the functions to autocomplete user commands
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

#include "autocomplete.h"

/**
 * Autocomplete the user commands
 *
 *@author Renê de Souza Pinto (added the linked circular list)
 *@param str The actual command
 *@return The full string, if founded, NULL if no matches found
 */
char *autocomplete(char *str) {

	struct dirent **namelist;
	char *cpath, *lookfor, *file, *fstr, f;
	int n, p, ls, rs;

	if(strlen(str) < 1)
		return(NULL);

	/* Search for most close path */
	p = 0;
	while(p < strlen(str) && str[p] != '/')
		p++;
	ls = p;

	p = strlen(str) - 1;
	while(p > 0 && str[p] != '/')
		p--;
	rs = p;

	if(ls < rs) {
		cpath = str_ncpy(cpath, &str[ls], (rs-ls));
		n     = scandir(cpath, &namelist, 0, NULL);
		free(cpath);

		lookfor = str_ncpy(lookfor, &str[rs+1], (strlen(str) - rs));
	} else if(ls == rs) {
		n       = scandir("/", &namelist, 0, NULL);
		lookfor = str_ncpy(lookfor, &str[ls+1], (strlen(str) - ls));
	} else {
		n       = scandir(".", &namelist, 0, NULL);
		lookfor = str_ncpy(lookfor, str, strlen(str));
	}

	/* Check for matches */
	f = 0;
	if(n > 0) {
		while(n--) {
			char *file = namelist[n]->d_name;

			if(strncmp(file, lookfor, strlen(lookfor)) == 0 && !f) {
				f = 1;

				fstr = str_ncpy(fstr, str, strlen(str) + (strlen(file) - strlen(lookfor)) );
				strcpy(&fstr[rs+1], file);
			}

			free(namelist[n]);
		}
		free(namelist);
	}
	free(lookfor);

	if(f) {
		return(fstr);
	} else {
		return(NULL);
	}
}


char *str_ncpy(char *dest, char *src, int n) {

	int i;

	dest = (char*)malloc(sizeof(char) * (n+1));
	if(dest == NULL)
		return(NULL);

	for(i=0; i<n; i++)
		dest[i] = src[i];
	dest[n] = '\0';

	return(dest);
}

