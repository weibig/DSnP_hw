/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   
   int parent(int i) { return (i-1)/2; } 
   void swap(Data *x, Data *y) 
   { 
      Data temp = *x; 
      *x = *y; 
      *y = temp; 
   }

   void insert(const Data& d) 
   {
      // First insert the new key at the end 
      _data.push_back(d);
      
      int i = _data.size()-1;
      // Fix the min heap property if it is violated 
      while (i != 0 && _data[i] < _data[parent(i)]) 
      { 
         swap(&_data[i], &_data[parent(i)]); ////
         i = parent(i); 
      } 
   }
   void MinHeapify(int i) 
   { 
      int l = (2*i + 1); //left
      int r = (2*i + 2); //right
      int smallest = i; 
      if (l < _data.size() && _data[l] < _data[i]) 
          smallest = l; 
      if (r < _data.size() && _data[r] < _data[smallest]) 
          smallest = r; 
      if (smallest != i) 
      { 
          swap(&_data[i], &_data[smallest]); 
          MinHeapify(smallest); 
      } 
   } 


   void delMin() 
   {
      _data[0] = _data[_data.size()-1];
      _data.pop_back();
      MinHeapify(0);

   }
   void delData(size_t i) 
   {
      _data[i] = _data[_data.size()-1];
      _data.pop_back();
      // Fix the min heap property if it is violated 
      MinHeapify(i);
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
