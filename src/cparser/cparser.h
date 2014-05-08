/**
 * CASH - The C Again SHell
 *
 * Copyright (C) 2007 Matheus Borges Teixeira <matheus.bt at gmail.com>
 * Copyright (C) 2007 Renê de Souza Pinto <rene at renesp.com.br>
 * Copyright (C) 2007 Thiago Camargo Fernandes <thiagocf at grad.icmc.usp.br>
 *
 * about_errors.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

/*****************************************************************************/
/*******************************Maximum Values********************************/
#define MAXLENGTH 256
#define NAMEOFTHINGS 20
/*******************************Maximum Values********************************/
/*****************************************************************************/

/*****************************************************************************/
/*******************************About Variables*******************************/
#define TYPESTRING 0
#define TYPEINT 1
#define TYPEFLOAT 2

#define DECLARESTRING char*
#define DECLAREINT long int
#define DECLAREFLOAT double

typedef struct _data {
	int type;
	char * name;
	char isExtern;
	void * content;
} data;

typedef struct _datalist {
	data * variable;
	struct _datalist * next;
} datalist;

datalist * beginning;

void insert_variable(data * variable);
data * create_zero();
char * replace_variables(char * original);
void flush_variables(void);
data * get_variable(char * name);
void purge_variable(data ** variable);
data * convert_variable(data * variable, int type);
char check_extern(char * name);
void refresh_extern(data * value);
void update_extern(data * modified);
/*******************************About Variables*******************************/
/*****************************************************************************/

/*****************************************************************************/
/*******************************About Errors**********************************/
#define NOTIMPLEMENTED -1
#define UNKNOWN 0
#define UNEXPECTEDFILEEND 1
#define CANTOPENFILE 2
#define UNEXPECTEDLINEEND 3
#define IRREGULARCHARACTER 4
#define CONSUMINGIDENTIFIER 5
#define SYNCHRONIZECHARACTER 6
#define SYNTAXERROR 7
#define INVALIDIDENTIFIER 8
#define ILLEGALTYPE 9
#define ILLEGALOPERATION 10

void interpreterError(int wich,char * explanation);
/*******************************About Errors**********************************/
/*****************************************************************************/

/*****************************************************************************/
/*******************************About Execution*******************************/
void get_block(FILE * from);
void get_track(FILE * from);
void get_if(FILE * from);
void get_for(FILE * from);
void get_while(FILE * from);
void get_do(FILE * from);
/*******************************About Execution*******************************/
/*****************************************************************************/

/*****************************************************************************/
/*******************************About Identification**************************/
#define VARIABLECODE 0
#define IFCODE 1
#define DOCODE 2 
#define WHILECODE 3
#define FORCODE 4
#define STRINGCODE 5
#define COMMANDCODE 6
#define ELSECODE 7
#define CONSTANTCODE 8
#define OPERATORCODE 9

char allowed_identifier_char(char which);
int get_identifier(FILE * from);
int identify(char * word);
data * consume_identifier(int code,FILE * from);
/*******************************About Identification**************************/
/*****************************************************************************/

/*****************************************************************************/
/*******************************About Operations******************************/
#define EMPTY -1
#define NOOPERATOR 0
#define ASSIGNMENT 1
#define EQUAL 2
#define LESSEQUAL 3
#define LESS 4
#define GREATEREQUAL 5
#define GREATER 6
#define PLUS 7
#define MINUS 8
#define TIMES 9
#define RESTDIVISION 10
#define DIVISION 11
#define NOTEQUAL 12
#define PLUSPLUS 13
#define MINUSMINUS 14
#define PLUSEQUAL 15
#define MINUSEQUAL 16
#define TIMESEQUAL 17
#define DIVISIONEQUAL 18
#define NOT 19
#define OR 20
#define AND 21
#define COMMA 22

/** No precendence at all **/
#define NOPRECEDENCE 20

int is_operator(FILE * from);
char is_executable(FILE * from);
data * run_command(char * command);
void consume_operator_string(char * what, FILE * from);
void consume_operator(int operatorcode, FILE * from);
void release_helper(data ** variable);
void normalize(data ** first,data ** second);
data * operate(int operatorcode, data * first, data * second);
data * pre_operate(FILE * from, int operation, data ** ongoing, int precedence, int expected);
data * calculate_expression(FILE * from,int precedence);
/*******************************About Operations******************************/
/*****************************************************************************/

/*****************************************************************************/
/*******************************About Skipping********************************/
void clean_blank(FILE * from);
void clean_comments(FILE * from);
void skip_track(FILE * from);
void skip_block(FILE * from);
/*******************************About Skipping********************************/
/*****************************************************************************/
/*#define __DEBUG__MODE__*/
/*#define __SKIP__SHELL__*/
void interpret(char * filename);
extern char * do_command(char *command, int from); /* From the Shell to actually execute commands */

#endif /*_INTERPRETER_H_*/

