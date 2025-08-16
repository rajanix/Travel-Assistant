#ifndef PLANNER_H
#include "planner.h"
#endif

#ifndef STD_HEADERS_H
#include "std_headers.h"
#endif

using namespace std;

void Planner::printDirectJourneys(string srcStnName, string destStnName)
{
  if (srcStnName == destStnName ) {
    cout << "SAME SOURCE AND DESTINATION STATIONS" << endl;
    return;
  }
  int index = stnNameToIndex.hashValue(srcStnName); // have to find the index where the station is 
  while (stnNameToIndex.getKeyAtIndex(index)!=srcStnName) index = (index+1)%(DICT_SIZE); 

  listOfObjects<StationConnectionInfo *> *tostation = adjacency[index].toStations;// adjacent stations to source
  listOfObjects<StationConnectionInfo *> *first = adjacency[index].toStations; // to check if the neighbour is the destination
  if (tostation == nullptr){
    cout << "NO TRAINS AVAILABLE DIRECTLY"<< endl;    
    return;
  }
  listOfObjects<TrainInfoPerStation *> *train = tostation->object->trains; // trains joining them

  while (first != nullptr && stnNameToIndex.getKeyAtIndex(first->object->adjacentStnIndex) != destStnName){
    first = first->next;
  }
  listOfObjects<int> *jcs = nullptr; // to store the corresponding journey codes 
  if (first != nullptr && stnNameToIndex.getKeyAtIndex(first->object->adjacentStnIndex) == destStnName) { 
  // incase if it is the case that adjacent station is the destination 
    listOfObjects<TrainInfoPerStation *> *Trains = first->object->trains;
    while (Trains != nullptr){
      listOfObjects<int> *node = new listOfObjects<int>(Trains->object->journeyCode);
      node->next = jcs;
      jcs = node;
      Trains = Trains->next;
    }
  }
  // optimised bfs algorithm is used after going to neighbours
  while (tostation != nullptr){
    // bfs is done on the graph defined on the vertices as the station and the edges defined as the train routes
    // optimisation done is that the first step in the bfs is done directly to find the journey codes of the train which goes from source station to destination
    // then bfs is done on the immediate neighbour and since the train has only one way to move from a station (or stop at that station considered)
    
    // This makes the time complexity = O(total number of edges) = O(total number of trains between two trains)

    int jc = train->object->journeyCode; // storing the journey code
    int stat = tostation->object->adjacentStnIndex; // adjacent station index
    listOfObjects<StationConnectionInfo *> *toStat = adjacency[stat].toStations; // to stations list
    if (adjacency[stat].toStations != nullptr){ // if it is nullptr continue {
      while (toStat!=nullptr && destStnName != stnNameToIndex.getKeyAtIndex(stat)){
      // bfs is done to do find that station it next goes to then bfs goes on to that station instead of all the vertices 
      // this reduces the number of vertices you need to search and the requirement of queue as there is no shortest path required and the 
      // graph becomes essential a one child graph where each vertex has atmost one child.
      
        listOfObjects<TrainInfoPerStation *> *Train =  toStat->object->trains;
        while (Train != nullptr && Train->object->journeyCode != jc) Train = Train->next;
        if (Train==nullptr){ // train stops at that station
          toStat = toStat->next;
        }
        else if (Train->object->journeyCode == jc){ // if it is the train required
          stat = toStat->object->adjacentStnIndex;
          toStat  = adjacency[stat].toStations;
        }
      }
      if (destStnName == stnNameToIndex.getKeyAtIndex(stat)){ // if found then
        listOfObjects<int> *node = jcs;
        while (node != nullptr) {
          if (node->object ==  jc) break;
          node = node->next;
        }
        if (node == nullptr){
          listOfObjects<int> *Node = new listOfObjects<int>(jc); // copying the  journey code object
          Node->next = jcs;
          jcs = Node;
        }
      }
    }  // updating after a loop
    train = train->next;
    if (train==nullptr){
      tostation =tostation->next;
      if (tostation == nullptr) break;
      else train = tostation->object->trains;
    }
    
  }
  // printing the result
  if (jcs == nullptr){
    cout << "NO TRAINS AVAILABLE DIRECTLY"<< endl;    
    return;
  }
  if (jcs!=nullptr){
    listOfObjects<TrainInfoPerStation *> *to_be_printed = nullptr;
    listOfObjects<TrainInfoPerStation *> *list = stationInfo[index];
    while(jcs!= nullptr){
      list = stationInfo[index];
      while (list != nullptr){
        if (list->object->journeyCode == jcs->object){
          listOfObjects<TrainInfoPerStation *> *node = new listOfObjects<TrainInfoPerStation *> (list->object);
          node->next = to_be_printed;
          to_be_printed = node;
        }
        list=list->next;
      }
      jcs = jcs->next;
    }
    printStationInfo(to_be_printed);
  }
  return;
}
