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

#ifndef _SDL_INTERNAL_H_
#define _SDL_INTERNAL_H_

#include "sdl-driver.h"
#include "widget.h"

#define WIN_W_SIZE 640
#define WIN_H_SIZE 480

typedef struct
{
  int           fullscreen;
  SDL_Window	*pWindow;
  TTF_Font	*font;
  SDL_Renderer	*renderer;
} SDL_Global;

typedef struct
{
  YWidgetState *wid;
  SDL_Rect      rect;
} SDLWid;

void sdlResize(YWidgetState *wid, int renderType);

void sdlWidInit(YWidgetState *wid, int t);

void sdlFillColorBg(SDLWid *swid, short r, short g, short b, short a);

void sdlFillImgBg(SDLWid *swid, const char *cimg);

void sdlWidDestroy(YWidgetState *wid, int t);

SDL_Rect  getRect(void);
SDL_Surface *wSurface(void);
SDL_Renderer *sgRenderer(void);
TTF_Font *sgDefaultFont(void);
int sgSetDefaultFont(const char *path);

#endif