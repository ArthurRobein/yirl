/*
**Copyright (C) 2015 Matthias Gatto
**
**This program is free software: you can redistribute it and/or modify
**it under the terms of the GNU Lesser General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.
**
**This program is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**GNU General Public License for more details.
**
**You should have received a copy of the GNU Lesser General Public License
**along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _YIRL_ENTITY_SCRIPT_H_
#define _YIRL_ENTITY_SCRIPT_H_

#include "yirl/script.h"
#include "yirl/entity.h"

#ifndef Y_INSIDE_TCC

#include <stdarg.h>

void *yesVCall(Entity *func, va_list ap);

#endif

void *yesCallInt(Entity *func, ...);

#define yesCall0(func) yesCallInt(func, Y_END_VA_LIST)

#define yesCall(func, args...) yesCallInt(func,				\
					  YUI_VA_ARGS_HANDELER(Y_END_VA_LIST, \
							       args))

#endif
