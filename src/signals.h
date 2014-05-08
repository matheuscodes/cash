/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at renesp.com.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * builtin_c.h
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

#ifndef __CASH_SIGNALS__

	#define __CASH_SIGNALS__

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <signal.h>
	#include <unistd.h>
	#include "linkedl.h"
	#include "builtin_c.h"
	#include "do_command.h"

	/* Prototypes */
	void start_signals(void);
	void handle_sigint(int id);
	void handle_sigstop(int id);
	void change_dir_sig(int sig);

#endif /* __CASH_SIGNALS___ */
