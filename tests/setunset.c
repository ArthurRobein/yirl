#include <glib.h>
#include <string.h>

#include "tests.h"
#include "entity.h"

void testsSetSimple(void)
{
  Entity *mainStruct = yeCreateStruct(NULL, NULL);
  Entity *test1 = yeCreateInt(NULL, 1, mainStruct);
  Entity *test2 = yeCreateFloat(NULL, 1, mainStruct);
  Entity *test3 = yeCreateString(NULL, "test", mainStruct);
  Entity *test4 = yeCreateFunction(NULL, "funcName", mainStruct);

  int testInt;
  double testDouble;
  const char *testStr;
  const char *testFunc;
  
  testInt = yeGetInt(test1);
  testDouble = yeGetFloat(test2);
  testStr = yeGetString(test3);
  testFunc = yeGetFunction(test4);

  g_assert(testInt == 1);
  g_assert(testDouble == 1);
  g_assert(!strcmp("test", testStr));
  g_assert(!strcmp("funcName", testFunc));

  yeSet(test1, 2);
  yeSet(test2, 2);
  yeSet(test3, "test2");
  yeSet(test4, "funcName2");

  testInt = yeGetInt(test1);
  testDouble = yeGetFloat(test2);
  testStr = yeGetString(test3);
  testFunc = yeGetFunction(test4);

  g_assert(testInt == 2);
  g_assert(testDouble == 2);
  g_assert(!strcmp("test2", testStr));
  g_assert(!strcmp("funcName2", testFunc));

  YE_DESTROY(mainStruct);
  g_assert(mainStruct == NULL);
}