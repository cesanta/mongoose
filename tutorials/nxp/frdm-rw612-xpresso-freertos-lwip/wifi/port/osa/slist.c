/****************************************************************************
 *
 *  Copyright (c) 2023, Michael Becker (michael.f.becker@gmail.com)
 *
 *  This file is part of the FreeRTOS Add-ons project.
 *
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
 ***************************************************************************/
/*
 *  Copyright 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdlib.h>
#include "osa/slist.h"

void SlAddNodeToTail(SlNode_t *Head, SlNode_t *Node)
{
    /********************/
    SlNode_t *Current;
    /********************/

    if (Head == NULL)
        return;

    if (Node == NULL)
        return;

    Current = Head;

    while (Current->Next != NULL)
    {
        Current = Current->Next;
    }

    Current->Next = Node;
    Node->Next    = NULL;
}

void SlInsertNodeAfter(SlNode_t *Marker, SlNode_t *Node)
{
    /********************/
    SlNode_t *Temp;
    /********************/

    if (Marker == NULL)
        return;

    if (Node == NULL)
        return;

    Temp         = Marker->Next;
    Marker->Next = Node;
    Node->Next   = Temp;
}

void SlInsertNodeBefore(SlNode_t *Head, SlNode_t *Marker, SlNode_t *Node)
{
    /********************/
    SlNode_t *Current;
    SlNode_t *Prior;
    int Found = 0;
    /********************/

    if (Marker == NULL)
        return;

    if (Node == NULL)
        return;

    if (Head == NULL)
        return;

    Current = Head->Next;
    Prior   = Head;

    while (Current != NULL)
    {
        if (Current == Marker)
        {
            Found = 1;
            break;
        }

        Prior   = Current;
        Current = Current->Next;
    }

    if (Found)
    {
        Prior->Next = Node;
        Node->Next  = Current;
    }
}

void SlRemoveNode(SlNode_t *Head, SlNode_t *Node)
{
    /********************/
    SlNode_t *Current;
    SlNode_t *Prior;
    int Found = 0;
    /********************/

    if (Head == NULL)
        return;

    if (Node == NULL)
        return;

    Current = Head->Next;
    Prior   = Head;

    while (Current != NULL)
    {
        if (Current == Node)
        {
            Found = 1;
            break;
        }

        Prior   = Current;
        Current = Current->Next;
    }

    if (Found)
    {
        Prior->Next = Current->Next;
    }
}

SlNode_t *SlRemoveNodeFromHead(SlNode_t *Head)
{
    /********************/
    SlNode_t *Node;
    /********************/

    if (Head == NULL)
        return NULL;

    Node = Head->Next;

    if (Node != NULL)
    {
        Head->Next = Node->Next;
    }

    return Node;
}

SlNode_t *SlRemoveNodeFromTail(SlNode_t *Head)
{
    /********************/
    SlNode_t *Current;
    SlNode_t *Prior;
    /********************/

    if (Head == NULL)
        return NULL;

    Current = Head->Next;
    Prior   = Head;

    if (SlIsListEmpty(Head))
    {
        return NULL;
    }

    while (Current->Next != NULL)
    {
        Prior   = Current;
        Current = Current->Next;
    }

    Prior->Next = NULL;

    return Current;
}
