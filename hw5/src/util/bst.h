/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
	friend class BSTree<T>;
	friend class BSTree<T>::iterator;

private:
    BSTreeNode<T> *leftchild;
    BSTreeNode<T> *rightchild;
    BSTreeNode<T> *parent;
    T _data;

 	BSTreeNode(const T& d, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0, BSTreeNode<T>* p = 0):
      _data(d), leftchild(l), rightchild(r), parent(p) {}


};


template <class T>
class BSTree
{
   // TODO: design your own class!
private:
    BSTreeNode<T> *root;
    BSTreeNode<T>* Leftmost(BSTreeNode<T> *current);
    BSTreeNode<T>* Successor(BSTreeNode<T> *current);
public:
    BSTree():root(0){};    

    BSTreeNode<T>* Search(int key);
    void InsertBST(int key, string element);
    void InorderPrint();        // 可以用來確認BST是否建立成功
    void Levelorder();          // 可以確認BST是否建立成功
    class iterator
   {
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->rightchild; return *(this); }
      iterator operator ++ (int) { BSTreeNode<T>* temp = _node; _node = _node->rightchild; return temp; }
      iterator& operator -- () { _node = _node->leftchild; return *(this); }
      iterator operator -- (int) { BSTreeNode<T>* temp = _node; _node = _node->leftchild; return temp; }

      iterator& operator = (const iterator& i) { _node = (*i); return *(this); }

      bool operator != (const iterator& i) const { return _node->_data != (*i); }
      bool operator == (const iterator& i) const { return _node->_data == (*i); }

   private:
      BSTreeNode<T>* _node;
   };

};

#endif // BST_H
