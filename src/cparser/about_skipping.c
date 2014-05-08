/**
 * CASH - The C Again SHell
 *
 * Copyright (C) 2007 Matheus Borges Teixeira <matheus.bt at gmail.com>
 * Copyright (C) 2007 Renê de Souza Pinto <rene at renesp.com.br>
 * Copyright (C) 2007 Thiago Camargo Fernandes <thiagocf at grad.icmc.usp.br>
 *
 * about_skipping.c
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

#include "cparser.h"

extern int line; /* Line counter */

/** 
 * Author's side note: 
 * Track - means one simple line of code.
 * Block - means one a group of lines of code delimited by {}
 */

/**
 * Cleans white spaces moving the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void clean_blank(FILE * from){
	char present = 0;	
	do {
		if(feof(from)){
			break;
		}
		present = fgetc(from);
		if(present == '\n'){
			line++;
		}
	} while ((present == ' ') || (present == '\t') || (present == '\n') || (present == 13));

	if((present != ' ') && (present != '\t') && (present != '\n') && (present != 13) && (!feof(from))){
		fseek(from,-sizeof(char),SEEK_CUR);
		if(present ==  '#' || present == '/'){
			clean_comments(from);
		}
	}

	return;
}

/**
 * Cleans code comments moving the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void clean_comments(FILE * from){
	char present = 0;
	if(feof(from)){
		return;
	}
	present = fgetc(from);
	if(present == '#'){
		do {
			if(feof(from)){
				break;
			}
			present = fgetc(from);
			if(present == '\n'){
				line++;
			}
		} while ((present != '\n') && (present != 13));
		clean_blank(from);
		return;
	}
	else if(present == '/'){
		if(feof(from)){
			fseek(from,-sizeof(char),SEEK_CUR);
			return;
		}
		present = fgetc(from);
		if(present == '*'){
			while(!feof(from)){
				present = fgetc(from);
				if(present == '*'){
					if(feof(from)){
						interpreterError(UNEXPECTEDFILEEND,"File ended while cleaning commentaries, missing '*/'.");
					}
					present = fgetc(from);
					if(present == '/'){
						clean_blank(from);
						return;
					}
				}
				if(present == '\n'){
					line++;
				}
			}
		}
		else if(present == '/'){
			do {
				if(feof(from)){
					break;
				}
				present = fgetc(from);
				if(present == '\n'){
					line++;
				}
			} while ((present != '\n') && (present != 13));
			clean_blank(from);
			return;
		}
		fseek(from,-2*sizeof(char),SEEK_CUR);
		return;
	}
	else {
		fseek(from,-sizeof(char),SEEK_CUR);
		return;
	}
}

/**
 * Skips an expression moving the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void skip_expression(FILE * from){
	char present = 0;
	do{
		if(feof(from)){
			break;
		}
		present = fgetc(from);
		if(present == '('){
			skip_expression(from);
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while skipping one expression.");
			}
			present = fgetc(from);
			if(present != ')'){
				interpreterError(SYNTAXERROR,"The expression syntax is incorrect, missing ')'.");
			}
		}
		if(present == '\n'){
			line++;
		}
	} while((present != ';') && (present != ')'));

	if(!feof(from)){
		fseek(from,-sizeof(char),SEEK_CUR);
		return;
	}
}

/**
 * Skips one line of code moving the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void skip_track(FILE * from){
	char present = 0;
	clean_blank(from);
	while((!feof(from)) && (present != ';') && (present != '\n') && (present != 13)){
		present = fgetc(from);
	}
	if(present == '\n'){
		line++;
	}
	clean_blank(from);
}

/**
 * Skips on block of code moving the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
void skip_block(FILE * from){
	char present;
	clean_blank(from);
	if(feof(from)){
		interpreterError(UNEXPECTEDFILEEND,"File ended while expecting to skip one block of code.");
	}
	present = fgetc(from);
	if(present == '{'){
		/* Skipping a Block */
		present = fgetc(from);
		while(present != '}'){
			if(present == '\n'){
				line++;
			}
			if(present == '{'){
				fseek(from,-sizeof(char),SEEK_CUR);
				skip_block(from);
			}
			clean_blank(from);
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"File ended while skiping one block of code, there is a '}' missing.");
			}
			present = fgetc(from);
		}
	}
	else{
		/* The block is a Track */
		fseek(from,-sizeof(char),SEEK_CUR);
		skip_track(from);
	}
}
