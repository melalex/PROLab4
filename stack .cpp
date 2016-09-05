//
// Created by Александр Сергеевич on 14.05.2016.
//

#include "stack.h"

int stack[MAX_LENGTH];

void push(int elem)
{
    stack[length++] = elem;
}

void* pop()
{
    return stack[--length];
}
