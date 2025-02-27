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
list_handle_t LIST_GetList(list_element_handle_t element)
{
    return element->list;
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
list_status_t LIST_AddTail(list_handle_t list, list_element_handle_t element)
{
    LIST_ENTER_CRITICAL();
    list_status_t listStatus = kLIST_Ok;

    listStatus = LIST_Error_Check(list, element);
    if (listStatus == kLIST_Ok) /* Avoiding list status error */
    {
        if (list->size == 0U)
        {
            list->head = element;
        }
        else
        {
            list->tail->next = element;
        }
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#else
        element->prev = list->tail;
#endif
        element->list = list;
        element->next = NULL;
        list->tail    = element;
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
list_status_t LIST_AddHead(list_handle_t list, list_element_handle_t element)
{
    LIST_ENTER_CRITICAL();
    list_status_t listStatus = kLIST_Ok;

    listStatus = LIST_Error_Check(list, element);
    if (listStatus == kLIST_Ok) /* Avoiding list status error */
    {
        /* Links element to the head of the list */
        if (list->size == 0U)
        {
            list->tail = element;
        }
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#else
        else
        {
            list->head->prev = element;
        }
        element->prev = NULL;
#endif
        element->list = list;
        element->next = list->head;
        list->head    = element;
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
    list_element_handle_t element;

    LIST_ENTER_CRITICAL();

    if ((NULL == list) || (list->size == 0U))
    {
        element = NULL; /*LIST_ is empty*/
    }
    else
    {
        element = list->head;
        list->size--;
        if (list->size == 0U)
        {
            list->tail = NULL;
        }
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#else
        else
        {
            element->next->prev = NULL;
        }
#endif
        element->list = NULL;
        list->head    = element->next; /*Is NULL if element is head*/
    }

    LIST_EXIT_CRITICAL();
    return element;
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
list_element_handle_t LIST_GetNext(list_element_handle_t element)
{
    return element->next;
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
list_element_handle_t LIST_GetPrev(list_element_handle_t element)
{
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
    return NULL;
#else
    return element->prev;
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
list_status_t LIST_RemoveElement(list_element_handle_t element)
{
    list_status_t listStatus = kLIST_Ok;
    LIST_ENTER_CRITICAL();

    if (element->list == NULL)
    {
        listStatus = kLIST_OrphanElement; /*Element was previusly removed or never added*/
    }
    else
    {
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
        list_element_handle_t element_list = element->list->head;
        while (NULL != element_list)
        {
            if (element->list->head == element)
            {
                element->list->head = element_list->next;
                break;
            }
            if (element_list->next == element)
            {
                element_list->next = element->next;
                break;
            }
            element_list = element_list->next;
        }
#else
        if (element->prev == NULL) /*Element is head or solo*/
        {
            element->list->head = element->next; /*is null if solo*/
        }
        if (element->next == NULL) /*Element is tail or solo*/
        {
            element->list->tail = element->prev; /*is null if solo*/
        }
        if (element->prev != NULL) /*Element is not head*/
        {
            element->prev->next = element->next;
        }
        if (element->next != NULL) /*Element is not tail*/
        {
            element->next->prev = element->prev;
        }
#endif
        element->list->size--;
        element->list = NULL;
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
list_status_t LIST_AddPrevElement(list_element_handle_t element, list_element_handle_t newElement)
{
    list_status_t listStatus = kLIST_Ok;
    LIST_ENTER_CRITICAL();

    if (element->list == NULL)
    {
        listStatus = kLIST_OrphanElement; /*Element was previusly removed or never added*/
    }
    else
    {
        listStatus = LIST_Error_Check(element->list, newElement);
        if (listStatus == kLIST_Ok)
        {
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
            list_element_handle_t element_list = element->list->head;
            while (NULL != element_list)
            {
                if ((element_list->next == element) || (element_list == element))
                {
                    if (element_list == element)
                    {
                        element->list->head = newElement;
                    }
                    else
                    {
                        element_list->next = newElement;
                    }
                    newElement->list = element->list;
                    newElement->next = element;
                    element->list->size++;
                    break;
                }
                element_list = element_list->next;
            }

#else
            if (element->prev == NULL) /*Element is list head*/
            {
                element->list->head = newElement;
            }
            else
            {
                element->prev->next = newElement;
            }
            newElement->list = element->list;
            element->list->size++;
            newElement->next = element;
            newElement->prev = element->prev;
            element->prev = newElement;
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
