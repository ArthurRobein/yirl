/*
**Copyright (C) 2013 Matthias Gatto
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

/*
 * This file contain the entity API.
 * All there's functions are made to be use in lua too
 */

#ifndef	ENTITY_H
#define	ENTITY_H

#include <stdio.h>

#define NONNULL(arg) __attribute__ ((nonnull (arg)))

#ifdef __linux__
# ifdef LIB
# define WEAK __attribute__((weak))
# else
# define WEAK //nothing :)
	#endif
#else
# define WEAK //nothing :)
#endif

/**
 * Here some macros to mutualise the code of entity
 */
#define RETURN_ERROR_BAD_TYPE(function, entity, returnValue) DPRINT_ERR("%s: bad entity, this entity is of type %s\n", (function), yeTypeToString(yeType(entity))); return (returnValue)



#ifdef __cplusplus
extern "C"
{
#endif
  /*Les differents type d'entite, chacune est definie par une structure apres*/
  typedef enum
    {
      BAD_TYPE = -1,
      STRUCT = 0,
      YINT,
      YFLOAT,
      YSTRING,
      ARRAY,
      FUNCTION, //here will the fun begin
      STATIC
    } EntityType;
  
#define	NBR_ENTITYTYPE	7

#define	YE_TO_ENTITY(X) ((Entity *)X)
#define	YE_TO_C_ENTITY(X) ((const Entity *)X)
#define	YE_TO_INT(X) ((IntEntity *)X)
#define	YE_TO_C_INT(X) ((const IntEntity *)X)
#define	YE_TO_FLOAT(X) ((FloatEntity *)X)
#define	YE_TO_C_FLOAT(X) ((const FloatEntity *)X)
#define	YE_TO_STRING(X) ((StringEntity *)X)
#define	YE_TO_C_STRING(X) ((const StringEntity *)X)
#define	YE_TO_STRUCT(X) ((StructEntity *)X)
#define	YE_TO_C_STRUCT(X) ((const StructEntity *)X)
#define	YE_TO_ARRAY(X) ((ArrayEntity *)X)
#define	YE_TO_C_ARRAY(X) ((const ArrayEntity *)X)
#define	YE_TO_FUNC(X) ((FunctionEntity *)X)
#define	YE_TO_C_FUNC(X) ((const FunctionEntity *)X)

  struct	Entity;

  /**
   * @father is the entity contening this one (a struct or an array)
   */
#define	ENTITY_HEADER				\
  EntityType	type;				\
  const char	*name;				\
  struct Entity	*father;			\
  unsigned int refCount;

  /* macro for perf purpose */
#define YE_INCR_REF(entity) do {		\
    entity->refCount += 1;			\
  } while (0)
  
#define YE_DECR_REF(entity) do {		\
    entity->refCount -= 1;		       	\
  } while (0)

#define YE_DESTROY_ENTITY(entity, type) do {	\
    YE_DECR_REF(entity);			\
    if (entity->refCount <= 0)			\
      free(((type *)entity));			\
  } while (0);
  
#define YE_ALLOC_ENTITY(ret, type) do {		\
    ret = malloc(sizeof(type));			\
    ret->refCount = 1;				\
  } while (0);

  typedef struct Entity
  {
    ENTITY_HEADER

  } Entity;

  typedef	struct
  {
    ENTITY_HEADER

    unsigned int len;
    Entity	**values;
    const char	*structName;
    // prototype is a void * because EntityStructPrototype can't contain a class(i think)
    const void	*prototype;
  } StructEntity;

  typedef	struct
  {
    ENTITY_HEADER

    unsigned int len;
    Entity	**values;
    EntityType	contentType;
  } ArrayEntity;

  typedef	struct
  {
    ENTITY_HEADER

    int		value;
  } IntEntity;

  typedef	struct
  {
    ENTITY_HEADER

    double	value;
  } FloatEntity;

  typedef	struct
  {
    ENTITY_HEADER

    unsigned int len;
    char	*value;
  } StringEntity;

  typedef	struct
  {
    ENTITY_HEADER
    
    // the name of the function to call
    char	*value;
    unsigned int nArgs;
    EntityType	*args; // including return value
  } FunctionEntity;

  typedef	struct
  {
    ENTITY_HEADER

    Entity	*value;
  } StaticEntity;

  /**
   * @param str   the type name
   * @return      the corresponding type, -1 if type not found
   */
  EntityType yeStringToType(const char *str) WEAK;

  /**
   * @param type
   * @return the corresponding string of the type
   */
  const char *yeTypeToString(int type) WEAK;

  /**
   * @return the entity at the position of the index or NULL
   */
  Entity *yeGetIdx(Entity *entity, unsigned int index) WEAK;
    
  /**
   * @return the entity at the position wich has the given name or NULL
   */
  Entity *yeGetStr(Entity *entity, const char *name) WEAK;

#ifdef __cplusplus
  extern "C++"
  {
    /**
     * @return the entity at the position of the index or NULL
     */
    Entity *yeGet(Entity *entity, unsigned int index) WEAK;
    Entity *yeGet(Entity *entity, const char *name) WEAK;
  }
#else
#define yeGet(ENTITY, INDEX) _Generic((INDEX),			\
				      unsigned int: yeGetIdx,	\
				      int: yeGetIdx,		\
				      const char *: yeGetStr,	\
				      char *: yeGetStr) (ENTITY, INDEX)

#endif


  /**
   * Like yeGetStr but dosn't work with sytaxe like this (entity1.entity11)
   */
  Entity *yeGetIdxFast(Entity *entity, const char *name) WEAK;

  
  /**
   * change the capacity than the array can store and init it to 0, "" or NULL
   */
  Entity	*yeExpandArray(Entity *entity, unsigned int size, EntityType arrayType) WEAK;
  void	yePushBack(Entity *array, Entity *toPush) WEAK;
  Entity *yePopBack(Entity *array) WEAK;
  Entity *yeArrayRemove(Entity *array, Entity *toRemove);
  
  /**
   * function who alloc an entity and set it to  0, "" or NULL
   */
  Entity *yeCreate(EntityType type, Entity *father, EntityType typeArray) WEAK;

   /** Ensemble de fonction pour cree et detruire chaque type d'entite.
    * Cannot initialise a structure in thers functions because the data are to complex
    */
  Entity *yeCreateStruct(Entity *father) WEAK;
  Entity *yeCreateInt(int value, Entity *father) WEAK;
  Entity *yeCreateFloat(double value, Entity *father) WEAK;
  Entity *yeCreateString(const char *string, Entity *father) WEAK;
  Entity *yeCreateFunction(const char *string, Entity *father) WEAK;
  Entity *yeCreateArray(EntityType contentType, Entity *father) WEAK;
  Entity *yeCreateStatic(Entity *value, Entity *father) WEAK;

  void yeDestroy(Entity *entity) WEAK;
  void yeDestroyStruct(Entity *entity) WEAK;
  void yeDestroyInt(Entity *entity) WEAK;
  void yeDestroyFloat(Entity *entity) WEAK;
  void yeDestroyString(Entity *entity) WEAK;
  void yeDestroyFunction(Entity *entity) WEAK;
  void yeDestroyRef(Entity *entity) WEAK;
  void yeDestroyStatic(Entity *entity) WEAK;
  void yeDestroyArray(Entity *entity) WEAK;

  /**
   * Set a referenced Entity to a Entity after checking the type of the referencing entity
   * @param entity	the referencing Entity
   * @param other	the referenced Entity
   */
  int	yeSetInt(Entity *entity, int value) WEAK;
  int	yeSetFloat(Entity *entity, double value) WEAK;
  void	yeSetString(Entity *entity, const char *value) WEAK;

  /**
   * @brief set a function entity to NULL
   */
  void	yeUnsetFunction(Entity *entity) WEAK;

  
  const char	*yeSetFunction(Entity *entity, const char *value) WEAK;
  
  /**
   * @brief set the informa
tion about the arguments of a function
   * @param nArgs number of arguments
   * @param args the argument list, sould be alocate first.
   */
  void	yeSetFunctionArgs(Entity *entity, unsigned int nArgs, EntityType *args) WEAK;
  

  Entity *yeInit(Entity *entity, const char *name, EntityType type, Entity *father)  WEAK;
  /**
   * set to a value to the index if the entity is an array or a generic array
   */
  
  int	yeSetIntAt(Entity *entity, unsigned int index, int value) WEAK;
  int	yeSetFloatAt(Entity *entity, unsigned int index, double value) WEAK;
  void	yeSetStringAt(Entity *entity, unsigned int index, const char *value) WEAK;
  int	yeSetIntAtStrIdx(Entity *entity, const char *index, int value) WEAK;
  int	yeSetFloatAtStrIdx(Entity *entity, const char *index, double value) WEAK;
  void	yeSetStringAtStrIdx(Entity *entity, const char *index, const char *value) WEAK;

  
#ifdef __cplusplus
extern "C++"
{
  void yeSetAt(Entity *entity, unsigned int index, const char *value) WEAK;
  void yeSetAt(Entity *entity, unsigned int index, int value) WEAK;
  void yeSetAt(Entity *entity, unsigned int index, Entity *value) WEAK;
  void yeSetAt(Entity *entity, unsigned int index, float value) WEAK;
  void yeSetAt(Entity *entity, const char *index, const char *value) WEAK;
  void yeSetAt(Entity *entity, const char *index, int value) WEAK;
  void yeSetAt(Entity *entity, const char *index, Entity *value) WEAK;
  void yeSetAt(Entity *entity, const char *index, float value) WEAK;
}
#else
#define yeSetAt(ENTITY, INDEX, VALUE) _Generic((VALUE),		\
					     int: yeSetIntAt,		\
					     float: yeSetFloatAt,		\
					     const char *: yeSetStringAt, \
					     char *: yeSetStringAt)(ENTITY, INDEX, VALUE)

#endif
  
  
  unsigned int yeLen(Entity *entity) WEAK;;
  /*Geter pour les entites 
    certain sont particuliere comme get Father qui permet de recuperer entite pere d'une entite (entite pere est celle qui a appeler/cree l'entite en parametre*/
  int	yeGetInt(Entity *entity) WEAK;
  double yeGetFloat(Entity *entity) WEAK;
  const char *yeGetString(Entity *entity) WEAK;
  const char *yeName(const Entity *entity) WEAK;
  Entity *yeFather(Entity *entity) WEAK;
  const char	*yeGetFunction(Entity *entity) WEAK;
  int	yeFunctionNumberArgs(const Entity *entity) WEAK;
  EntityType yeGetFunctionArg(const Entity *entity, int i) WEAK;
  EntityType yeContentType(const Entity *entity) WEAK;

  /* as yeName but return a printable result */
  const char *yePrintableName(const Entity *entity);
  const char *yePrintableStructName(const Entity *entity);

  /**
   * @param entity
   * @return if entity is not null return the type, -1 otherwise
   */
  int	yeType(const Entity *entity);

  /**
   * @param buf the buffer where the string is store
   * @param sizeBuf the size of buf
   * @return the number of caracter write into buf, -1 if not enough place
   */
  int yeToString(Entity *entity, char *buf, int sizeBuf);

  /**
   * Check if Entity are the same type and if they are not NULL and copy the values from src to dest.
   * @param src	The source Entity from where the values will be copied from.
   * @param dest	The destination Entity to where the values will be pasted.
   * @return	NULL if entities do not have the same type or are NULL, dest Entity otherwise.
   */
  Entity*		yeCopy(Entity* src, Entity* dest);

  /**
   * Copy the data from src Entity to dest Entity.
   * Get the values and copy each Entity in the StructEntity.
   * @param src	Source Entity from where the data will be copy
   * @param dest	Destination Entity where the data will be past
   * @return	destination Entity if src AND dest or not null, NULL otherwise
   */
  StructEntity*		yeCopyStruct(StructEntity* src, StructEntity* dest);


#ifdef __cplusplus
}
#endif


#endif
