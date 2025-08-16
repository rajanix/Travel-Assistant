#ifndef HEAP_H
#define HEAP_H

#ifndef STD_HEADERS
#include "std_headers.h"
#endif

using namespace std;

template <typename T> class TreeNode {
 public:
  T object;
  TreeNode<T> *left;
  TreeNode<T> *right;
  TreeNode<T> *parent;

  TreeNode( TreeNode<T> *left,TreeNode<T> *parent,TreeNode<T> *right){
    this->left = left ;
    this->parent = parent;
    this->right = right;
  }
  TreeNode(T initObj) {
    object = initObj;
    left = right = parent = nullptr;
  }
  ~TreeNode() {;}
};

template <typename T> class Heap {
  TreeNode<T> *root;
  TreeNode<T> *last; // parent of next node that has to be filled.
  //TreeNode<T> *UpdateLastNode(TreeNode<T> *node);
  int findlevel(TreeNode<T> *node);
  void recursiveDelete(TreeNode<T> *currNode);
  void heapify(TreeNode<T> *node);
  
 public:
  Heap();
  ~Heap();

  void insert(T obj);
  T delMax();
  T getMax() {
    if (root!=nullptr) return root->object;
  }
  void printHeap(TreeNode<T> *node);
  //  void displayAboveThresh(int thresh);
};
#endif

