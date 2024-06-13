/*
 * Copyright 2018-2019, 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "fsl_component_generic_list.h"

#if defined(OSA_USED)
#include "fsl_os_abstraction.h"
#if (defined(USE_RTOS) && (USE_RTOS > 0U))
#define LIST_ENTER_CRITICAL() \
    OSA_SR_ALLOC();           \
    OSA_ENTER_CRITICAL()
#define LIST_EXIT_CRITICAL() OSA_EXIT_CRITICAL()
#else
#define LIST_ENTER_CRITICAL() uint32_t regPrimask = DisableGlobalIRQ();
#define LIST_EXIT_CRITICAL()  EnableGlobalIRQ(regPrimask);
#endif
#else
#define LIST_ENTER_CRITICAL() uint32_t regPrimask = DisableGlobalIRQ();
#define LIST_EXIT_CRITICAL()  EnableGlobalIRQ(regPrimask);
#endif

static list_status_t LIST_Error_Check(list_handle_t list, list_element_handle_t newElement)
{
    list_status_t listStatus = kLIST_Ok;
#if (defined(GENERIC_LIST_DUPLICATED_CHECKING) && (GENERIC_LIST_DUPLICATED_CHECKING > 0U))
    list_element_handle_t element = list->head;
#endif
    if ((list->max != 0U) && (list->max == list->size))
    {
        listStatus = kLIST_Full; /*List is full*/
    }
#if (defined(GENERIC_LIST_DUPLICATED_CHECKING) && (GENERIC_LIST_DUPLICATED_CHECKING > 0U))
    else
    {
        while (element != NULL) /*Scan list*/
        {
            /* Determine if element is duplicated */
            if (element == newElement)
            {
                listStatus = kLIST_DuplicateError;
                break;
            }
            element = element->next;
        }
    }
#endif
    return listStatus;
}

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
/*! *********************************************************************************
 * \brief     Initializes the list descriptor.
 *
 * \param[in] list - LIST_ handle to init.
 *            max - Maximum number of elements in list. 0 for unlimited.
 *
 * \return void.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
void LIST_Init(list_handle_t list, uint32_t max)
{
    list->head = NULL;
    list->tail = NULL;
    list->max  = max;
    list->size = 0;
}

/*! *********************************************************************************
 * \brief     Gets the list that contains the given element.
 *
 * \param[in] element - Handle of the element.
 *
 * \return NULL if element is orphan.
 *         Handle of the list the element is inserted into.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_handle_t LIST_GetList(list_element_handle_t listElement)
{
    return listElement->list;
}

/*! *********************************************************************************
 * \brief     Links element to the tail of the list.
 *
 * \param[in] list - ID of list to insert into.
 *            element - element to add
 *
 * \return kLIST_Full if list is full.
 *         kLIST_Ok if insertion was successful.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_status_t LIST_AddTail(list_handle_t list, list_element_handle_t listElement)
{
    LIST_ENTER_CRITICAL();
    list_status_t listStatus = kLIST_Ok;

    listStatus = LIST_Error_Check(list, listElement);
    if (listStatus == kLIST_Ok) /* Avoiding list status error */
    {
        if (list->size == 0U)
        {
            list->head = listElement;
        }
        else
        {
            list->tail->next = listElement;
        }
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#else
        listElement->prev = list->tail;
#endif
        listElement->list = list;
        listElement->next = NULL;
        list->tail        = listElement;
        list->size++;
    }

    LIST_EXIT_CRITICAL();
    return listStatus;
}

/*! *********************************************************************************
 * \brief     Links element to the head of the list.
 *
 * \param[in] list - ID of list to insert into.
 *            element - element to add
 *
 * \return kLIST_Full if list is full.
 *         kLIST_Ok if insertion was successful.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_status_t LIST_AddHead(list_handle_t list, list_element_handle_t listElement)
{
    LIST_ENTER_CRITICAL();
    list_status_t listStatus = kLIST_Ok;

    listStatus = LIST_Error_Check(list, listElement);
    if (listStatus == kLIST_Ok) /* Avoiding list status error */
    {
        /* Links element to the head of the list */
        if (list->size == 0U)
        {
            list->tail = listElement;
        }
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#else
        else
        {
            list->head->prev = listElement;
        }
        listElement->prev = NULL;
#endif
        listElement->list = list;
        listElement->next = list->head;
        list->head        = listElement;
        list->size++;
    }

    LIST_EXIT_CRITICAL();
    return listStatus;
}

/*! *********************************************************************************
 * \brief     Unlinks element from the head of the list.
 *
 * \param[in] list - ID of list to remove from.
 *
 * \return NULL if list is empty.
 *         ID of removed element(pointer) if removal was successful.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_element_handle_t LIST_RemoveHead(list_handle_t list)
{
    list_element_handle_t listElement;

    LIST_ENTER_CRITICAL();

    if ((NULL == list) || (list->size == 0U))
    {
        listElement = NULL; /*LIST_ is empty*/
    }
    else
    {
        listElement = list->head;
        list->size--;
        if (list->size == 0U)
        {
            list->tail = NULL;
        }
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#else
        else
        {
            listElement->next->prev = NULL;
        }
#endif
        listElement->list = NULL;
        list->head        = listElement->next; /*Is NULL if element is head*/
    }

    LIST_EXIT_CRITICAL();
    return listElement;
}

/*! *********************************************************************************
 * \brief     Gets head element ID.
 *
 * \param[in] list - ID of list.
 *
 * \return NULL if list is empty.
 *         ID of head element if list is not empty.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_element_handle_t LIST_GetHead(list_handle_t list)
{
    return list->head;
}

/*! *********************************************************************************
 * \brief     Gets next element ID.
 *
 * \param[in] element - ID of the element.
 *
 * \return NULL if element is tail.
 *         ID of next element if exists.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_element_handle_t LIST_GetNext(list_element_handle_t listElement)
{
    return listElement->next;
}

/*! *********************************************************************************
 * \brief     Gets previous element ID.
 *
 * \param[in] element - ID of the element.
 *
 * \return NULL if element is head.
 *         ID of previous element if exists.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_element_handle_t LIST_GetPrev(list_element_handle_t listElement)
{
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
    return NULL;
#else
    return listElement->prev;
#endif
}

/*! *********************************************************************************
 * \brief     Unlinks an element from its list.
 *
 * \param[in] element - ID of the element to remove.
 *
 * \return kLIST_OrphanElement if element is not part of any list.
 *         kLIST_Ok if removal was successful.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_status_t LIST_RemoveElement(list_element_handle_t listElement)
{
    list_status_t listStatus = kLIST_Ok;
    LIST_ENTER_CRITICAL();

    if (listElement->list == NULL)
    {
        listStatus = kLIST_OrphanElement; /*Element was previusly removed or never added*/
    }
    else
    {
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
        list_element_handle_t element_list = listElement->list->head;
        list_element_handle_t element_Prev = NULL;
        while (NULL != element_list)
        {
            if (listElement->list->head == listElement)
            {
                listElement->list->head = element_list->next;
                break;
            }
            if (element_list->next == listElement)
            {
                element_Prev       = element_list;
                element_list->next = listElement->next;
                break;
            }
            element_list = element_list->next;
        }
        if (listElement->next == NULL)
        {
            listElement->list->tail = element_Prev;
        }
#else
        if (listElement->prev == NULL)                   /*Element is head or solo*/
        {
            listElement->list->head = listElement->next; /*is null if solo*/
        }
        if (listElement->next == NULL)                   /*Element is tail or solo*/
        {
            listElement->list->tail = listElement->prev; /*is null if solo*/
        }
        if (listElement->prev != NULL)                   /*Element is not head*/
        {
            listElement->prev->next = listElement->next;
        }
        if (listElement->next != NULL) /*Element is not tail*/
        {
            listElement->next->prev = listElement->prev;
        }
#endif
        listElement->list->size--;
        listElement->list = NULL;
    }

    LIST_EXIT_CRITICAL();
    return listStatus;
}

/*! *********************************************************************************
 * \brief     Links an element in the previous position relative to a given member
 *            of a list.
 *
 * \param[in] element - ID of a member of a list.
 *            newElement - new element to insert before the given member.
 *
 * \return kLIST_OrphanElement if element is not part of any list.
 *         kLIST_Full if list is full.
 *         kLIST_Ok if insertion was successful.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
list_status_t LIST_AddPrevElement(list_element_handle_t listElement, list_element_handle_t newElement)
{
    list_status_t listStatus = kLIST_Ok;
    LIST_ENTER_CRITICAL();

    if (listElement->list == NULL)
    {
        listStatus = kLIST_OrphanElement; /*Element was previusly removed or never added*/
    }
    else
    {
        listStatus = LIST_Error_Check(listElement->list, newElement);
        if (listStatus == kLIST_Ok)
        {
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
            list_element_handle_t element_list = listElement->list->head;
            while (NULL != element_list)
            {
                if ((element_list->next == listElement) || (element_list == listElement))
                {
                    if (element_list == listElement)
                    {
                        listElement->list->head = newElement;
                    }
                    else
                    {
                        element_list->next = newElement;
                    }
                    newElement->list = listElement->list;
                    newElement->next = listElement;
                    listElement->list->size++;
                    break;
                }
                element_list = element_list->next;
            }

#else
            if (listElement->prev == NULL) /*Element is list head*/
            {
                listElement->list->head = newElement;
            }
            else
            {
                listElement->prev->next = newElement;
            }
            newElement->list = listElement->list;
            listElement->list->size++;
            newElement->next  = listElement;
            newElement->prev  = listElement->prev;
            listElement->prev = newElement;
#endif
        }
    }

    LIST_EXIT_CRITICAL();
    return listStatus;
}

/*! *********************************************************************************
 * \brief     Gets the current size of a list.
 *
 * \param[in] list - ID of the list.
 *
 * \return Current size of the list.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
uint32_t LIST_GetSize(list_handle_t list)
{
    return list->size;
}

/*! *********************************************************************************
 * \brief     Gets the number of free places in the list.
 *
 * \param[in] list - ID of the list.
 *
 * \return Available size of the list.
 *
 * \pre
 *
 * \post
 *
 * \remarks
 *
 ********************************************************************************** */
uint32_t LIST_GetAvailableSize(list_handle_t list)
{
    return (list->max - list->size); /*Gets the number of free places in the list*/
}
