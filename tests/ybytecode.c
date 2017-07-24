#include <glib.h>

#include "tests.h"
#include "ybytecode.h"
#include "ybytecode-script.h"
#include "condition.h"

void ysciptAdd(void)
{
  yeInitMem();
  Entity *args = yeCreateArrayExt(NULL, NULL,
				  YBLOCK_ARRAY_NOINIT |
				  YBLOCK_ARRAY_NOMIDFREE);

  int64_t test0[] = {0,
		     'i', 4, //stack 4
		     'i', 3, //stack 3
		     //stack 0 will be use to store resulte
		     'i', 0,
		     // add stack 0 to stack 1
		     // and store resulte in stack 2
		     '+', 0, 1, 2,
		     'E', 2 // return 2nd elem
  };

  for (int i = 0; i < 5000000; ++i) {
    yeDestroy(ybytecode_exec(args, test0));
  }
  yeDestroy(args);
  yeEnd();
}

void yscriptBenchLoop(void)
{
  yeInitMem();
  Entity *args = yeCreateArrayExt(NULL, NULL,
				  YBLOCK_ARRAY_NOINIT |
				  YBLOCK_ARRAY_NOMIDFREE);
  Entity *tmp;

  // this is a simple for(int i = 0; i < 500 000; ++i)
  int64_t test1[] = {0,
		     'i', 0, //stack[0] = 2 // 2
		     'j', 7, // jmp to loop comparaison // 4
		     YB_INCR, 0, // 6
		     // if stack 0 is iferior to stack[2], goto 5
		     YB_INF_COMP_NBR, 0, 50000000, 5,
		     'i', 2, // stack[1] = 2
		     '*', 0, 1, 0, // mult stack[0] by 2
		     'E', 0 // return 2nd elem
  };

  tmp = ybytecode_exec(args, test1);
  g_assert(yeGetIntDirect(tmp) == (50000000 * 2));
  yeDestroy(tmp);
  yeDestroy(args);
  yeEnd();
}

void yscriptLoop(void)
{
  yeInitMem();
  Entity *args = yeCreateArrayExt(NULL, NULL,
				  YBLOCK_ARRAY_NOINIT |
				  YBLOCK_ARRAY_NOMIDFREE);
  Entity *tmp;

  // this is a simple for(int i = 0; i < 500 000; ++i)
  int64_t test1[] = {0,
		     'i', 0, //stack 1 - 2
		     'i', 1, //stack 0 - 4
		     'i', 5000, // 6
		     'j', 13, // jmp to loop - 8
		     '+', 0, 1, 0,
		     // if stack 0 is iferior to stack 2, goto 9
		     '<', 0, 2, 9,
		     'i', 2, // stack 2 in 3
		     '*', 0, 3, 0, // mult 0 by 2
		     'E', 0 // return 2nd elem
  };

  tmp = ybytecode_exec(args, test1);
  g_assert(yeGetIntDirect(tmp) == (5000 * 2));
  yeDestroy(tmp);
  yeDestroy(args);
  yeEnd();
}

void ybytecodeScript(void)
{
  void *sm = NULL;
  int64_t test1[] = {0,
		     'i', 0, //stack 1 - 2
		     'i', 1, //stack 0 - 4
		     'i', 50000000, // 6
		     '+', 1, 2, 0,
		     'E', 0
  };
  int64_t testWithArgs[] = {0,
			    YB_BRUTAL_CAST, 0, YINT,
			    YB_BRUTAL_CAST, 1, YINT,
			    '+', 0, 1, 0,
			    'E', 0
  };
  int64_t testWithEntArgs[] = {0,
			       'i', 0,
			       '+', 0, 1, 2,
			       'E', 2
  };
  Entity *gc, *i0, *i1;
  yeInitMem();
  gc = yeCreateArray(NULL, NULL);
  i0 = yeCreateInt(7, gc, NULL);
  i1 = yeCreateInt(8, gc, NULL);

  sm = ysYBytecodeManager();
  g_assert(sm);

  g_assert(!ysRegistreFunc(sm, "add", test1));
  g_assert(!ysRegistreFunc(sm, "argsadd", testWithArgs));
  g_assert(!ysRegistreFunc(sm, "entargsadd", testWithEntArgs));
  g_assert((uint64_t)ysCall(sm, "add") == 50000001);
  g_assert((uint64_t)ysCall(sm, "argsadd", 50000000, 1) == 50000001);
  g_assert((uint64_t)ysCall(sm, "add") == 50000001);
  g_assert((uint64_t)ysCall(sm, "argsadd", 1, 2) == 3);
  g_assert((uint64_t)ysCall(sm, "entargsadd", i0, i1) == (8 + 7));
  g_assert((uint64_t)ysCall(sm, "entargsadd", i0, i1) == (8 + 7));

  g_assert(!ysYBytecodeEnd());
  yeDestroy(gc);
  yeEnd();
}


void ybytecodeLoopCallFunction(void)
{
  int64_t test1[] = {0,
		     's', (uint64_t)"testFunc", //stack "testFunc"
		     'F', 0, 1, // function with name at stack 0
		     '+', 0, 1, 0, // add stack 0 to stack 0
		     // store result in stack 0
		     'e', // end function
		     'i', 0,
		     'i', 1,
		     'i', 5000000,
		     'j', 24,
		     'c', 2, 1, 2, 3, // 2 arguments, call stack 1,
				      //with arguments at stack 2 and 3
		     '<', 2, 4, 19,
		     'E', 2 // return 2nd elem, so 15 , 52
  };
  void *sm = NULL;

  yeInitMem();
  sm = ysYBytecodeManager();
  g_assert(sm);

  g_assert(!ysRegistreFunc(sm, "test", test1));
  g_assert((uint64_t)ysCall(sm, "test") == 5000000);

  g_assert(!ysYBytecodeEnd());
  yeEnd();
}

void ybytecodeAddFunction(void)
{
  int64_t test1[] = {0,
		     's', (uint64_t)"testFunc", //stack "testFunc"
		     'F', 0, 1, // function with name at stack 0
		     '+', 0, 1, 0, // add stack 0 to stack 0
		     // store result in stack 0
		     'e', // end function
		     'i', 15,
		     'i', 52,
		     'c', 2, 1, 2, 3, // 2 arguments, call stack 1,
				      //with arguments at stack 2 and 3
		     'E', 2 // return 2nd elem, so 15 , 52
  };
  void *sm = NULL;

  yeInitMem();
  sm = ysYBytecodeManager();
  g_assert(sm);

  g_assert(!ysRegistreFunc(sm, "test", test1));
  for (int i = 0; i < 5000000; ++i)
    g_assert((uint64_t)ysCall(sm, "test") == (52 + 15));

  g_assert(!ysYBytecodeEnd());
  yeEnd();
}

void ybytecodeConditions(void)
{
  void *sm;
  Entity *condition;

  yeInitMem();
  sm = ysYBytecodeManager();
  g_assert(sm);
  condition = yeCreateArray(NULL, NULL);
  yeCreateString(">", condition, NULL);
  yeCreateInt(1, condition, NULL);
  yeCreateInt(0, condition, NULL);
  g_assert(yeCheckCondition(condition));
  g_assert(!ysYBytecodeEnd());
  yeDestroy(condition);
  yeEnd();
}

