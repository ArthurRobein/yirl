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

#ifndef _YIRL_COUNTER_H_
#define _YIRL_COUNTER_H_

typedef struct {
  uint64_t beg;
} YTimer;

#ifdef Y_INSIDE_TCC
uint64_t g_get_monotonic_time(void);
#else
#include <glib.h>
#endif

/**
 * Create a counter and start it
 */
static inline YTimer *YTimerCreate(void)
{
  YTimer *ret = g_new(YTimer, 1);

  ret->beg = g_get_monotonic_time();
  return ret;
}

static inline void YTimerReset(YTimer *cnt)
{
  cnt->beg = g_get_monotonic_time();
}

static inline uint64_t YTimerGet(YTimer *cnt)
{
  return g_get_monotonic_time() - cnt->beg;
}

#endif
