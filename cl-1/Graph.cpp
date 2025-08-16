#ifndef GRAPH_BST_CPP
#define GRAPH_BST_CPP

#ifndef STD_HEADERS_H
#include "std_headers.h"
#endif

#ifndef GRAPH_BST_H
#include "Graph.h"
#endif

// recursive DFS 
void Graph::DFS(int node,BST &tree) { 
  // visiting the node
  visited[node]++;
  // arrival time,essential for finding cycles
  if (arrtime[node] == -1) arrtime[node] = ++timer;  
  tree.insert(node+1); // inserting the node
  links<int> *currnode = nodes[node].adjacency_list; // adjacency_list
  for(int i=0;i<nodes[node].no_of_neighbours;i++){ 
    if (visited[currnode->value] == 0) { // visiting it first time
      pred[currnode->value] = node; 
      // if (node == 8146) cout <<"CHILD:"<<currnode->value<<endl;      
      DFS(currnode->value,tree);
    }
    else {
      if (currnode->value!= pred[node] && arrtime[currnode->value] < arrtime[node]) { // visting a node which is already visited
        int prevnode = currnode->value;
        int currentNode = node;
        bool crossEdge = !(isancestor(node,prevnode));
        if (!crossEdge){
          while (currentNode != prevnode){
            incycle[currentNode] = 1;
            currentNode = pred[currentNode];
          }
          incycle[prevnode] = 1;
        }
        else { // case where there is a edge like cross edge
          while (currentNode != -1){
            currentNode = pred[currentNode];
            prevnode = currnode->value;
            while (prevnode !=-1){
              if (currentNode == prevnode) break;
              prevnode = pred[prevnode];
            }            
            if (prevnode != -1) {      
              int curr = node;
              while (curr != prevnode){
                incycle[curr] = 1;
                curr = pred[curr];
              }        
              int pre = currnode->value;
              while (pre != prevnode){
                incycle[pre] = 1;
                pre = pred[pre];
              }
              incycle[prevnode] = 1;
              break;
            } // it is always assured that there will be a Least common ancestor
          }
        }
      } 
      if (currnode->value != pred[node]) {  // except when it is a parent we have to do DFS we encounter it next time.
        if (visited[currnode->value] == 1) DFS(currnode->value,tree);
      }      
    }
    currnode = currnode->prev; // moving across adjacency list
  }
}

void Graph::modifiedDFS(){ // tranversing the visited array
  trees = nullptr;
  no_of_trees = 0;
  for (int i=0;i<numNodes;i++){
    if (visited[i] == 0){
      BST *tree = new BST(); // new BST
      DFS(i,*tree);
      if (trees == nullptr) trees = new links<BST>(*tree);
      else{
        trees->next = new links<BST>(*tree,trees); // linked list of the BSTs 
        trees = trees->next; // every new node will be added to end of the linked list  
      }
      no_of_trees++;
    }
  }
}

void Graph::printResults() { // printing the results
  cout << "No. of connected components:" << no_of_trees<< endl;
  int singleVisited = 0,doubleVisited = 0,cycle = 0;
  for (int i=0;i<numNodes;i++){
    if (visited[i] == 1) singleVisited++;
    else doubleVisited++;
    if (incycle[i]) cycle++;
  }
  cout <<"No. of nodes visited once:" << singleVisited<< endl;
  cout <<"No. of nodes visited twice:" << doubleVisited<< endl;
  cout <<"No. of nodes that are present in a cycle:" << cycle<< endl;
  for (int i=0;i<no_of_trees;i++){
    cout <<"Connected Component-BST No." << i+1<< endl;
    trees->value.printBST("");
    trees = trees->prev;
  }
  //cout << pred[0] << endl;
  return;
}

#endif
