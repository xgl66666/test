/*
 *  @file list.h
 *
 *  @brief
 *   Implements a generic list.
 *
 *  @author Thomas Bourlard
 *  $Id:$
 */

#ifndef	_LIST_H_
#define	_LIST_H_

#include <stddef.h>

/*
 * a elt_t variable can be any pointer.
 * NULL: invalid element
 */
typedef void* elt_t;
/*
 * index is a reference to an element.
 * invalid index = lp->size
 */
typedef unsigned int index_t;
#define LIST_INDEX_INVALID ((index_t)0xffffffff)

struct list_s
{
    const size_t size;
    elt_t array[0];
};
typedef struct list_s * list_t;

/*
 * List definitions.
 */

#define LIST_STRUCT(name, lsize)           \
struct name                                \
{                                          \
    size_t size;                           \
    elt_t array[lsize];                    \
}

#define LIST_INITIALIZER(lsize)            \
{                                          \
    .size = (lsize),                       \
    .array[0 ... ((lsize)-1)] = NULL,      \
}

/*
 * Create/Define a list.
 */
#define LIST(list, size)                   \
    LIST_STRUCT(list##_s, size) __##list = \
    LIST_INITIALIZER(size);                \
    list_t list = (list_t) &__##list

/*
 * This function add the element <e> and return its reference/index <i>,
 * only if the list is not full.
 *
 *  @returns
 *    - a reference to the element, or
 *    - LIST_INDEX_INVALID in case there is no more space in the list.
 *
 */

/*
for RAM2RAM Session Handling Test
*/
#define DAL_PREFIX			0x56780000
#define DAL_PREFIX_UNMASK	0x0000FFFF
#define DAL_PREFIX_MASK		0xFFFF0000

static inline index_t ListAdd(list_t list, elt_t e)
{
	int i;
	struct list_s * lp = list;

	if (NULL != e)
	{
		for (i = 0; (size_t)i < lp->size; i++)
		{
			if (NULL == lp->array[i])
			{
				lp->array[i] = e;
				break;
			}
		}
	}
	if ((size_t)i == lp->size)
	{
		return LIST_INDEX_INVALID;
	}

	return (i | DAL_PREFIX); //with prefix for DAL test suite
}

/*
 * This function removes the element referenced by <i>,
 * only if the element exists.
 *
 *  @returns
 *    - the removed element, or
 *    - NULL in case the element was not found.
 *
 */
static inline elt_t ListRemove(list_t list, index_t i)
{
	i = i & DAL_PREFIX_UNMASK;
	struct list_s * lp = list;
	elt_t e = NULL;

	if (i < lp->size)
	{
		e = lp->array[i];
		lp->array[i] = NULL;
	}
	return e;
}

/*
 * This function returns the element referenced by <i>,
 * only if the element exists.
 *
 *  @returns
 *    - the element, or
 *    - NULL in case there is no element.
 *
 */
static inline elt_t ListGet(list_t list, index_t i)
{
	i = i & DAL_PREFIX_UNMASK;
	struct list_s * lp = list;

	if (i >= lp->size)
	{
		return NULL;
	}
	else
	{
		return lp->array[i];
	}
}

#endif	/* !_LIST_H_ */

