/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
      _isSorted = false;
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_next; return *(this); }
      iterator operator ++ (int) { DListNode<T>* temp = _node; _node = _node->_next; return temp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { DListNode<T>* temp = _node; _node = _node->_prev; return temp; }

      iterator& operator = (const iterator& i) { _node = (*i); return *(this); }

      bool operator != (const iterator& i) const { return _node->_data != (*i); }
      bool operator == (const iterator& i) const { return _node->_data == (*i); }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return _head; }
   iterator end() const { return (_head->_prev); }///
   bool empty() const { return (begin() == end()); }
   size_t size() const { 
      size_t count = 0;
      for(iterator li = begin(); li != end(); li++)
         count++;

      return count;
   }///

   void push_back(const T& x) { 


      DListNode<T> *temp = new DListNode<T>(x); 
      
      temp->_prev = end()._node->_prev;
      temp->_next = end()._node;
      (end()._node->_prev)->_next = temp; ///
      (end()._node->_prev) = temp;

      if(size() == 1)
      {
         _head = temp;
      }


   }

   void pop_front() {
      // cout << "pop_front()!!" << endl;///
      bool changeHead = false;
      if(size() == 1)
         changeHead = true;



      (end()._node->_next) = _head->_next;
      _head->_next->_prev = end()._node;
      _head = _head->_next;

      if(changeHead)
      {
         // cout << "self cycle!" << endl;
         _head = end()._node;
      }

      
   }
   void pop_back() {
      // cout << "pop_back()!!" << endl << size();///

      erase(end());

      // *(end()._node->_prev) = *(end()._node->_prev->_prev);///
      // *(end()._node->_prev->_prev)->_next = *end();
   }

   // return false if nothing to erase
   bool erase(iterator pos) {
      if(empty())
      {
         // cout << "erase empty!!" << endl;///
         return false; 
      }
      else if(pos == begin())
      {
         pop_front();
      }
      else
      {
         // cout << "erase1: " << *pos << " " << size() << endl;///
         (pos._node->_next)->_prev = (pos._node->_prev);
         (pos._node->_prev)->_next = (pos._node->_next);
         
      }

      return true;
   }
   bool erase(const T& x) { 
      if(find(x) != end())
      {
         // cout << "erase2: " << x << endl;///
         erase(find(x));
         return true;
      }

      return false; 
   }

   iterator find(const T& x) { 
      for(iterator li = begin(); li != end(); li++)
      {
         if(*li == x) 
            return li;
      }

      return end();
   }

   void clear() {(end()._node->_prev) = end()._node; (end()._node->_next) = end()._node; _head = end()._node;}  // delete all nodes except for the dummy node

   void sort() const {

      iterator next = begin();
      for(iterator i = begin(); i != end(); i++)
      {
         next++;
         for(iterator j = next; j != end(); j++)
         {
            if(*i > *j)
            {
               T temp = *i;
               *i = *j;
               *j = temp;
            }
         }
      }
      
   }
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
   // Partitions the list taking the last element as the pivot 
     
   /* sorts the linked list by changing next pointers (not data) */

   // void MergeSort(DListNode<T>*  head) 
   // { 
   //     DListNode<T>* a; 
   //     DListNode<T>* b; 
     
   //     /* Base case -- length 0 or 1 */
   //     if ((head == NULL) || (head->next == NULL)) { 
   //         return; 
   //     } 
     
   //     /* Split head into 'a' and 'b' sublists */
   //     FrontBackSplit(head, &a, &b); 
     
   //     /* Recursively sort the sublists */
   //     MergeSort(&a); 
   //     MergeSort(&b); 
     
   //     /* answer = merge the two sorted lists together */
   //     head = SortedMerge(a, b); 
   // } 
     
   // /* See https:// www.geeksforgeeks.org/?p=3622 for details of this  
   // function */
   // DListNode<T>* SortedMerge(DListNode<T>* a, DListNode<T>* b) 
   // { 
   //     DListNode<T>* result = NULL; 
     
   //     /* Base cases */
   //     if (a == NULL) 
   //         return (b); 
   //     else if (b == NULL) 
   //         return (a); 
     
   //     /* Pick either a or b, and recur */
   //     if (a->_data <= b->_data) { 
   //         result = a; 
   //         result->_next = SortedMerge(a->_next, b); 
   //     } 
   //     else { 
   //         result = b; 
   //         result->_next = SortedMerge(a, b->_next); 
   //     } 
   //     return (result); 
   // } 
     
   // /* UTILITY FUNCTIONS */
   // /* Split the nodes of the given list into front and back halves,  
   //     and return the two lists using the reference parameters.  
   //     If the length is odd, the extra node should go in the front list.  
   //     Uses the fast/slow pointer strategy. */
   // void FrontBackSplit(DListNode<T>* source, 
   //                     DListNode<T>** frontRef, DListNode<T>** backRef) 
   // { 
   //     DListNode<T>* fast; 
   //     DListNode<T>* slow; 
   //     slow = source; 
   //     fast = source->_next; 
     
   //     /* Advance 'fast' two nodes, and advance 'slow' one node */
   //     while (fast != NULL) { 
   //         fast = fast->_next; 
   //         if (fast != NULL) { 
   //             slow = slow->_next; 
   //             fast = fast->_next; 
   //         } 
   //     } 
     
   //     /* 'slow' is before the midpoint in the list, so split it in two  
   //     at that point. */
   //     *frontRef = source; 
   //     *backRef = slow->_next; 
   //     slow->_next = NULL; 
   // } 
     
};

#endif // DLIST_H
