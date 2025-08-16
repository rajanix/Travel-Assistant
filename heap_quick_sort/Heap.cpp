#ifndef STD_HEADERS_H
#include "std_headers.h"
#endif

#ifndef HEAP_H
#include "Heap.h"
#endif
// customised constructor.
template<typename T>
Heap<T>::Heap(){
  root = nullptr;
  last = nullptr;
}
// recursive delete for deleting the nodes of the tree in the destructor.
template<typename T>
void Heap<T>::recursiveDelete(TreeNode<T> *currNode) {
    if (currNode != nullptr) {
      TreeNode<T> *leftChild = currNode->left;
      TreeNode<T> *rightChild = currNode->right;
    
      delete currNode;
      recursiveDelete(leftChild);
      recursiveDelete(rightChild);
    }
    return;
}
// function to find the level of a particular node.
template<typename T>
int Heap<T>::findlevel(TreeNode<T> *node){
  int level = 0;
  while (node!= nullptr) {
    node=node->parent;
    level++;
  }
  return level;
}
// function to insert the node into the heap.
template<typename T>
void Heap<T>::insert(T obj) {
  if (root ==  nullptr) {// base case if the heap is null.
    root = new TreeNode<T>(obj);
    last = root;
    //printHeap(root);
    return;
  }
  // maintaining the last position's parent node during all the process.
  TreeNode<T> *currNode; // else 
  if (last->left == nullptr) { // left must be filled first.
    last->left = new TreeNode<T>(obj);
    last->left->parent = last; // connecting both ways
    currNode = last->left; // last added node
  }
  else { // only then right must be filled
    last->right = new TreeNode<T>(obj);
    last->right->parent = last; // connecting both ways
    currNode = last->right;// last added node
  }

  // moving it up till it does not violate the heap's property.  
  while (currNode->parent != nullptr){
    if (currNode->parent->object < currNode->object){
      T temp = currNode->object;
      currNode->object = currNode->parent->object ; 
      currNode->parent->object = temp; 
      currNode = currNode->parent;
    }
    else break;
  }
  // changing the last pointer.
  // if left is filled , then last does not change
  // if it is right then last has to change.
  if (last->right != nullptr ) {
    // finding the last parent whose left child is a ancestor of last 
    while (last->parent != nullptr && last->parent->right == last) last = last->parent;
    // the process had reached the root
    // in this case we have to move to do the left most node
    if (last->parent == nullptr){
      while (last->left != nullptr) last=last->left ;
      return;
    }
    // in this other case, move to the right child and move to its left most node.
    else if (last->parent->left = last) {
      last = last->parent->right;
      while (last->left != nullptr) last = last->left ;
      //printHeap(root);
      return;
    }
  }
  else {
    //printHeap(root); // for testing purposes
  }
  
}
// function for deleting the max element.
template<typename T>
T Heap<T>::delMax() {
  //printHeap(root);
  // insert your code here
  TreeNode<T> *currNode = root; // to be deleted
  TreeNode<T> *to_deleted ; // last node that is actually removed from the heap
  T obj = root ->object;// to be returned
  // since the last denotes the parent of the next node that has to be added
  // have to moved to last position of the last
  if (last->left != nullptr){ // if the last node added is a left child of its parent 
    to_deleted = last->left;
  }
  else {  
    while (last->parent != nullptr && last->parent->left == last){
      last = last->parent; // moving till you find a node whose left child is a ancestor of the last 
    }
    if (last->parent == nullptr){ // this is if you had reached the root 
      while (last->right != nullptr) last = last->right;// move till the right most node
      to_deleted = last; //to be deleted is last node 
      last = ( (last->parent != nullptr) ? last->parent : root );// the last node will be now the parent of to_be_deleted
    }
    else if (last->parent->right == last){ // otherwise, move to the right most node of the that sub-heap
      last = last->parent->left;
      while (last->right != nullptr) last=last->right;
      to_deleted = last;// updated the last node
      last = last->parent;// the last node will now be its parent
    }
  } 
  if (last==root && to_deleted ==root){ // if deleting the root
    last = nullptr;
    root = nullptr;
    return obj;
  }
  root->object = to_deleted->object;
  if (last->right != nullptr) last->right = nullptr; // deleting node other than root.
  else last->left = nullptr; 
  heapify(root); // heapify the root now
  return obj;
}

// heapify function used to modify the heap rooted at node if the given node 
// violate the heap property but the left and right sub heaps follow the heap property.
template<typename T>
void Heap<T>::heapify(TreeNode<T> *node){
  if (node == nullptr) return; // base cases
  else if (node->left == nullptr && node->right == nullptr) return; // base cases
  else if (node->left != nullptr && node->right == nullptr && node->object < node->left->object){ // swap only if the child is greater than the node
      T temp = node->object ; // only left node can be filled if only one child is present.
      node->object = node->left->object;
      node->left->object = temp; 
      heapify(node->left);
  }
  else if (node->right != nullptr) { // two nodes are present
    if (node->object < node->left->object || node->object < node->right->object){ // compare and swap 
      if (node->left->object < node->right->object && node->object < node->right->object){
        T temp = node->object ;
        node->object = node->right->object;
        node->right->object = temp; 
        heapify(node->right);// recursive call
      }
      else if (node->right->object < node->left->object && node->object < node->left->object) {
        T temp = node->object ;
        node->object = node->left->object;
        node->left->object = temp; 
        heapify(node->left);
      } 
      else if (node->object < node->left->object) { // default case in case both the nodes are equal
        T temp = node->object ;
        node->object = node->left->object;
        node->left->object = temp; 
        heapify(node->left);
      }
      else{
        return;
      }
    }
  } 
  else {
    return;
  }  
}

template<typename T>
void Heap<T>::printHeap(TreeNode<T> *node ){ // similar to print function in BST .
  static int no_of_spaces = 0; // but uses static variable and recursion
  if( node != nullptr )
  {    
      if (node->parent == nullptr) {
        for (int i=0;i<no_of_spaces;i++) cout << " " ;
        cout << "|--" << node->object << endl; // using same prefixes as printBST
      }
      else {
        for (int i=0;i<no_of_spaces;i++) cout << "   " ;
        cout << ( (node->parent->left == node) ? "|--" : "|__" );
        // print the value of the node
        cout << node->object << endl;
      }
      TreeNode<T> *curr = node;
      node = node->left;
      // enter the next tree level - left and right branch
      no_of_spaces ++ ; // increasing the no of spaces after checking each level.
      printHeap(node);
      node = curr->right;
      printHeap(node);
      node = curr;
      no_of_spaces --; // decreasing the no of spaces after checking each level.
  }
}

template<typename T>
Heap<T>::~Heap(){ // custom defined destructor
  recursiveDelete(root);
}
