//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   xlList.h
//    Author:      Streamlet
//    Create Time: 2009-09-26
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __XLLIST_H_2BEF1B3C_A056_4EC7_B5E3_9898E7945B54_INCLUDED__
#define __XLLIST_H_2BEF1B3C_A056_4EC7_B5E3_9898E7945B54_INCLUDED__


#include "../Memory/xlMemory.h"
#include "../Meta/xlAssert.h"
#include "xlLinkedListNode.h"
#include "xlIterator.h"

namespace xl
{
    template <typename T, typename NodeType = LinkedListNode<T>>
    class List
    {
    public:
        List()
            : m_pHead(nullptr), m_pTail(nullptr), m_nSize(0)
        {

        }

        List(const List &that)
            : m_pHead(nullptr), m_pTail(nullptr), m_nSize(0)
        {
            *this = that;
        }

        List(List &&that)
            : m_pHead(nullptr), m_pTail(nullptr), m_nSize(0)
        {
            *this = Memory::Move(that);
        }

        ~List()
        {
            Release();
        }

    public:
        List &operator = (const List &that)
        {
            if (this == &that)
            {
                return *this;
            }

            this->Clear();

            for (NodeType *p = that.m_pHead; p != nullptr; p = p->pNext)
            {
                this->PushBack(p->tValue);
            }

            return *this;
        }

        List &&operator = (List &&that)
        {
            if (this == &that)
            {
                return *this;
            }

            this->m_pHead = that.m_pHead;
            this->m_pTail = that.m_pTail;
            this->m_nSize = that.m_nSize;

            that.m_pHead = nullptr;
            that.m_pTail = nullptr;
            that.m_nSize = 0;

            return *this;
        }

        bool operator == (const List &that) const
        {
            if (this == &that)
            {
                return true;
            }

            if (this->m_nSize != that.m_nSize)
            {
                return false;
            }

            for (NodeType *pThis = this->m_pHead, *pThat = that.m_pHead;
                 pThis != nullptr && pThat != nullptr;
                 pThis = pThis->pNext, pThat = pThat->pNext)
            {
                if (pThis->tValue != pThat->tValue)
                {
                    return false;
                }
            }

            return true;
        }

        bool operator != (const List &that) const
        {
            if (this == &that)
            {
                return false;
            }

            if (this->m_nSize != that.m_nSize)
            {
                return true;
            }

            for (NodeType *pThis = this->m_pHead, *pThat = that.m_pHead;
                 pThis != nullptr && pThat != nullptr;
                 pThis = pThis->pNext, pThat = pThat->pNext)
            {
                if (pThis->tValue != pThat->tValue)
                {
                    return true;
                }
            }

            return false;
        }

    public:
        bool Empty() const
        {
            return m_nSize == 0;
        }

        size_t Size() const
        {
            return m_nSize;
        }

    protected:
        void Insert(NodeType *pWhere, const T &tValue)
        {
            NodeType *pNewNode = new NodeType(tValue);

            XL_ASSERT(m_pHead == nullptr && m_pTail == nullptr && m_nSize == 0 ||
                      m_pHead != nullptr && m_pTail != nullptr && m_nSize != 0);

            if (m_pHead == nullptr && m_pTail == nullptr)
            {
                m_pHead = pNewNode;
                m_pTail = pNewNode;
            }
            else if (pWhere == m_pHead)
            {
                pNewNode->pNext = m_pHead;
                m_pHead->pPrev = pNewNode;
                m_pHead = pNewNode;
            }
            else if (pWhere == nullptr)
            {
                pNewNode->pPrev = m_pTail;
                m_pTail->pNext = pNewNode;
                m_pTail = pNewNode;
            }
            else
            {
                pNewNode->pPrev = pWhere->pPrev;
                pNewNode->pNext = pWhere;
                pWhere->pPrev->pNext = pNewNode;
                pWhere->pPrev = pNewNode;
            }

            ++m_nSize;
        }

        void Delete(NodeType *pNode)
        {
            if (pNode == m_pHead && pNode == m_pTail && pNode != nullptr)
            {
                m_pHead = nullptr;
                m_pTail = nullptr;
            }
            else if (pNode == m_pHead && pNode != m_pTail)
            {
                pNode->pNext->pPrev = nullptr;
                m_pHead = pNode->pNext;
            }
            else if (pNode == m_pTail && pNode != m_pHead)
            {
                pNode->pPrev->pNext = nullptr;
                m_pTail = pNode->pPrev;
            }
            else
            {
                pNode->pPrev->pNext = pNode->pNext;
                pNode->pNext->pPrev = pNode->pPrev;
            }

            delete pNode;
            --m_nSize;

            XL_ASSERT(m_pHead == nullptr && m_pTail == nullptr && m_nSize == 0 ||
                      m_pHead != nullptr && m_pTail != nullptr && m_nSize != 0);
        }

    public:
        void PushFront(const T &tValue)
        {
            Insert(m_pHead, tValue);
        }

        void PushBack(const T &tValue)
        {
            Insert(nullptr, tValue);
        }

        void PopFront()
        {
            Delete(m_pHead);
        }

        void PopBack()
        {
            Delete(m_pTail);
        }

        void Clear()
        {
            for (NodeType *p = m_pHead, *q = p; p != nullptr; )
            {
                p = p->pNext;
                delete q;
            }

            m_pHead = nullptr;
            m_pTail = nullptr;
            m_nSize = 0;
        }

    protected:
        NodeType *m_pHead;
        NodeType *m_pTail;
        size_t m_nSize;

    public:
        typedef LinkedListIterator<T, NodeType> Iterator;
        typedef ReverseLinkedListIterator<T, NodeType> ReverseIterator;

    protected:
        void Release()
        {
            Clear();
        }

    public:
        Iterator Begin() const
        {
            return Iterator(m_pHead);
        }

        Iterator End() const
        {
            return Iterator(nullptr);
        }

        ReverseIterator ReverseBegin() const
        {
            return ReverseIterator(m_pTail);
        }

        ReverseIterator ReverseEnd() const
        {
            return ReverseIterator(nullptr);
        }

    public:
        void Insert(const Iterator &itWhere, const T &tValue)
        {
            NodeType *pWhere = (NodeType *)itWhere;
            Insert(pWhere, tValue);
        }

        void Insert(const ReverseIterator &itWhere, const T &tValue)
        {
            NodeType *pWhere = (NodeType *)itWhere;
            Insert(pWhere, tValue);
        }

        template <typename I>
        void Insert(const Iterator &itWhere, const I &itBegin, const I &itEnd)
        {
            for (I it = itBegin; it != itEnd; ++it)
            {
                Insert(itWhere, *it);
            }
        }

        template <typename I>
        void Insert(const ReverseIterator &itWhere, const I &itBegin, const I &itEnd)
        {
            for (I it = itBegin; it != itEnd; ++it)
            {
                Insert(itWhere, *it);
            }
        }

        Iterator Delete(const Iterator &itWhere)
        {
            NodeType *pNode = (NodeType *)itWhere;
            NodeType *pNext = pNode->pNext;
            Delete(pNode);
            return Iterator(pNext);
        }

        ReverseIterator Delete(const ReverseIterator &itWhere)
        {
            NodeType *pNode = (NodeType *)itWhere;
            NodeType *pPrev = pNode->pPrev;
            Delete(pNode);
            return ReverseIterator(pPrev);
        }

        Iterator Delete(const Iterator &itBegin, const Iterator &itEnd)
        {
            NodeType *pBegin = (NodeType *)itBegin;
            NodeType *pEnd = (NodeType *)itEnd;

            for (NodeType *p = pBegin; p != pEnd; p = p->pNext)
            {
                Delete(p);
            }

            return Iterator(pEnd);
        }

        ReverseIterator Delete(const ReverseIterator &itBegin, const ReverseIterator &itEnd)
        {
            NodeType *pBegin = (NodeType *)itBegin;
            NodeType *pEnd = (NodeType *)itEnd;

            for (NodeType *p = pBegin; p != pEnd; p = p->pNext)
            {
                Delete(p);
            }

            return ReverseIterator(pEnd);
        }
    };

} // namespace xl

#endif // #ifndef __XLLIST_H_2BEF1B3C_A056_4EC7_B5E3_9898E7945B54_INCLUDED__
