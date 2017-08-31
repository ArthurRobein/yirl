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

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern "C" {
#include <glib.h>
#include "sdl-internal.h"
#include "yirl/canvas.h"
#include "yirl/rect.h"
#include "yirl/entity.h"
}

static int sdl2Render(YWidgetState *state, int t)
{
  SDLWid *wid = reinterpret_cast<SDLWid *>(ywidGetRenderData(state, t));

  printf("rendering canvas %p\n", wid);
  return -1;
}

static int sdl2Init(YWidgetState *wid, int t)
{
  wid->renderStates[t].opac = g_new(SDLWid, 1);
  sdlWidInit(wid, t);
  return 0;
}

extern "C" {
  int ysdl2RegistreCanvas(void)
  {
    int ret = ywidRegistreTypeRender("canvas", ysdl2Type(),
				     sdl2Render, sdl2Init, sdlWidDestroy);
    return ret;
  }
}
