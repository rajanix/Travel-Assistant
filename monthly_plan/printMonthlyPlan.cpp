#ifndef PRINT_MONTHLY_PLAN_CPP
#define PRINT_MONTHLY_PLAN_CPP

#ifndef STD_HEADERS_H
#include "std_headers.h"
#endif

using namespace std;

class TrainWithJc{
  public:
    int journeyCode;
    int fromStationIndex;
    int toStationIndex;
    TrainInfoPerStation* from;
    TrainInfoPerStation* to;

    int cost() {
      int hrs = ((to->arrTime/100)- (from->depTime/100));
      int mins ;
      if (to->arrTime%100 == 0 && from->depTime%100 != 0){
        hrs -- ;mins = 60 - from->depTime%100;
      }
      else {
        mins = to->arrTime%100 - from->depTime%100;
      }
      if (from->depTime > to ->arrTime) return DIRECT_JOURNEY_RATE*((24*60)+(hrs*60)+mins) ;
      else return DIRECT_JOURNEY_RATE*((hrs*60)+mins);
    } 

    bool operator==(const TrainWithJc &object){
      return (journeyCode == object.journeyCode);
    }

    bool operator<(TrainWithJc &object){
      return (cost() < (object.cost()) );
    }
};

class sortedset{
  listOfObjects<TrainWithJc> *head;

  void insert(TrainWithJc train){
    listOfObjects<TrainWithJc > *node = head;
    while (node!=nullptr) {
      if (train < node->object ){
        listOfObjects<TrainWithJc> *newnode = new listOfObjects<TrainWithJc>(train);
        newnode->prev = node->prev;
        newnode->next = node;
        node->prev = newnode;
        break;
      }
      node = node->next;
    }
  } 

  TrainWithJc remove(){
    TrainWithJc returnobject = head->object;
    listOfObjects<TrainWithJc> *node = head ;
    delete node;
    head = head->next;
    return returnobject;
  }
};
  

int cost(TrainInfoPerStation *from,TrainInfoPerStation *to) {
      int hrs = ((to->arrTime/100)- (from->depTime/100));
      int mins ;
      if (to->arrTime%100 == 0 && from->depTime%100 != 0){
        hrs -- ;mins = 60 - from->depTime%100;
      }
      else {
        mins = to->arrTime%100 - from->depTime%100;
      }
      if (from->depTime > to ->arrTime) return DIRECT_JOURNEY_RATE*((24*60)+(hrs*60)+mins) ;
      else return DIRECT_JOURNEY_RATE*((hrs*60)+mins);
    }


void Planner::printMonthlyPlan(string srcStnName, listOfObjects<string> * destStnNameList)
{
  int srcindex = stnNameToIndex.get(srcStnName)->value;
  // cout << "Station Info" << endl;
  // printStationInfo(stationInfo[srcindex]);
  // cout << "fromstations" << endl;
  // listOfObjects<StationConnectionInfo *> *fromtrain = adjacency[srcindex].fromStations;
  // while (fromtrain != nullptr){
  //   printStationInfo(fromtrain ->object->trains);
  //   fromtrain = fromtrain->next;
  // }
  // cout << "tostations" << endl;
  // fromtrain = adjacency[srcindex].toStations;
  // while (fromtrain != nullptr){
  //   printStationInfo(fromtrain ->object->trains);
  //   fromtrain = fromtrain->next;
  // }
  // cout << "fromstations" << endl;
  // int destIndex = stnNameToIndex.get(destStnNameList->object)->value;
  // fromtrain = adjacency[destIndex].fromStations;
  // while (fromtrain != nullptr){
  //   printStationInfo(fromtrain ->object->trains);
  //   fromtrain = fromtrain->next;
  // }
  // return;
  listOfObjects<int> *sourceStat = nullptr;
  int number = 0;
  while (destStnNameList != nullptr){
    number ++;
    int destIndex = stnNameToIndex.get(destStnNameList->object)->value;
    if (sourceStat == nullptr){
      sourceStat=  new listOfObjects<int>(destIndex);
    }
    else {
      sourceStat->next = new listOfObjects<int>(destIndex);
      sourceStat = sourceStat->next;
    }
  }

  listOfObjects<int> *Jcs = nullptr; // MST 
  int found = 0;
  bool previous = false;
  int index = srcindex;
  while (found != number){
    listOfObjects<StationConnectionInfo *> *fromtrain = adjacency[index].toStations;
    int minJc = -1,min_value = 0; 
    while (fromtrain != nullptr){
      int adj = fromtrain->object->adjacentStnIndex;
      listOfObjects<StationConnectionInfo *>*totrains =  adjacency[adj].fromStations;
      while (totrains!=nullptr && totrains->object->adjacentStnIndex !=index) totrains = totrains->next;
      listOfObjects<TrainInfoPerStation *> *from = fromtrain->object->trains;
      listOfObjects<TrainInfoPerStation *> *to = totrains->object->trains;
      while (to != nullptr && to->object->journeyCode != from->object->journeyCode) to = to->next;
      int value = cost(from->object,to->object);
      if (value < min_value){
        min_value = value;
        minJc = from->object->journeyCode;
      }
      fromtrain = fromtrain->next;
    }
  }
}

#endif
