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

#ifndef _YIRL_GAME_H_
#define _YIRL_GAME_H_

#include "yirl/entity.h"
#include "yirl/widget.h"
#include "yirl/utils.h"
#include "yirl/map.h"
#include "yirl/sound.h"

#ifndef Y_INSIDE_TCC
#include "yirl/native-script.h"
#include "yirl/script.h"
#else
#define Y_END_VA_LIST ((void *)0xDEAD0000)
#endif

typedef enum {
  NONE   = 0,
  SDL2   = 1,
  CURSES = 2,
  ALL    = CURSES | SDL2
} RenderType;

typedef enum {
  YJSON = 0,
  YRAW_FILE,
  END_YFILETYPE
} YFileType;

typedef struct {
  const char *path;
} ModuleConf;

typedef struct RenderConf_ {
  const char *name;
} RenderConf;

#define GList void

typedef struct {
  ModuleConf *startingMod;
  GList *rConf;
  const char *win_name;
  int w;
  int h;
} GameConfig;

#undef GList

extern char *ygBinaryRootPath;

static inline void ygBinaryRootPathFree(void)
{
  free(ygBinaryRootPath);
  ygBinaryRootPath = "./";
}

#define ygInitGameConfig(cfg, path, render)				\
  _Generic((render),							\
	   const char *: ygInitGameConfigByStr,				\
	   char *: ygInitGameConfigByStr,				\
	   Y_GEN_CLANG_ARRAY(char, ygInitGameConfigByStr),		\
	   RenderType : ygInitGameConfigByRenderType,			\
	   int : ygInitGameConfigByRenderType) (cfg, path, render)

int ygInitGameConfigByRenderType(GameConfig *cfg,
				 const char *path, RenderType t);
int ygInitGameConfigByStr(GameConfig *cfg, const char *path,
			  const char *render);

void *ygCallInt(const char *mod, const char *callName, ...);

/* because tcc bug with 0 args */
#define ygVCall(mod, callName) ygCallInt(mod, callName, Y_END_VA_LIST)

#define ygCall(mod, callName, args...)					\
	ygCallInt(mod, callName,					\
		  YUI_VA_ARGS_HANDELER(Y_END_VA_LIST,			\
				       args))

int ygRegistreFuncInternal(void *manager, int nbArgs, const char *name,
			   const char *toRegistre);

#ifndef Y_INSIDE_TCC
static inline int ygRegistreFunc(void *manager, int nbArgs, const char *name,
				 const char *toRegistre)
{
	return ygRegistreFuncInternal(manager, nbArgs, name, toRegistre);
}
#else
static inline int ygRegistreFunc(int nbArgs, const char *name,
				 const char *toRegistre)
{
	return ygRegistreFuncInternal(ygGetTccManager(), nbArgs,
				      name, toRegistre);
}
#endif

Entity *ygFileToEnt(YFileType t, const char *path, Entity *father);
int ygEntToFile(YFileType t, const char *path, Entity *ent);

void ygCleanGameConfig(GameConfig *cfg);

int ygInit(GameConfig *config);
int ygIsInit(void);

/**
 * @return true is the game loop is looping
 */
int ygIsAlive(void);

int ygStartLoop(GameConfig *config);
int ygDoLoop(void);

Entity *ygLoadMod(const char *path);
Entity *ygGetMod(const char *path);
Entity *ygGetFuncExt(const char *func);

int ygLoadScript(Entity *mod, void *manager, const char *path);

void *ygGetManager(const char *name);

void ygEnd(void);

/* scrits managers */
void *ygGetLuaManager(void);
void *ygGetTccManager(void);

int ygBind(YWidgetState *wid, const char *callback);

Entity *ygGet(const char *toFind);
void ygSetInt(const char *toSet, int val);

int ygAddDefine(const char *name, char *val);

/**
 * @brief   push @ent to yirl global scope
 * @param   ent entity to push
 * @param   name mandatory name
 */
int yePushToGlobalScope(Entity *ent, const char *name);

#endif
