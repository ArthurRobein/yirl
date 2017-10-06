/*
**Copyright (C) 2017 Matthias Gatto
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

#ifndef	_YIRL_CANVAS_H_
#define	_YIRL_CANVAS_H_

#include "yirl/widget.h"

int ywCanvasInit(void);
int ywCanvasEnd(void);
int ysdl2RegistreCanvas(void);
/**
 * add @pos to object position at @objIdx
 */
int ywCanvasMoveObjByIdx(Entity *wid, int objIdx, Entity *pos);

/**
 * @wid an entity that contain an elem "resources"
 *	generally a canvas widget but can be any entity as long as this entity
 *	have a "resources" field
 */
Entity *ywCanvasObjSize(Entity *wid, Entity *obj);

#endif