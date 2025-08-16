#ifndef GRAPH_BST_H
#define GRAPH_BST_H

#ifndef STD_HEADERS_H
#include "std_headers.h"
#endif

struct TreeNode{// Tree node
  int value;
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent;

  TreeNode(int value){
    this->value = value;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
  }
  TreeNode(int value,TreeNode* left,TreeNode* right,TreeNode* parent){
    this->value = value;
    this->left = left ;
    this->right = right;
    this->parent = parent;
  }
};

template <typename T>
class links{ // linked list 
  public:
    T value;
    links* next;
    links* prev;

    links(T value){
      this->value = value;
      next = nullptr;
      prev = nullptr;
    }

    links(T value,links* prev){
      this->value = value;
      this->prev = prev;
      next = nullptr;
    }
};

class BST
{
  TreeNode *root;
public:
  BST() {
    root = nullptr;
  }
  BST(TreeNode *node){
    root = node;
  }
  // inserting the node in the BST
  void insert(int value){
    if(root == nullptr){
      root =  new TreeNode(value);
    }
    else{
      TreeNode *node = root,*parent = nullptr;
      while (node != nullptr){
        parent = node;
        if (node->value < value) node = node->right;
        else node = node->left;      
      }
      if (node==nullptr){
        if (parent->value < value) parent->right = new TreeNode(value,nullptr,nullptr,parent);
        else parent->left = new TreeNode(value,nullptr,nullptr,parent);
      }
    }
  }
  // printing the BST
  void printBST(const string& prefix, bool isLeft=false)
  {
      if( root != nullptr )
      {
          cout << prefix;
          cout << (isLeft ? "|--" : "|__" );
          // print the value of the node
          cout << root->value<< endl;
          TreeNode *curr = root;
          root = root->left;
          // enter the next tree level - left and right branch
          printBST( prefix + (isLeft ? "│   " : "    "), true);
          root = curr->right;
          printBST( prefix + (isLeft ? "│   " : "    "), false);
          root = curr;
      }
  }
};


class Node // Graph nodes
{
public:
  int value;
  links<int> *adjacency_list; // stores the indices of the neighbours but the it refers to the last node
  int no_of_neighbours;

  Node(){
    this->adjacency_list = nullptr;
    no_of_neighbours = 0;
  }
};

class Graph
{
  int numNodes;
  int numEdges;
  fstream graphFile;

  Node *nodes; // array of the node
  links<BST> *trees; // linked lists of the tree

  int *visited; // visited array
  bool *incycle; // array to find if the node is in the cycle
  int *pred; // predecessor array 
  int *arrtime; // arrival time
  int no_of_trees; // no of connected components
  int timer; // timer for the node

public:
  Graph(int nNodes, int nEdges, string gFileName)
  {
    // intialising
    numNodes = nNodes;
    numEdges = nEdges;
    timer = 0;
    visited = new int[numNodes];
    incycle = new bool[numNodes];
    pred = new int[numNodes];
    arrtime = new int[numNodes];
    trees = nullptr;
    for (int i=0;i<numNodes;i++){
      visited[i] = 0;
      incycle[i] = 0;
      arrtime[i] = -1;
      pred[i] = -1;
    }
    
    graphFile.open(gFileName, ios::out);
    if (!graphFile.is_open())
    {
      cerr << "Couldn't open graph file " << gFileName << ". Aborting ..." << endl;
      exit(-1);
    }

    graphFile << numNodes << endl;
    graphFile << numEdges << endl;

    nodes = new Node[numNodes];
    if (nodes == nullptr)
    {
      cerr << "Memory allocation failure." << endl;
      exit(-1);
    }

    for (int i = 0; i < numNodes; i++)
    {
      nodes[i].value = i + 1;
    }

    srand(time(0));
    int n1, n2;
    int countEdges = 0;
    while (countEdges < numEdges)
    {
      do
      {
        n1 = rand() % numNodes;
        n2 = rand() % numNodes;
        // cin >> n1 >> n2;
        // n1--;n2--;
      } while (n1 == n2);
      // checking the adjacency list
      if (nodes[n1].adjacency_list!= nullptr){
        int i=0;
        links<int>* node = nodes[n1].adjacency_list;
        for (i=0;i<nodes[n1].no_of_neighbours;i++) {
          if (node->value == n2) break;
          node = node->prev;
        }
        if (i!=nodes[n1].no_of_neighbours) continue;
      } 
      // adding the edges 
      if (nodes[n1].adjacency_list == nullptr) {
        nodes[n1].adjacency_list = new links<int>(n2);
      }
      else {
        nodes[n1].adjacency_list->next = new links<int>(n2,nodes[n1].adjacency_list);
        nodes[n1].adjacency_list = nodes[n1].adjacency_list->next;
      }
      nodes[n1].no_of_neighbours ++;
      // adding the edges
      if (nodes[n2].adjacency_list == nullptr) {
        nodes[n2].adjacency_list = new links<int>(n1);
      }
      else {
        nodes[n2].adjacency_list->next = new links<int>(n1,nodes[n2].adjacency_list);
        nodes[n2].adjacency_list = nodes[n2].adjacency_list->next;      
      }
      nodes[n2].no_of_neighbours ++;

      graphFile << n1 + 1 << " " << n2 + 1 << endl;
      countEdges++;
    }
  }

  ~Graph()
  {
    if (graphFile.is_open())
    {
      graphFile.close();
    }
    delete[] nodes;
  }

  // new function added.
  bool isancestor(int node,int prevnode){
    while (node != prevnode){
      if (node == -1) return false;
      node = pred[node];
    }
    return true;
  }
  void DFS(int node,BST &tree);
  void modifiedDFS();
  void printResults();
};

#endif
