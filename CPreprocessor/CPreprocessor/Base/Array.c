#include "Array.h"
#include <stdlib.h>
#include "config.h"
#include "StringEx.h"

Result Array_Reserve(Array* p, size_t nelements)
{
    Result result = RESULT_OK;
    if(nelements > p->capacity)
    {
        void** pnew = p->pItems;
        pnew = (void**)realloc(pnew, nelements * sizeof(void*));
        if(pnew)
        {
            p->pItems = pnew;
            p->capacity = nelements;
        }
        else
        {
            result = RESULT_OUT_OF_MEM;
        }
    }
    return result;
}


Result Array_Grow(Array* p, size_t nelements)
{
    Result result = RESULT_OK;
    if(nelements > p->capacity)
    {
        size_t new_nelements = p->capacity + p->capacity / 2;
        if(new_nelements < nelements)
        {
            new_nelements = nelements;
        }
        result = Array_Reserve(p, new_nelements);
    }
    return result;
}

void Array_Pop(Array* p, void(*pfDestroyData)(void*))
{
    if(p->size > 0)
    {
        void* pItem = p->pItems[p->size - 1];
        p->pItems[p->size - 1] = NULL;
        p->size--;
        if(pfDestroyData)
        {
            pfDestroyData(pItem);
        }
    }
    else
    {
        ASSERT(false);
    }
}

void* Array_Top(Array* p)
{
    ASSERT(p->size > 0);
    return p->pItems[p->size - 1];
}

Result Array_Push(Array* p, void* pItem)
{
    Result result = Array_Grow(p, p->size + 1);
    if(result == RESULT_OK)
    {
        p->pItems[p->size] = pItem;
        p->size++;
    }
    return result;
}

void Array_Clear(Array* p, void(*pfDestroyData)(void*))
{
    for(size_t i = 0; i < p->size; i++)
    {
        if(pfDestroyData)
            pfDestroyData(p->pItems[i]);
    }
    free(p->pItems);
    p->pItems = NULL;
    p->size = 0;
    p->capacity = 0;
}


Result Array_Init(Array* p)
{
    p->capacity = 0;
    p->size = 0;
    p->pItems = NULL;
    return RESULT_OK;
}

Result Array_Create(Array** pp)
{
    Result result = RESULT_OUT_OF_MEM;
    Array* p = (Array*)malloc(sizeof(Array));
    if(p)
    {
        result = Array_Init(p);
        if(result == RESULT_OK)
        {
            *pp = p;
        }
        else
        {
            free(p);
        }
    }
    return result;
}

void Array_Destroy(Array* st, void (*pfDestroyData)(void*))
{
    Array_Clear(st, pfDestroyData);
}

void Array_Swap(Array* p1, Array* p2)
{
    size_t c = p1->capacity;
    size_t s = p1->size;
    void** pp = p1->pItems;
    p1->capacity = p2->capacity;
    p1->size = p2->size;
    p1->pItems = p2->pItems;
    p2->capacity = c;
    p2->size = s;
    p2->pItems = pp;
}

void Array_Delete(Array* st, void(*pfDestroyData)(void*))
{
    Array_Destroy(st, pfDestroyData);
    free(st);
}

/////////


Result ArrayInt_Reserve(ArrayInt* p, size_t nelements)
{
    Result result = RESULT_OK;
    if(nelements > p->capacity)
    {
        int* pnew = p->pItems;
        pnew = (int*)realloc(pnew, nelements * sizeof(int));
        if(pnew)
        {
            p->pItems = pnew;
            p->capacity = nelements;
        }
        else
        {
            result = RESULT_OUT_OF_MEM;
        }
    }
    return result;
}


Result ArrayInt_Grow(ArrayInt* p, size_t nelements)
{
    Result result = RESULT_OK;
    if(nelements > p->capacity)
    {
        size_t new_nelements = p->capacity + p->capacity / 2;
        if(new_nelements < nelements)
        {
            new_nelements = nelements;
        }
        result = ArrayInt_Reserve(p, new_nelements);
    }
    return result;
}

bool ArrayInt_IsEmpty(ArrayInt* p)
{
    return p->size == 0;
}

int ArrayInt_Pop(ArrayInt* p)
{
    int ival = 0;
    if(p->size > 0)
    {
        ival = p->pItems[p->size - 1];
        p->pItems[p->size - 1] = 0;
        p->size--;
    }
    else
    {
        ASSERT(false);
    }
    return ival;
}

int ArrayInt_Top(ArrayInt* p)
{
    ASSERT(p->size > 0);
    return p->pItems[p->size - 1];
}

Result ArrayInt_Push(ArrayInt* p, int ival)
{
    Result result = ArrayInt_Grow(p, p->size + 1);
    if(result == RESULT_OK)
    {
        p->pItems[p->size] = ival;
        p->size++;
    }
    return result;
}

void ArrayInt_Clear(ArrayInt* p)
{
    free(p->pItems);
    p->pItems = NULL;
    p->size = 0;
    p->capacity = 0;
}

Result ArrayInt_Init(ArrayInt* p)
{
    p->capacity = 0;
    p->size = 0;
    p->pItems = NULL;
    return RESULT_OK;
}

Result ArrayInt_Create(ArrayInt** pp)
{
    Result result = RESULT_OUT_OF_MEM;
    ArrayInt* p = (ArrayInt*)malloc(sizeof(ArrayInt));
    if(p)
    {
        result = ArrayInt_Init(p);
        if(result == RESULT_OK)
        {
            *pp = p;
        }
        else
        {
            free(p);
        }
    }
    return result;
}

void ArrayInt_Destroy(ArrayInt* st)
{
    ArrayInt_Clear(st);
}

void ArrayInt_Delete(ArrayInt* st)
{
    ArrayInt_Destroy(st);
    free(st);
}



Result StrArray_Reserve(StrArray* p, size_t nelements)
{
    return Array_Reserve((Array*)p, nelements);
}

Result StrArray_Push(StrArray* p, const char* pItem)
{
    String s;
    Result result = String_Init(&s, pItem);
    if(result == RESULT_OK)
    {
        result = Array_Push((Array*)p, s);
        if(result != RESULT_OK)
        {
            String_Destroy(&s);
        }
    }
    return result;
}

static void Array_DeleteStrVoid(void* p)
{
    String_Destroy((String*)(&p));
}

void StrArray_Clear(StrArray* p)
{
    Array_Clear((Array*)p, Array_DeleteStrVoid);
}

Result StrArray_Init(StrArray* p)
{
    return Array_Init((Array*)p);
}

void StrArray_Destroy(StrArray* p)
{
    Array_Destroy((Array*)p, &Array_DeleteStrVoid);
}

