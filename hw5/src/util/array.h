/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0) {_isSorted = 0;}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*this); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () { _node = _node+1; return (*this); }
      iterator operator ++ (int) { iterator temp(*this); _node = _node+1; return (temp); }
      iterator& operator -- () { _node = _node-1; return (*this); }
      iterator operator -- (int) { iterator temp(*this); _node = _node-1; return (temp); }

      iterator operator + (int i) const { iterator temp(this->_node + i); return (temp); }
      iterator& operator += (int i) { _node = _node + i; return (*this); }

      iterator& operator = (const iterator& i) { this->_node = i._node; return (*this); }

      bool operator != (const iterator& i) const { return !(this->_node == i._node); }
      bool operator == (const iterator& i) const { return (this->_node == i._node); }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const {if(_capacity == 0) {return 0;} return _data; }///cout << "_data = " << _data << endl; 

   //end() points to the next address of the last element. If the Array is NOT yet initialized (i.e. _capacity == 0), both begin() and end() = 0.
   iterator end() const {if(_capacity == 0) {return 0;} return _data+_size; }///
   bool empty() const { return (_size == 0); }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { if(i < _size)return _data[i]; return 0;}
   const T& operator [] (size_t i) const { if(i < _size)return _data[i]; return 0; }

   void push_back(const T& x) 
   {
      // cout << "Call push_back()" << endl;///
      // cout << _capacity << " " << _size << endl; ///
      if(_capacity <= _size) 
         expand(); 
      _data[_size] = x; 

      // cout << "_data[_size] = " << _data[_size] << " " << _size << endl;///

      _size++;
      
      // cout << (begin() == _data) << " " << _data << endl;///
      // cout << (end() == &_data[_size]) << " " << &_data[_size] << endl;///
   }
   

   void pop_front() {if(empty()) return; _data[0] = _data[_size-1]; _size--; }//delete only
   void pop_back() {if(empty()) return; _size--;}


   bool erase(iterator pos) {  if(empty()) return false; *pos = *(end()+(-1)); _size--; return true;} //cout << "erase: " << *pos << endl;
   

   //remove the element x from the ADT. Return false if x does not exist in the ADT. If there are multiple existences of element x in the ADT, remove the firstly encountered one (i.e. by traversing from begin()) and leave the others untouched. The size of ADT, of course, will be decremented by 1 afterwards.
   bool erase(const T& x) 
   { 
      
      for(int i = 0; i < _size; i++)
      {
         if(_data[i] == x)
         {
            _data[i] = _data[_size-1]; 
            _size--;
            return true;
         }
      }
      return false; 
   }

   iterator find(const T& x) { for(iterator li = begin(); li != end(); li++){if(*li == x) return li;} return end(); }

   //reset its _size to 0, DO NOT release its memory (i.e. _capacity remains the same).
   void clear() { _size = 0;}

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
   void expand()
   {
      // cout << "Call expand" << endl;
      //cout << "After expand with _capacity = " << _capacity << endl;
      if(_capacity == 0) 
         _capacity = 1; 
      else
         _capacity = _capacity * 2; 
      
      T *temp = new T[_capacity];
      
      for(int i = 0; i < _size; i++)
         temp[i] = _data[i];
      
      _data = temp;

   }
};

#endif // ARRAY_H
