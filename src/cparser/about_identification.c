/**
 * CASH - The C Again SHell
 *
 * Copyright (C) 2007 Matheus Borges Teixeira <matheus.bt at gmail.com>
 * Copyright (C) 2007 Renê de Souza Pinto <rene at renesp.com.br>
 * Copyright (C) 2007 Thiago Camargo Fernandes <thiagocf at grad.icmc.usp.br>
 *
 * about_identification.c
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

int given_constants_count = 0; /* Counter for constant names */

/**
 * Checks whether a character can be used in an identifier.
 *
 * @author Matheus Borges Teixeira
 * @param which - character to be checked
 * @return 1 for yes, 0 for no
 */
char allowed_identifier_char(char which){
	/* TODO: Restrict even further. */
	switch(which){
		case '\0':
		case ' ':
		case '\t':
		case '\n':
		case '"':
		case '\'':
		case '*':
		case '$':
		case '{':
		case '}':
		case '(':
		case ')':
		case ';':
			return 0;
		default:
			return 1;
	}
}

/**
 * Detects an identifier without moving the file pointer.
 *
 * @author Matheus Borges Teixeira
 * @param from - opened file being read
 */
int get_identifier(FILE * from){
	fpos_t position;
	char * word = NULL;
	char present = 0;
	int counter = 0;
	int saved_line = 0;
	int result = 0;

	clean_blank(from);

	if(is_operator(from) != NOOPERATOR){
		return OPERATORCODE;
	}


	if(feof(from)){
		return EMPTY;
	}
	
	fgetpos(from,&position);
	saved_line = line;
	present = fgetc(from);

	if((present >= '0') && (present <= '9')){
		fsetpos(from, &position);
		return CONSTANTCODE;
	}
	
	if(present == '"'){
		fsetpos(from, &position);
		return STRINGCODE;
	}

	if(present == '\''){
		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while parsin an identifier.");
		}
		present = fgetc(from);
		if(present == '\\'){
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while parsing an identifier.");
			}
			/* Skipping one char (content of the char is irrelevant) */
			present = fgetc(from);
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while parsing an identifier.");
			}
			/* Second char must close constant */
			present = fgetc(from);
			if(present == '\''){
				fsetpos(from,&position);
				return CONSTANTCODE;
			}
			else{
				fsetpos(from,&position);
				return COMMANDCODE;
			}
		}
		else{
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while parsing an identifier.");
			}
			present = fgetc(from);
			if(present == '\''){
				fsetpos(from,&position);
				return CONSTANTCODE;
			}
			else{
				fsetpos(from,&position);
				return COMMANDCODE;
			}
		}
	}

	while(allowed_identifier_char(present)){
		
		if(is_operator(from) != NOOPERATOR){
			/* Identifier ended and an operator appeared */
			counter++;
			break;
		}
		if(feof(from)){
			interpreterError(UNEXPECTEDFILEEND,"File ended abruptly while parsing an identifier.");
		}
		present = fgetc(from);
		counter++;
	}
	if(present == '"'){
		interpreterError(IRREGULARCHARACTER,"Unexpected \" found while parsing an identifier.");
	}
	if(present == '\n'){
		interpreterError(UNEXPECTEDLINEEND,"Line finished while parsing an identifier.");
	}

	fsetpos(from,&position);
	word = (char*)malloc(sizeof(char)*counter+1);
	fread(word,sizeof(char),counter,from);
	fsetpos(from,&position);
	line = saved_line;
	word[counter] = '\0';

	if(strlen(word) > 0){
		#ifdef __DEBUG__MODE__
		printf("Got identifier '%s' of size %d in line %d\n",word,counter,line);
		#endif
		result = identify(word);
		free(word);
		return result;
	}
	else{
		#ifdef __DEBUG__MODE__
		printf("Found an empty identifier in line %d\n",line);
		#endif
		free(word);
		return EMPTY;
	}
}

/**
 * Transforms an identifier into a code.
 * Used to recognize special identifiers.
 *
 * @author Matheus Borges Teixeira
 * @param word - string holding the identifier
 * @return a code for what the identifier is
 */
int identify(char * word){
	if(!strcmp(word,"if")){
		return IFCODE;
	} 
	else{
		if(!strcmp(word,"do")){
			return DOCODE;
		}
		else{
			if(!strcmp(word,"while")){
				return WHILECODE;
			}
			else{
				if(!strcmp(word,"for")){
					return FORCODE;
				}
				else{
					if(!strcmp(word,"else")){
						return ELSECODE;
					}
					else{
						return VARIABLECODE;
					}
				}
			}
		}
	}
}

/**
 * Consumes an identifier.
 * Used to move the file pointer and retrieve variables and constants.
 *
 * @author Matheus Borges Teixeira
 * @param code - identifier to be expected
 * @param from - opened file being read
 * @return variable with the consumed content or NULL
 */
data * consume_identifier(int code,FILE * from){
	char present = 0;
	char * name = NULL;
	fpos_t position;
	int counter = 0;
	data * result = NULL;
	char is_float = 0;
			
	switch(code){
		case IFCODE:
			name = (char*)malloc(sizeof(char)*strlen("if")+1);
			fread(name,sizeof(char),strlen("if"),from);
			free(name);
			return NULL;

		case ELSECODE:
			name = (char*)malloc(sizeof(char)*strlen("else")+1);
			fread(name,sizeof(char),strlen("else"),from);
			free(name);
			return NULL;

		case FORCODE:
			name = (char*)malloc(sizeof(char)*strlen("for")+1);
			fread(name,sizeof(char),strlen("for"),from);
			free(name);
			return NULL;

		case DOCODE:
			name = (char*)malloc(sizeof(char)*strlen("do")+1);
			fread(name,sizeof(char),strlen("do"),from);
			free(name);
			return NULL;

		case WHILECODE:
			name = (char*)malloc(sizeof(char)*strlen("while")+1);
			fread(name,sizeof(char),strlen("while"),from);
			free(name);
			return NULL;

		case COMMANDCODE:
			fgetpos(from,&position);
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a command.");

			}
			present = fgetc(from);
			if(present == '\''){
				/*TODO: Test inside expression commands */
				fgetpos(from,&position);
				while(present != '\''){
					if(present == '\n'){
						interpreterError(UNEXPECTEDLINEEND,"Line ended while expecting a '''.");
					}
					counter++;
					if(feof(from)){
						interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading an expression command.");
					}
					present = fgetc(from);
				}
			}
			else{
				while(present != ';'){
					if(present == '\n'){
						interpreterError(UNEXPECTEDLINEEND,"Line ended while expecting a ';'.");
						break;
					}
					counter++;
					if(feof(from)){
						interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a command.");
					}
					present = fgetc(from);
				}
			}
			fsetpos(from, &position);
			name = (char*)malloc(sizeof(char)*counter+1);
			fread(name,sizeof(char),counter,from);
			name[counter] = '\0';
			result = run_command(name);
			free(name);
			return result;

		case VARIABLECODE:
			fgetpos(from,&position);
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a variable.");
			}
			present = fgetc(from);
			while(allowed_identifier_char(present)){
				counter++;
				if(is_operator(from) != NOOPERATOR){
					break;
				}
				if(feof(from)){
					interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a variable.");
				}
				present = fgetc(from);
			}
			fsetpos(from, &position);
			if(counter == 0){
				interpreterError(SYNTAXERROR,"Expected variable but none found.");
			}
			name = (char*)malloc(sizeof(char)*counter+1);
			fread(name,sizeof(char),counter,from);
			name[counter] = '\0';
			result = get_variable(name);
			free(name);
			return result;


		case CONSTANTCODE:
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a constant.");
			}
			fgetpos(from,&position);
			present = fgetc(from);
			counter = 1;
			if((present >= '0') && (present <= '9')){
				/* This is a number */
				if(feof(from)){
					interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a number.");
				}
				present = fgetc(from);
				while(((present >= '0') && (present <= '9')) || present == '.' || present == 'E' || present == 'e'){
					if(feof(from)){
						interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a number.");
					}
					if(present == '.' || present == 'E' || present == 'e'){
						present = fgetc(from);
						counter++;
						is_float = 1;
						if(!(((present >= '0') && (present <= '9')) || present != '-' || present != '+')){
							interpreterError(SYNTAXERROR,"Malformed float.");
						}
						if(feof(from)){
							interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a float.");
						}
					}
					present = fgetc(from);
					counter++;
				}
				fsetpos(from,&position);
				name = (char*)malloc(counter*sizeof(char) + 1);
				fread(name,sizeof(char),counter,from);
				name[counter] = '\0';
				if(is_float == 1){
					result = (data*)malloc(sizeof(data));
					result -> type = TYPEFLOAT;
					result -> isExtern = 0;
					result -> content = (DECLAREFLOAT*)malloc(sizeof(DECLAREFLOAT));
					result -> name = (char*)malloc(sizeof(char)*NAMEOFTHINGS+1);
					sprintf(result -> name,"_constant%d",given_constants_count++);
					/* TODO: handle conversion errors nicely */
					*((DECLAREFLOAT*)(result -> content)) =  strtof(name,NULL);
					free(name);
					
					#ifdef __DEBUG__MODE__
					printf("Created a float %s in line %d\n",result -> name,line);
					#endif
					return result;
				}
				else{
					result = (data*)malloc(sizeof(data));
					result -> type = TYPEINT;
					result -> isExtern = 0;
					result -> content = (DECLAREINT*)malloc(sizeof(DECLAREINT));
					result -> name = (char*)malloc(sizeof(char)*NAMEOFTHINGS+1);
					sprintf(result -> name,"_constant%d",given_constants_count++);
					
					*((DECLAREINT*)(result -> content)) =  atoi(name);
					free(name);
					
					#ifdef __DEBUG__MODE__
					printf("Created an integer %s in line %d\n",result -> name,line);
					#endif
					return result;
				}
			}
			else if(present == '\''){
				/* TODO: read simple chars */
			}
			interpreterError(UNKNOWN,"Illegal state: while reading a constant, it could not be parsed.");
			break;

		case STRINGCODE:
			if(feof(from)){
				interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while expecting a string.");
			}
			present = fgetc(from);
			counter = 0;
			if(present == '"'){
				fgetpos(from,&position);
				do{
					if(feof(from)){
						interpreterError(UNEXPECTEDFILEEND,"Reached the end of the file while reading a string.");
					}
					present = fgetc(from);
					if(present == '\n'){
						interpreterError(UNEXPECTEDLINEEND,"Reached the end of the line while reading a string.");
					}
					counter++;
				} while(present != '"');
				fsetpos(from,&position);
				result = (data*)malloc(sizeof(data));
				result -> type = TYPESTRING;
				result -> isExtern = 0;
				result -> content = (DECLARESTRING)malloc((counter-1)*sizeof(char)+1);
				fread(result -> content,sizeof(char),counter-1,from);
				fgetc(from); /*Cleaning char*/
				((DECLARESTRING)(result -> content))[counter-1] = '\0';
				result -> name = (char*)malloc(sizeof(char)*NAMEOFTHINGS+1);
				sprintf(result -> name,"_string%d",given_constants_count++);
				#ifdef __DEBUG__MODE__
				printf("Created a string %s in line %d\n",result -> name,line);
				#endif
				return result;
			}
			interpreterError(UNKNOWN,"Illegal state: while reading a string, no string was found.");
			break;
		case OPERATORCODE:
			interpreterError(CONSUMINGIDENTIFIER,"Found an operator while an identifier was expected.");
			break;
		case EMPTY:
			return NULL;
		default:
			interpreterError(CONSUMINGIDENTIFIER,"Unknown identifier code.");
	}
}
