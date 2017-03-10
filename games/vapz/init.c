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

#include <yirl/game.h>
#include <yirl/menu.h>
#include <yirl/map.h>
#include <yirl/contener.h>
#include <yirl/text-screen.h>

void *init(int nbArgs, void **args)
{
  Entity *mod = args[0];
  Entity *init;
  Entity *map = yeCreateArray(mod, "game");

  yeCreateString("vapz", map, "<type>");
  yePushBack(map, yeGetByStr(mod, "resources.map"), "resources");

  yeCreateFunction("scoreInit", ygGetManager("tcc"), mod, "scoreInit");
  init = yeCreateArray(NULL, NULL);
  yeCreateString("vapz", init, "name");
  yeCreateFunction("vapzInit", ygGetManager("tcc"), init, "callback");
  ywidAddSubType(init);
  return NULL;
}

static void pushBullet(Entity *map, Entity *bulletPos, Entity *bulletDir)
{
  Entity *bullets = yeGetByStrFast(map, "bullets");
  Entity *bullet;

  if (!bullets)
    bullets = yeCreateArray(map, "bullets");

  bullet = yeCreateArray(bullets, NULL);
  yePushBack(bullet, bulletPos, "pos");
  yePushBack(bullet, bulletDir, "dir");
  yeCreateInt(3, bullet, "id");
  ywMapPushElem(map, bullet, bulletPos, "bullet");
}

static void removePizza(Entity *map, Entity *textScreen, Entity *pos,
			Entity *bullet, Entity *toRemove)
{
  Entity *txt = yeGetByStrFast(textScreen, "text");
  Entity *score = yeGetByStrFast(map, "score");
  Entity *bullets = yeGetByStrFast(map, "bullets");
  Entity *pizzas = yeGetByStrFast(map, "pizzas");

  ywMapRemoveByEntity(map, pos, toRemove);
  ywMapRemoveByEntity(map, pos, bullet);
  yeRemoveChild(bullets, bullet);
  yeRemoveChild(pizzas, toRemove);
  yeSetString(txt, "score: ");
  yeAddInt(score, 1);
  yeAddEnt(txt, score);
}

static void bulletsTurn(Entity *map, Entity *textScreen)
{
  Entity *bullets = yeGetByStr(map, "bullets");
  Entity *pizzas = yeGetByStr(map, "pizzas");

  ywMapSetOutBehavior(map, YMAP_OUT_NOTHING);
  YE_ARRAY_FOREACH(bullets, bullet) {
    Entity *pos = yeGetByStr(bullet, "pos");
    Entity *arrayAtPos = ywMapGetCase(map, pos);
    Entity *toRemove;

    ywMapAdvenceWithEPos(map, pos, yeGetByStr(bullet, "dir"), bullet);
    if (!ywMapIsInside(map, pos)) {
      yeRemoveChild(bullets, bullet);
    }
  }
}

static void pizzaMaker(Entity *map, Entity *pizzas)
{
  Entity *pos;
  Entity *pizza;

  pizza = yeCreateArray(pizzas, NULL);
  yeCreateInt(2, pizza, "id");

  switch (yuiRand() & 3) {
  case 0:
    pos = ywPosCreateInts(0, yuiRand() % (ywMapH(map) - 1), pizza, "pos");
    ywPosCreateInts(-1, 0, pizza, "dir");
    break;
  case 1:
    pos = ywPosCreateInts(ywMapW(map) - 1, yuiRand() % (ywMapH(map) - 1),
			  pizza, "pos");
    ywPosCreateInts(1, 0, pizza, "dir");
    break;
  case 2:
    pos = ywPosCreateInts(yuiRand() % (ywMapW(map) - 1), 0, pizza, "pos");
    ywPosCreateInts(0, 1, pizza, "dir");
    break;
  case 3:
    pos = ywPosCreateInts(yuiRand() % (ywMapW(map) - 1), ywMapH(map) - 1,
			  pizza, "pos");
    ywPosCreateInts(0, -1, pizza, "dir");
    break;
  }
  ywMapPushElem(map, pizza, pos, "pizza");
}

static void clean(Entity *wid)
{
  yeRemoveChildByStr(wid, "entries");
}

static int lose(int score)
{
  Entity *str = yeCreateString("vapz:scenes.lose", NULL, NULL);
  Entity *txt = ygGet("vapz:scenes.lose.text");
  yeSetString(txt, "you lose, score: ");
  yeAddInt(txt, score);
  ywidNext(str);
  yeDestroy(str);
  return 1;
}

static int pizzaTurn(Entity *map, Entity *vkPos, Entity *textScreen)
{
  Entity *pizzas = yeGetByStr(map, "pizzas");

  if (!pizzas)
    pizzas = yeCreateArray(map, "pizzas");
  pizzaMaker(map, pizzas);
  ywMapSetOutBehavior(map, YMAP_OUT_WARP);
  YE_ARRAY_FOREACH(pizzas, pizza) {
    Entity *pos = yeGetByStrFast(pizza, "pos");
    Entity *bulletHit;

    bulletHit = ywMapGetNbrEntityAt(map, pos, 3);
    if (bulletHit) {
      removePizza(map, textScreen, pos, bulletHit, pizza);
    }
    ywMapAdvenceWithEPos(map, pos, yeGetByStrFast(pizza, "dir"), pizza);
    if (ywPosIsSameEnt(pos, vkPos, 0)) {
      return 1;
    }
    bulletHit = ywMapGetNbrEntityAt(map, pos, 3);
    if (bulletHit) {
      removePizza(map, textScreen, pos, bulletHit, pizza);
    }
  }
  return 0;
}

void *vapzAction(int nbArgs, void **args)
{
  Entity *gc = yeCreateArray(NULL, NULL);
  Entity *wid = args[0];
  YEvent *events = args[1];
  YEvent *eve = events;
  Entity *vk = yeGetByStrFast(wid, "viking");
  Entity *vkPos = yeGetByStrFast(vk, "pos");
  Entity *nextPos = ywPosCreateInts(0, 0, gc, NULL);
  Entity *bulletDir = NULL;
  Entity *bulletPos = ywPosCreateEnt(vkPos, 0, gc, NULL);
  Entity *map = ywCntGetEntry(wid, 0);
  Entity *textScreen = ywCntGetEntry(wid, 1);
  Entity *tl = yeGetByStrFast(wid, "turn-length");
  void *ret = tl ? (void *)ACTION : (void *)NOTHANDLE;
  int fire = 0;

  YEVE_FOREACH(eve, events) {

    if (ywidEveType(eve) != YKEY_DOWN)
      continue;
    switch (ywidEveKey(eve)) {
    case Y_ESC_KEY:
      ywidNext(yeCreateString("vapz:scenes.main", gc, NULL));
      clean(wid);
      yeDestroy(gc);
      return (void *)ACTION;
      break;
    case '\n':
      ret = (void *)ACTION;
      break;
    case Y_UP_KEY:
      ret = (void *)ACTION;
      ywPosSetY(nextPos, -1);
      break;
    case Y_DOWN_KEY:
      ret = (void *)ACTION;
      ywPosSetY(nextPos, 1);
      break;
    case Y_LEFT_KEY:
      ret = (void *)ACTION;
      ywPosSetX(nextPos, -1);
      break;
    case Y_RIGHT_KEY:
      ret = (void *)ACTION;
      ywPosSetX(nextPos, 1);
      break;
    case 'w':
    case 'z':
      fire = 1;
      bulletDir = ywPosCreateInts(0, -1, gc, NULL);
      ret = (void *)ACTION;
      break;
    case 'a':
    case 'q':
      fire = 1;
      bulletDir = ywPosCreateInts(-1, 0, gc, NULL);
      ret = (void *)ACTION;
      break;
    case 's':
      fire = 1;
      bulletDir = ywPosCreateInts(0, 1, gc, NULL);
      ret = (void *)ACTION;
      break;
    case 'd':
      fire = 1;
      bulletDir = ywPosCreateInts(1, 0, gc, NULL);
      ret = (void *)ACTION;
      break;

    default:
      break;
    }
  }
  if (bulletDir)
    pushBullet(map, bulletPos, bulletDir);
  ywMapSetOutBehavior(map, YMAP_OUT_BLOCK);
  Entity *isTouch = ywMapGetNbrEntityAt(map, vkPos, 2);
  if (isTouch) {
    lose(yeGetInt(yeGetByStrFast(map, "score")));
    clean(wid);
    goto exit;
  }
  ywMapAdvenceWithEPos(map, vkPos, nextPos,
		       ywMapGetNbrEntityAt(map, vkPos, 1));
  if (ret == (void *)ACTION) {
    bulletsTurn(map, textScreen);
    if (pizzaTurn(map, vkPos, textScreen)) {
      lose(yeGetInt(yeGetByStrFast(map, "score")));
      clean(wid);
      goto exit;
    }
    ywContenerUpdate(wid, map);
    ywContenerUpdate(wid, textScreen);
  }

 exit:
  yeDestroy(gc);
  return ret;
}


void *vapzInit(int nbArgs, void **args)
{
  Entity *main = args[0];
  Entity *cur_layer;
  Entity *resources = yeGetByStrFast(main, "resources");
  Entity *layers = yeCreateArray(main, "entries");
  Entity *viking = yeReCreateArray(main, "viking", NULL);
  Entity *vkPos = yeGetByStr(viking, "pos") ? :
    ywPosCreateInts(12, 12, viking, "pos");
  Entity *textScreen;

  yuiRandInit();
  cur_layer = ywMapCreateDefaultEntity(layers, NULL, resources,
				       0, 25, 25);
  ywMapPushNbr(cur_layer, 1, vkPos, NULL);
  yeCreateString("map", cur_layer, "<type>");
  yeCreateString("rgba: 255 255 255 255", cur_layer, "background");

  yeCreateString("stacking", main, "cnt-type");

  textScreen = yeCreateArray(layers, NULL);
  yeCreateString("text-screen", textScreen, "<type>");
  yeCreateString("score:", textScreen, "text");

  yeCreateInt(0, cur_layer, "score");
  yeCreateInt(150000, main, "turn-length");
  yeCreateInt(YRECALL_INIT, main, "recreate-logic");
  ywidCreateFunction("vapzAction", ygGetTccManager(), main, "action");
  void *ret = ywidNewWidget(main, "contener");
  return ret;
}