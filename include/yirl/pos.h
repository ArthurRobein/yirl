/*
**Copyright (C) 2016 Matthias Gatto
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

#ifndef _YIRL_POS_H_
#define _YIRL_POS_H_

#include "yirl/entity.h"

#define ywSizeW(size) (ywPosX(size))
#define ywSizeH(size) (ywPosY(size))

static inline int ywPosX(Entity *pos)
{
  return yeGetInt(yeGetByIdx(pos, 0));
}

static inline int ywPosY(Entity *pos)
{
  return yeGetInt(yeGetByIdx(pos, 1));
}

static inline void ywPosPrint(Entity *pos)
{
  printf("x: %d - y: %d\n",
	 ywPosX(pos), ywPosY(pos));
}

static inline char * ywPosToString(Entity *pos)
{
  static char tmp[4][256];
  static int i;

  if (unlikely(!pos))
    return (char *)"(nil)";
  ++i;
  i &= 3;
  snprintf(tmp[i], 256, "x: %d - y: %d",
	   yeGetInt(yeGetByIdx(pos, 0)), yeGetInt(yeGetByIdx(pos, 1)));
  return tmp[i];
}

#define ywSizeCreate(w, h, father, name) (ywPosCreate(w, h, father, name))

/**
 * @posX The position in X
 * @posY The position in Y
 * @father the father of the returned entity in which we store the return,
 * can be NULL.
 * @name string index at which we store the returned entity,
 * if NULL but not @father, the return is push back
 * @return an Entity that store a "Position", Must be free if @father is NULL
 */
Entity *ywPosCreateInts(int posX, int posY, Entity *father,
			const char *name);

static inline Entity *ywPosCreateEnt(Entity *other, int useless,
				     Entity *father, const char *name)
{
  (void)useless;
  return ywPosCreateInts(ywPosX(other), ywPosY(other), father, name);
}

#define ywPosCreate(x, y, father, name)			\
  _Generic(x, Entity *: ywPosCreateEnt,			\
	   void *: ywPosCreateEnt,			\
	   const Entity *: ywPosCreateEnt,		\
	   double : ywPosCreateInts,			\
	   int : ywPosCreateInts) (x, y, father, name)

static inline Entity *ywPosSetInts(Entity *pos, int posX, int posY)
{
  yeSetInt(yeGetByIdx(pos, 0), posX);
  yeSetInt(yeGetByIdx(pos, 1), posY);
  return pos;
}

static inline Entity *ywPosSetEnt(Entity *pos, Entity *other,
				  int useless)
{
  (void)useless;
  yeSetInt(yeGetByIdx(pos, 0), yeGetInt(yeGetByIdx(other, 0)));
  yeSetInt(yeGetByIdx(pos, 1), yeGetInt(yeGetByIdx(other, 1)));
  return pos;
}

#define ywPosSet(pos, x, y)			\
  _Generic(x, Entity *: ywPosSetEnt,		\
	   void *: ywPosSetEnt,			\
	   const Entity *: ywPosSetEnt,		\
	   double : ywPosSetInts,		\
	   int : ywPosSetInts) (pos, x, y)


static inline Entity *ywPosSetX(Entity *pos, int posX)
{
  yeSetInt(yeGetByIdx(pos, 0), posX);
  return pos;
}

static inline Entity *ywPosSetY(Entity *pos, int posY)
{
  yeSetInt(yeGetByIdx(pos, 1), posY);
  return pos;
}

static inline int ywPosIsSameEnt(Entity *pos1, Entity *pos2,
				 int useless)
{
  (void)useless;
  return ((yeGetInt(yeGetByIdx(pos1, 0)) == yeGetInt(yeGetByIdx(pos2, 0))) &&
	  (yeGetInt(yeGetByIdx(pos1, 1)) == yeGetInt(yeGetByIdx(pos2, 1))));
}

static inline int ywPosIsSameInts(Entity *pos1, int x, int y)
{
  return ((yeGetInt(yeGet(pos1, 0)) == x) &&
	  (yeGetInt(yeGet(pos1, 1)) == y));
}

#define ywPosIsSame(pos, x, y)			\
  _Generic(x, Entity *: ywPosIsSameEnt,		\
	   void *: ywPosIsSameEnt,			\
	   const Entity *: ywPosIsSameEnt,		\
	   double : ywPosIsSameInts,			\
	   int : ywPosIsSameInts) (pos, x, y)


static inline int ywPosIsSameXEnt(Entity *pos1, Entity *pos2)
{
  if (yeType(pos2) == YINT)
    return (yeGetInt(yeGet(pos1, 0)) == yeGetInt(pos2));
  return (yeGetInt(yeGet(pos1, 0)) == yeGetInt(yeGet(pos2, 0)));
}

static inline int ywPosIsSameXInt(Entity *pos1, int x)
{
  return (yeGetInt(yeGet(pos1, 0)) == x);
}

#define ywPosIsSameX(pos, x)			\
  _Generic(x, Entity *: ywPosIsSameXEnt,	\
	   void *: ywPosIsSameXEnt,		\
	   const Entity *: ywPosIsSameXEnt,	\
	   double : ywPosIsSameXInt,		\
	   int : ywPosIsSameXInt) (pos, x)

static inline int ywPosIsSameYEnt(Entity *pos1, Entity *pos2)
{
  if (yeType(pos2) == YINT)
    return (yeGetInt(yeGet(pos1, 1)) == yeGetInt(pos2));
  return (yeGetInt(yeGet(pos1, 1)) == yeGetInt(yeGet(pos2, 0)));
}

static inline int ywPosIsSameYInt(Entity *pos1, int x)
{
  return (yeGetInt(yeGet(pos1, 1)) == x);
}

#define ywPosIsSameY(pos, x)			\
  _Generic(x, Entity *: ywPosIsSameYEnt,	\
	   void *: ywPosIsSameYEnt,		\
	   const Entity *: ywPosIsSameYEnt,	\
	   double : ywPosIsSameYInt,		\
	   int : ywPosIsSameYInt) (pos, x)

static inline Entity *ywPosAdd(Entity *pos1, Entity *pos2)
{
  yeAddEnt(yeGet(pos1, 0), yeGet(pos2, 0));
  yeAddEnt(yeGet(pos1, 1), yeGet(pos2, 1));
  return pos1;
}

static inline Entity *ywPosSubXY(Entity *pos1, int x, int y)
{
  yeSubInt(yeGet(pos1, 0), x);
  yeSubInt(yeGet(pos1, 1), y);
  return pos1;
}

static inline Entity *ywPosSub(Entity *pos1, Entity *pos2)
{
  yeSubEnt(yeGet(pos1, 0), yeGet(pos2, 0));
  yeSubEnt(yeGet(pos1, 1), yeGet(pos2, 1));
  return pos1;
}

static inline Entity *ywPosMultXY(Entity *pos1, int x, int y)
{
  yeMultInt(yeGet(pos1, 0), x);
  yeMultInt(yeGet(pos1, 1), y);
  return pos1;
}

static inline Entity *ywPosDoPercent(Entity *pos1, int arg)
{
  Entity *x = yeGet(pos1, 0);
  Entity *y = yeGet(pos1, 1);

  yeSetInt(x, yuiPercentOf(yeGetInt(x), arg));
  yeSetInt(y, yuiPercentOf(yeGetInt(y), arg));
  return pos1;
}

static inline Entity *ywPosAddXY(Entity *pos, int x, int y)
{
  yeAddInt(yeGetByIdx(pos, 0), x);
  yeAddInt(yeGetByIdx(pos, 1), y);
  return pos;
}

static inline Entity *ywSegmentFromPos(Entity *posA, Entity *posB,
				       Entity *father, const char *name)
{
  Entity *ret = yeCreateArray(father, name);

  yeCopy(posB, ret);
  ywPosSub(ret, posA);
  return ret;
}

double ywPosAngle(Entity *p0, Entity *p1);

/**
 * move of 1 @from in the direction of @to
 * example: from is 0,0 and to is 0,3, this function will
 * increment X in @from, so @from will be 0,1 so if you call
 * this function with the same argument 3 times @from will be equal to
 * 0,3 (@to value)
 * @return 1 if @from has been modifie
 */
static inline int ywPosMoveToward(Entity *from, Entity *to)
{
  int x = 0, y = 0;

  if (ywPosX(from) > ywPosX(to))
    x = -1;
  else if (ywPosX(from) < ywPosX(to))
    x = 1;

  if (ywPosY(from) > ywPosY(to))
    y = -1;
  else if (ywPosY(from) < ywPosY(to))
    y = 1;

  ywPosAddXY(from, x, y);
  return x || y;
}

#endif
