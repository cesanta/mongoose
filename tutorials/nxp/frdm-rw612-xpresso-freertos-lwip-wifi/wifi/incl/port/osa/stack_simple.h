/*
 *  Copyright (c) 2023, Michael Becker (michael.f.becker@gmail.com)
 *  Copyright 2023-2024 NXP
 */
/*!\file stack_simple.h
 *\brief This file is part of the FreeRTOS Add-ons project.
 */

/*
 *  Source Code:
 *  https://github.com/michaelbecker/freertos-addons
 *
 *  Project Page:
 *  http://michaelbecker.github.io/freertos-addons/
 *
 *  On-line Documentation:
 *  http://michaelbecker.github.io/freertos-addons/docs/html/index.html
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so,subject to the
 *  following conditions:
 *
 *  + The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *  + Credit is appreciated, but not required, if you find this project
 *    useful enough to include in your application, product, device, etc.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef STACK_H_
#define STACK_H_

#include "slist.h"

/**
 *  The stack structure, we leverage low overhead singly linked lists here.
 */
typedef struct Stack_t_
{
    /**
     *  How many items are in the stack.
     */
    int Count;

    /**
     *  The head of the stack.
     */
    SlNode_t Head;

} Stack_t;

/**
 *  Initialize a Stack structure you provide.
 *
 *  @param Stack Pointer to your stack structure.
 */
void InitStack(Stack_t *Stack);

/**
 *  Push an item onto the stack.
 *
 *  Note that you have to have embedded an SListNode inside your data
 *  structure.
 *
 *  @param Stack Pointer to your stack structure.
 *  @param Node The SListNode you want on the stack.
 */
void PushOnStack(Stack_t *Stack, SlNode_t *Node);

/**
 *  Pop an item off the stack.
 *
 *  Note that you have to have embedded an SListNode inside your data
 *  structure.
 *
 *  @param Stack Pointer to your stack structure.
 *  @return An SListNode from the stack, or NULL if it's empty.
 */
SlNode_t *PopOffStack(Stack_t *Stack);

/**
 *  @return True if the stack is empty, false otherwise.
 */
#define IsStackEmpty(_stack) ((_stack)->Count == 0)

#endif
