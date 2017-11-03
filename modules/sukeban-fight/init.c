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

#include <yirl/game.h>
#include <yirl/container.h>
#include <yirl/menu.h>
#include <yirl/canvas.h>
#include <yirl/rect.h>
#include <yirl/timer.h>

void rendMap(Entity *ent)
{
  Entity *map = yeGet(ent, "_map");
  Entity *canvas = ywCntGetEntry(ent, 0);
  Entity *objs = yeGet(canvas, "objs");
  Entity *size = yeGet(canvas, "wid-pix");
  int widthQuarter = ywRectW(size) / 4;
  int hightThier = ywRectH(size) / 3;

  for (int x = 0; x < 4; ++x) {
    for (int y = 0; y < 3; ++y) {
      Entity *obj = yeGet(yeGet(map, x), y);
      Entity *objSize;

      if (!obj)
	continue;
      objSize = ywCanvasObjSize(ent, obj);
      yeRemoveChildByStr(obj, "pos");
      ywPosCreate(widthQuarter * x + widthQuarter / 2 - ywPosX(objSize) / 2,
		  hightThier * y  + hightThier / 2 - ywPosY(objSize) / 2,
		  obj, "pos");
      yePushBack(objs, obj, NULL);
    }
  }
}

static Entity *getMenuCnt(Entity *wid)
{
  return ywCntGetEntry(wid, 1);
}

static Entity *getLoader(Entity *widget)
{
  Entity *menu_cnt = getMenuCnt(widget);
  return yeGet(menu_cnt, "loader");
}

static Entity *getLoaderAt(Entity *main, int pos)
{
  ywCntGetEntry(getLoader(main), pos);
}

void *sukeFightAttack(int nbArg, void **args)
{
  Entity *menu = args[0];
  Entity *main = yeGet(menu, "main");
  Entity *pcDoingActionIdx = yeGet(main, "pcDoingAction");

  if (yeGetInt(pcDoingActionIdx) < 0) {
    return (void *)NOTHANDLE;
  }
  printf("attack attack attack ! ore wa senshi ! %p\n",
	 yeGet(getLoaderAt(main, yeGetInt(pcDoingActionIdx)), "guy"));
  return (void *)NOTHANDLE;
}

void *sukeFightAction(int nbArg, void **args)
{
  Entity *ent = args[0];
  Entity *pcDoingAction = yeGet(ent, "pcDoingAction");

  if (yeGetInt(pcDoingAction) >= 0) {
    return (void *)NOTHANDLE;
  }

  void *timer = yeGetDataAt(ent, "timer");
  uint64_t t = YTimerGet(timer) / 100000;
  Entity *loader = getLoader(ent);
  ywContainerUpdate(ent, loader);

  for (int j = 0; j < 6; ++j) {
    Entity *curLoadingBar = ywMenuGetEntry(loader, j);
    Entity *guy = yeGet(curLoadingBar, "guy");

    if (!guy)
      break;
    Entity *curTime = yeGet(guy, "current-reload-time");

    yeSetInt(curTime, t);
    Entity *cur_bar = ywCntGetEntry(loader, j);
    Entity *percent = yeGet(cur_bar, "loading-bar-%");
    yeSetInt(percent, yeGetInt(curTime) * 100 /
	     yeGetInt(yeGet(guy, "reload-time")));
    if (yeGetInt(percent) == 100) {
      yeSetInt(pcDoingAction, j);
      return (void *)NOTHANDLE;
    }
  }

  return (void *)NOTHANDLE;
}

static void tryPushGuyToMenu(Entity *menu_cnt, Entity *guy)
{
  Entity *name = yeGet(guy, "name");
  Entity *loader = yeGet(menu_cnt, "loader");
  Entity *loader_entries = yeGet(loader, "entries");
  Entity *menu = ywCntGetEntry(menu_cnt, 0);
  Entity *menu_entries = yeGet(menu, "entries");

  if (name) {
    yeReCreateInt(0, guy, "current-reload-time");
    Entity *menu_entry = yeCreateArray(menu_entries, NULL);
    yeCreateString(yeGetString(name), menu_entry, "text");
    Entity *loader_entry = yeCreateArray(loader_entries, NULL);
    yeCreateString(NULL, loader_entry, "text");
    yeCreateString("loading-bar", loader_entry, "type");
    yeCreateInt(0, loader_entry, "loading-bar-%");
    yePushBack(loader_entry, guy, "guy");
  }
}

void *sukeFightInit(int nbArg, void **args)
{
  Entity *ent = args[0];
  Entity *entries = yeCreateArray(ent, "entries");
  Entity *bad_guys = yeGet(ent, "bad-guys");
  Entity *good_guys = yeGet(ent, "good-guys");
  Entity *canvas;
  Entity *menu_cnt;
  Entity *menu_cnt_entries;
  Entity *menu_caracters_list;
  Entity *menu;
  Entity *menu_entries;
  Entity *menu_entry;
  Entity *map = yeReCreateArray(ent, "_map", NULL);
  Entity *rows[4];
  void *ret;
  Entity *loader;
  Entity *loader_entries;

  yeReCreateInt(1, ent, "current");
  yeReCreateInt(-1, ent, "pcDoingAction");
  yeCreateFunction("sukeFightAction", ygGetManager("tcc"), ent, "action");
  YTimerReset(yeGetData(yeCreateDataExt(NULL, ent, "timer",
					YE_DATA_USE_OWN_METADATA)));
  for (int i = 0; i < 4; ++i)
    rows[i] = yeCreateArray(map, NULL);
  canvas = yeCreateArray(entries, NULL);
  yeCreateString("canvas", canvas, "<type>");
  yeCreateArray(canvas, "objs");
  yeReplaceBack(canvas, yeGet(ent, "resources"), "resources");
  Entity *good_front_row = yeGet(good_guys, 0);
  Entity *good_back_row = yeGet(good_guys, 1);
  Entity *bad_front_row = yeGet(bad_guys, 0);
  Entity *bad_back_row = yeGet(bad_guys, 1);

  for (int i = 0; i < 3; ++i) {
    Entity *guy = yeGet(good_front_row, i);
    if (guy) {
      yePushAt(rows[2], guy, i);
    }
    guy = yeGet(good_back_row, i);
    if (guy) {
      yePushAt(rows[3], guy, i);
    }
    guy = yeGet(bad_front_row, i);
    if (guy) {
      yePushAt(rows[1], guy, i);
    }
    guy = yeGet(bad_back_row, i);
    if (guy) {
      yePushAt(rows[0], guy, i);
    }
  }
  yeCreateInt(70, canvas, "size");
  menu_cnt = yeCreateArray(entries, NULL);
  yeCreateString("container", menu_cnt, "<type>");
  menu_cnt_entries = yeCreateArray(menu_cnt, "entries");
  yeCreateString("vertical", menu_cnt, "cnt-type");
  yeReCreateInt(2, menu_cnt, "current");
  yeCreateString("rgba: 123 123 255 255", menu_cnt, "background");

  menu_caracters_list = yeCreateArray(menu_cnt_entries, NULL);
  yeCreateString("menu", menu_caracters_list, "<type>");
  yeReCreateInt(-1, menu_caracters_list, "current");
  yeCreateInt(20, menu_caracters_list, "size");
  menu_entries = yeCreateArray(menu_caracters_list, "entries");

  loader = yeCreateArray(menu_cnt_entries, NULL);
  yeCreateString("menu", loader, "<type>");
  yePushBack(menu_cnt, loader, "loader");
  yeCreateArray(loader, "entries");
  yeReCreateInt(-1, loader, "current");

  for (int i = 0; i < 3; ++i) {
    Entity *guy = yeGet(good_front_row, i);

    if (guy)
      tryPushGuyToMenu(menu_cnt, guy);
    guy = yeGet(good_back_row, i);
    if (guy)
      tryPushGuyToMenu(menu_cnt, guy);
  }

  menu = yeCreateArray(menu_cnt_entries, NULL);
  yeCreateString("menu", menu, "<type>");
  yePushBack(menu, ent, "main");
  menu_entries = yeCreateArray(menu, "entries");
  menu_entry = yeCreateArray(menu_entries, NULL);
  yeCreateString("attack", menu_entry, "text");
  yeCreateFunction("sukeFightAttack", ygGetTccManager(), menu_entry, "action");
  menu_entry = yeCreateArray(menu_entries, NULL);
  yeCreateString("run away", menu_entry, "text");
  yeCreateString("FinishGame", menu_entry, "action");

  ret = ywidNewWidget(ent, "container");
  rendMap(ent);
  return ret;
}

void *init_sukeban_fight(int nbArg, void **args)
{
  Entity *mod = args[0];
  Entity *init = yeCreateArray(NULL, NULL);

  yeCreateString("sukeban-fight", init, "name");
  yeCreateFunction("sukeFightInit", ygGetManager("tcc"), init, "callback");
  ywidAddSubType(init);

  return NULL;
}
