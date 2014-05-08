/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * cash_readline.c
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

#include "cash_readline.h"


/* Extern global variables */
extern char *name_of_cash;			/* Cash executable file name					*/
extern char *VERSION;				/* Version of CASH								*/
extern char *RELEASE;				/* Release of CASH								*/
extern linked_clist *c_history;		/* Command history list							*/
extern linked_list **c_hashtable;	/* Hash table to command history		 	 	*/


/**
 * Read a line from terminal with special keys and functions (arrows, TAB, history, ...)
 *
 * @author Renê de Souza Pinto
 * @return char* The line typed by user
 */
char *readline(void) {

	LINE_BUFFER *buff;
	linked_clist *e_history;
	char keys[8];
	char sem1, tab_pressed, space;
	char *temp_str, *str;
	unsigned long int  i;

	/* Check the buffer */
	if(old_buff == NULL) {
		/* Alloc LINE_BUFFER and initial buffer string */
		buff = (LINE_BUFFER*)malloc(sizeof(LINE_BUFFER));
		if(buff == NULL) {
				return(NULL);
		} else {
				buff->max_size = DEFAULT_BSIZE - 1;
				buff->line 	   = (char*)malloc(sizeof(char) * DEFAULT_BSIZE);
				if(buff->line == NULL) {
					return(NULL);
				} else {
					memset(buff->line, '\0', buff->max_size);
				}
		}
		old_buff = buff;
	} else {
		/* Clear and use the allocated buffer */
		buff = old_buff;
		memset(buff->line, '\0', buff->size);
	}

	write_prompt();


	/* Start to read keys */
	keys[0]       = '\0';
	sem1		  = 0;
	tab_pressed   = 0;
	space		  = 0;
	buff->size    = 0;
	buff->cur_pos = 0;
	e_history     = c_history;
	comm_hn		  = clist_get_size(e_history);
	temp_str      = NULL;
	while(keys[0] != ENTER_KEY) {

		/**
		 * Read keys
		 * n = Number of bytes read 
		 */
		int n = read(fd_out, keys, 4);

		/* Check for function keys */
		if(n >= 3) {

			if(strncmp(keys, UP_ARROW, 3) == 0 ||
					strncmp(keys, DOWN_ARROW, 3) == 0 ||
					strncmp(keys, PAGE_UP, 3) == 0 ||
					strncmp(keys, PAGE_DOWN, 3) == 0) {		/* Up/Down page/arrow */
						/* History */
						if(e_history != NULL) {
								char *str;
								linked_clist *head;
								unsigned long int str_l;

								head = c_history;

								if(strncmp(keys, UP_ARROW, 3) == 0 || strncmp(keys, PAGE_UP, 3) == 0) {
										e_history = e_history->prev;
								} else {
										e_history = e_history->next;
								}
								str   = (char*)e_history->element;
								str_l = strlen(str);

								if(!sem1) {
										if(temp_str != NULL) {
												free(temp_str);
										}

										if(buff->line != NULL)
												temp_str = (char*)malloc(sizeof(char) * strlen(&buff->line[1]));

										if(temp_str != NULL) {
												if(strcmp(&buff->line[1], "") != 0 && strcmp(temp_str, "") != 0) {
														strcpy(temp_str, &buff->line[1]);
														sem1 = 1;
														if(strcmp(&buff->line[1], temp_str) != 0) {
																clist_add(&head, temp_str);
																add_string(&c_hashtable, temp_str);
														}
												} else {
														if(!space) {
																clist_add(&head, "");
																space = 1;
														}
												}
										}
								}

								/* Clear line */
								write(fd_out, TERM_CLEAR_LINE_END, strlen(TERM_CLEAR_LINE_END));
								for(i=buff->cur_pos; i>0; i--)
										write(fd_out, "\b \b", 3);

								/* Print command */
								memset(buff->line, '\0', buff->size);
								buff->cur_pos = str_l;
								buff->size	  = str_l;
								strcpy(&buff->line[1], str);
								write(fd_out, str, str_l);
						}

			} else if(strncmp(keys, LEFT_ARROW, 3) == 0) {	/* Left arrow	*/

					if(buff->cur_pos > 0) {
							write(fd_out, "\b", 1);
							buff->cur_pos--;
					} else {
							write(fd_out, "\a", 1);
					}

			} else if(strncmp(keys, RIGHT_ARROW, 3) == 0) {	/* Right arrow	*/

					if(buff->cur_pos < buff->size) {
							write(fd_out, &buff->line[buff->cur_pos+1], 1);
							buff->cur_pos++;
					} else if(buff->cur_pos == buff->size) {
							write(fd_out, "\a", 1);
					}

			} else if(strncmp(keys, DEL_KEY, 3) == 0) {		/* Delete		*/

					if(buff->cur_pos >= 0 && buff->cur_pos < buff->size) {

							unsigned long j, pf;

							for(i=(buff->cur_pos + 1), j=0; i<=buff->size; i++, j++) {
									buff->line[i] = buff->line[i+1];
							}
							pf = j + 2;
							buff->line[buff->size]     = ' ';
							buff->line[buff->size + 1] = '\0';
							write(fd_out, &buff->line[buff->cur_pos+1], pf);

							/* Put cursor on rigth position */
							for(j=0; j<(pf-2); j++)
									write(fd_out, "\b", 1);

							buff->size--;
					} else {
							write(fd_out, "\a", 1);
					}
			} else if(strncmp(keys, END_KEY, 3) == 0 ||
						strncmp(keys, END_KEY2, 3) == 0) {	/* End			*/

					/* Move cursor to the end of line */
					if(buff->cur_pos < buff->size) {
							do {
								buff->cur_pos++;
								write(fd_out, &buff->line[buff->cur_pos], 1);
							} while(buff->cur_pos < buff->size);
					}
			} else if(strncmp(keys, HOME_KEY, 3) == 0 ||
						strncmp(keys, HOME_KEY2, 3) == 0) {	/* Home			*/

					/* Move cursor to the start of line */
					if(buff->cur_pos > 0) {
							while(buff->cur_pos > 0) {
								buff->cur_pos--;
								write(fd_out, "\b", 1);
							}
					}
					//buff->cur_pos = 0;
			}

			keys[0] = '\0';
		} else {

			switch(keys[0]) {

					case BACKSPACE_KEY:	/* Delete a char */
										if(buff->cur_pos > 0) {
											if(buff->cur_pos == buff->size) {
													write(fd_out, "\b \b", 3);
													buff->line[buff->size] = '\0';
											} else {
													unsigned long j;

													write(fd_out, "\b", 1);
													for(i=buff->cur_pos, j=0; i<=buff->size; i++, j++) {
															buff->line[i] = buff->line[i+1];
													}
													buff->line[buff->size]     = ' ';
													buff->line[buff->size + 1] = '\0';
													write(fd_out, &buff->line[buff->cur_pos], (j + 2));

													/* Put cursor on rigth position */
													for(j=buff->size; j>=buff->cur_pos; j--)
															write(fd_out, "\b", 1);
											}
											buff->size--;
											buff->cur_pos--;
										} else {
											write(fd_out, "\a", 1);
										}
										break;

					case CTRL_K:		/* Clear to end of line */
										write(fd_out, TERM_CLEAR_LINE_END, strlen(TERM_CLEAR_LINE_END));
										memset(&buff->line[buff->cur_pos + 1], '\0', (buff->size - buff->cur_pos));
										buff->size = buff->cur_pos;
										break;

					case CTRL_L:		/* Clear screen */
										write(fd_out, TERM_CLEAR_SCREEN, strlen(TERM_CLEAR_SCREEN));
										write_prompt();
										continue;

					case CTRL_W:		/* Remove a word */
										{
											int j, i;
											ssize_t n;

											i = 0;
											while(buff->line[buff->cur_pos - i] == ' ')
												i++;

											while(buff->line[buff->cur_pos - i] != ' ' && (buff->cur_pos - i) > 0)
												i++;

											if( (buff->cur_pos - i) >= 0) {
												/* Remove from string */
												for(j=(buff->cur_pos - i + 1); j<(buff->size - i + 1); j++) {
													buff->line[j] = buff->line[j + i];
												}

												/* Back cursor and clear */
												for(j=0; j<i; j++) {
													write(fd_out, "\b", 1);
													buff->line[buff->size - j] = '\0';
												}
												write(fd_out, TERM_CLEAR_LINE_END, strlen(TERM_CLEAR_LINE_END));
												buff->cur_pos -= i;
												buff->size    -= i;
												n = write(fd_out, &buff->line[buff->cur_pos + 1], (buff->size - buff->cur_pos));


												/* Ajust the cursor */
												for(j=0; j<n; j++)
													write(fd_out, "\b", 1);

											}
										}
										break;

					case TAB_KEY: 		/* Simple auto complete */
										{
										 char *str;
										 char *ret;
										 int j;

										 if(buff->cur_pos < buff->size)
										 	break;

										 str = (char*)malloc(sizeof(char) * buff->size);
										 if(str == NULL)
										 	break;

										 strcpy(str, &buff->line[1]);
										 ret = autocomplete(str);

										 if(ret != NULL) {
										 	for(j=buff->cur_pos; j>0; j--) {
												write(fd_out, "\b \b", 3);
												buff->line[j] = '\0';
											}
											buff->cur_pos = strlen(ret);
											buff->size    = strlen(ret);
											strcpy(&buff->line[1], ret);

										    write(fd_out, ret, strlen(ret));
											free(ret);
										 }

										 free(str);
										}
							  			break;

					case ENTER_KEY:		/* ENTER key */
										write(fd_out, "\n", 1);
										break;

					case CTRL_R:		/* TODO: Ctrl+R - Search on hash table */
										break;

					default: 			/* Write to screen */
										if(buff->cur_pos > buff->max_size) {
												/* Re-allocate memory */
												char *new_line;
												new_line = realloc(buff->line, (buff->max_size + DEFAULT_BSIZE) );
												if(new_line != NULL) {
														buff->line      = new_line;
														buff->max_size += DEFAULT_BSIZE;
												} else {
														/* No more memory for string, play a beep */
														write(fd_out, "\a", 1);
														break;
												}
										}

										if(buff->cur_pos == buff->size) {
												buff->cur_pos++;
												buff->line[buff->cur_pos] = keys[0];
												write(fd_out, &keys[0], 1);
										} else {
												/* Move text on screen */
												unsigned long j;

												for(i=(buff->size+1), j=1; i>buff->cur_pos; i--, j++) {
														buff->line[i] = buff->line[i-1];
												}
												buff->line[buff->cur_pos+1] = keys[0];
												write(fd_out, &buff->line[buff->cur_pos+1], j);

												/* Put cursor on rigth position */
												for(j=buff->size; j>buff->cur_pos; j--)
														write(fd_out, "\b", 1);

												buff->cur_pos++;
										}
										buff->size++;
			}
		}
	}
	buff->line[buff->size + 1] = '\0';
	temp_str = malloc(sizeof(char) * strlen(&buff->line[1]));
	if(temp_str != NULL)
			strcpy(temp_str, &buff->line[1]);

	/* Add to the history */
	if(clist_get_size(c_history) >= HISTORY_SIZE) {
			remove_string(&c_hashtable, (char*)c_history->prev->element);
			clist_remove(&c_history, HISTORY_SIZE - 1);
	}

	if(c_history != NULL && temp_str != NULL) {
			e_history = c_history->prev;

			if(strcmp(temp_str, e_history->element) != 0 &&
							strcmp(temp_str, "") != 0) {

					clist_add(&c_history, temp_str);
					add_string(&c_hashtable, temp_str);
					comm_hn++;
			}
	} else {

			if(temp_str != NULL) {
					if(strcmp(temp_str, "") != 0) {
							clist_add(&c_history, temp_str);
							add_string(&c_hashtable, temp_str);
							comm_hn++;
					}
			}
	}

	/* Remove null string */
	i = clist_search(c_history, "", 1);
	if(i != -1) {
			clist_remove(&c_history, i);
	}

	/* Increment command number */
	comm_n++;

	/* Return the line */
	return(&buff->line[1]);
}


/**
 * Read a 'PS#' variable string and convert to a printable string
 *
 * @author Renê de Souza Pinto
 * @param ps_str The 'PS#' string variable
 * @param p_str  Pointer to the new printable string
 * @return int Put the printable string in p_str, and return your length
 */
char *read_ps_string(char *ps_str, size_t *u_size) {

	size_t i, p;
	size_t s_size, v_size;
	char *new_str;
	char c_char;

	new_str = (char*)malloc(sizeof(char) * (DEFAULT_BSIZE + strlen(ps_str)));
	if(new_str == NULL)
			return(NULL);

	/* Read ps_str */
	s_size     = DEFAULT_BSIZE + strlen(ps_str);
	v_size     = 0;
	new_str[0] = '\0';
	i          = 0;
	c_char     = 1;
	while(i < strlen(ps_str)) {

			char c = ps_str[i];

			if(c == '\\') {
					char str_date[MAX_STRING_SIZE];
					char str_cdate[MAX_STRING_SIZE];
					struct passwd *pwd;
					time_t t;
					struct tm *tp;
					char *str;
					uid_t uid;
					char fm_ok = 0;
					char c2    = ps_str[i+1];


					/* Get the current time and date */
					t  = time(NULL);
					tp = localtime(&t);
					if(tp == NULL)
							return(NULL);

					/* Check the special character */
					switch(c2) {

							case 'a': /* A bell */
									  strrcat(new_str, "\a", s_size);
									  break;

							case 'd': /* The date in "Weekday Month Date" format (e.g., "Tue May 26") */
									  if(strftime(str_date, MAX_STRING_SIZE, "\%a \%B \%d", tp) != 0) {
											  strrcat(new_str, str_date, s_size);
											  v_size += strlen(str_date);
									  }
									  break;

							case 'D': /* 
									   * The format is passed to strftime(3) and the result is inserted 
									   * into the prompt string; an empty format results in a default 
									   * time representation.
									   */
									  i += 2;
									  p  = i;
									  c  = ps_str[p];
									  if(c == '{') {
											  while(p < strlen(ps_str) && (p-i) < MAX_STRING_SIZE) {
													  p++;
													  c = ps_str[p];
													  if(c == '}') {
															  fm_ok = 1;
															  break;
													  } else {
															  str_cdate[p - i - 1] = c;
													  }
									  		  }
											  str_cdate[p - i - 1] = '\0';
									  }
									  if(!fm_ok) {
											  strcpy(str_cdate, "\%a \%B \%d \%H:\%M");
											  i -= 2;
									  } else {
											  i = p - 1;
									  }
									  /* Call strftime */
 									  if(strftime(str_date, MAX_STRING_SIZE, str_cdate, tp) != 0) {
											  strrcat(new_str, str_date, s_size);
											  v_size += strlen(str_date);
									  }
									  break;

							case 'e': /* An ASCII escape character */
									  strrcat(new_str, "\033", s_size);
									  break;

							case 'h': /* The hostname up to the first `.' */
									  str = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);
									  if(str != NULL) {
											  /* Machine name */
											  if(gethostname(str, MAX_STRING_SIZE) == 0) {
													  strrcat(new_str, str, s_size);
													  v_size += strlen(str);
											  }
											  free(str);
									  }
									  break;

							case 'H': /* The hostname */
									  str = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);
									  if(str != NULL) {
											  /* Machine name */
											  if(gethostname(str, MAX_STRING_SIZE) == 0) {
													  strcat(str, ".");
													  strrcat(new_str, str, s_size);
													  v_size += strlen(str);

													  /* Domain name */
													  if(getdomainname(str, MAX_STRING_SIZE) == 0) {
															  strrcat(new_str, str, s_size);
															  v_size += strlen(str);
													  }
											  }
											  free(str);
									  }
									  break;

							case 'j': /* The number of jobs currently managed by the shell */
									  str = (char*)malloc(sizeof(char) * 20); /* good enough for unsigned int */
									  if(str != NULL) {
											  sprintf(str, "%d", n_jobs);
											  strrcat(new_str, str, s_size);
									  		  v_size += strlen(str);
											  free(str);
									  }
									  break;

							case 'l': /* The basename of the shell's terminal device name */
									  p = strlen(term_name) - 1;
									  while(p > 0) {
											  if(term_name[p] != '/') {
													  p--;
											  } else {
													  p++;
													  break;
											  }
									  }
									  strrcat(new_str, &term_name[p], s_size);
									  v_size += strlen(&term_name[p]);
									  break;

							case 'n': /* Newline */
									  strrcat(new_str, "\n", s_size);
									  break;

							case 'r': /* Carriage return */
									  strrcat(new_str, "\r", s_size);
									  break;

							case 's': /* The name of the shell, the basename of $0 (the portion following the final slash) */
									  strrcat(new_str, name_of_cash, s_size);
									  v_size += strlen(name_of_cash);
									  break;

							case 't': /* The current time in 24-hour HH:MM:SS format */
									  if(strftime(str_date, MAX_STRING_SIZE, "\%H:\%M:\%S", tp) != 0) {
											  strrcat(new_str, str_date, s_size);
											  v_size += strlen(str_date);
									  }
									  break;

							case 'T': /* The current time in 12-hour HH:MM:SS format */
 									  if(strftime(str_date, MAX_STRING_SIZE, "\%I:\%M:\%S", tp) != 0) {
											  strrcat(new_str, str_date, s_size);
											  v_size += strlen(str_date);
									  }
									  break;

							case '@': /* The current time in 12-hour am/pm format */
									  if(strftime(str_date, MAX_STRING_SIZE, "\%I:\%M \%P", tp) != 0) {
											  strrcat(new_str, str_date, s_size);
											  v_size += strlen(str_date);
									  }
									  break;

							case 'A': /* The current time in 24-hour HH:MM format */
									  if(strftime(str_date, MAX_STRING_SIZE, "\%H:\%M", tp) != 0) {
											  strrcat(new_str, str_date, s_size);
											  v_size += strlen(str_date);
									  }
									  break;

							case 'u': /* The username of the current user */
									  pwd = getpwuid(geteuid());
									  if(pwd != NULL) {
											  strrcat(new_str, pwd->pw_name, s_size);
											  v_size += strlen(pwd->pw_name);
									  }
									  break;

							case 'v': /* The version of cash (e.g., 1.00) */
									  strrcat(new_str, VERSION, s_size);
									  v_size += strlen(VERSION);
									  break;

							case 'V': /* The release of cash, version + patch level (e.g., 1.00.0) */
									  strrcat(new_str, RELEASE, s_size);
									  v_size += strlen(RELEASE);
									  break;

							case 'w': /* The current working directory, with $HOME abbreviated with a tilde */
									  str = (char*)malloc(sizeof(char) * MAX_PATH_STRING_SIZE);
									  if(str != NULL) {
											  char *hdir;
											  size_t len;

											  /* Get current working directory and home directory */
											  str  = getcwd(str, MAX_PATH_STRING_SIZE);
											  hdir = getenv("HOME");

											  len = strlen(hdir);
											  if(strncmp(str, hdir, len) == 0) {
													  strrcat(new_str, "~", s_size);
													  v_size++;

													  strrcat(new_str, &str[len], s_size);
													  v_size += strlen(&str[len]);
											  } else {
											  		  strrcat(new_str, str, s_size);
											  		  v_size += strlen(str);
											  }
											  free(str);
									  }
									  break;

							case 'W': /* The basename of the current working directory, with $HOME abbreviated with a tilde */
 									  str = (char*)malloc(sizeof(char) * MAX_PATH_STRING_SIZE);
									  if(str != NULL) {
											  char *hdir;
											  size_t len;

											  /* Get current working directory and home directory */
											  str  = getcwd(str, MAX_PATH_STRING_SIZE);
											  hdir = getenv("HOME");

											  len = strlen(hdir);
											  if(strcmp(str, hdir) == 0) {
													  strrcat(new_str, "~", s_size);
													  v_size++;
											  } else {
											  		  /* Get the basename */
													  p = strlen(str) - 1;
													  if(strlen(str) == 1) {
															  p = 0;
													  } else {
															  while(p >= 0) {
																	  if(str[p] != '/') {
																			  p--;
											  				  		  } else {
																			  p++;
													  				  		  break;
											  		  		  		  }
									          		 		  }
													  }
													  strrcat(new_str, &str[p], s_size);
									  				  v_size += strlen(&str[p]);
											  }
											  free(str);
									  }
									  break;

							case '!': /* The history number of this command */
									  str = (char*)malloc(sizeof(char) * 40); /* good enough for unsigned long int */
									  if(str != NULL) {
											  sprintf(str, "%d", comm_hn);
											  strrcat(new_str, str, s_size);
									  		  v_size += strlen(str);
											  free(str);
									  }
									  break;

							case '#': /* The command number of this command */
									  str = (char*)malloc(sizeof(char) * 20); /* good enough for unsigned int */
									  if(str != NULL) {
											  sprintf(str, "%d", comm_n);
											  strrcat(new_str, str, s_size);
									  		  v_size += strlen(str);
											  free(str);
									  }
									  break;

							case '$': /* If the effective UID is 0, a #, otherwise a $ */
									  uid = getuid();
									  if(uid == 0) {
											  strrcat(new_str, "#", s_size);
									  } else {
											  strrcat(new_str, "$", s_size);
									  }
									  v_size++;
									  break;

							case '\\': /* A backslash */
									   strrcat(new_str, "\\", s_size);
									   v_size++;
									   break;

							case '[': /* Do not count the next chars while \] not found */
									  c_char = 0;
									  break;

							case ']': /* Back to count the chars */
									  c_char = 1;
									  break;

							default:  /* Check for a octal number */
									  if( isdigit(c2) && (i+3) < strlen(ps_str) ) {
											  char ch, d2, d3;
											  char num_str[4];

											  d2 = ps_str[i+2];
											  d3 = ps_str[i+3];

 											  /* Check and print the character corresponding to the octal number nnn */
											  if(isdigit(d2) && isdigit(d3)) {
													  /* Convert */
													  sprintf(num_str, "%c%c%c", c2, d2, d3);
													  ch = oct2dec(num_str);
													  sprintf(num_str, "%c", ch);

													  /* Print */
													  strrcat(new_str, num_str, s_size);
													  v_size++;
													  i+=2;
											  } else {
													  strrcat(new_str, "\\", s_size);
													  v_size++;
													  i--;
											  }
									  } else {
											  strrcat(new_str, "\\", s_size);
											  v_size++;
											  i--;
									  }
									  break;

					}

					i++;

			} else {
					char cstr[2];

					cstr[0] = c;
					cstr[1] = '\0';

					strrcat(new_str, cstr, s_size);
					if(c_char)
							v_size++;
			}

			/* Check string size */
			if(strlen(new_str) > s_size)
					s_size = strlen(new_str);

			/* Increment counter */
			i++;
	}

	/* Return */
	*u_size = v_size;
	return(new_str);
}


/**
 * Write the cash prompt
 *
 * @author Renê de Souza Pinto
 */
void write_prompt(void) {
	char *ps1;
	size_t ps_size;

	/* Convert PS1 variable an print the correct string */
	ps1 = getenv("PS1");
	if(ps1 != NULL) {
			char *str_ps1 = read_ps_string(ps1, &ps_size);
			if(str_ps1 != NULL) {
					write(fd_out, str_ps1, strlen(str_ps1));
					free(str_ps1);
			}
	} else {
			write(fd_out, DEFAULT_PS1, strlen(DEFAULT_PS1));
	}
}


/**
 * Concatenate two strings with memory reallocation when necessary
 *
 * @author Renê de Souza Pinto
 * @param dest The destination string
 * @param src String to be appended with dest
 * @param size_dest Size of dest memory
 * @return char* Pointer to new string or null, if memory allocation failed
 */
char *strrcat(char *dest, char *src, size_t dest_size) {

	size_t total;
	char *new_str;

	if(dest == NULL || src == NULL)
			return(NULL);

	/* Calculate the total size of dest */
	total = strlen(dest) + strlen(src) + 1;

	/* Reallocate memory if necessary */
	if(dest_size < total) {
			new_str = (char*)realloc(dest, sizeof(char) * total);
			if(new_str == NULL) {
					return(NULL);
			} else {
					dest = new_str;
			}
	}

	/* Concatenate the strings and return */
	strcat(dest, src);
	return(dest);
}


/**
 * Convert octal string to decimal number
 *
 * @author Renê de Souza Pinto
 * @param octn The octal number in String format
 * @return int octn in decimal base
 */
int oct2dec(char *octn) {

	int x;
	int mult = 1;
	int res  = 0;

	for(x=(strlen(octn)-1); x>=0; x--) {
			if( !isdigit(octn[x]) )
					return(0);

			int dig = (int)octn[x] - 48;
			res    += (dig * mult);
			mult   *= 8;
	}

	return(res);
}

