#include "Heap.cpp"

using namespace std;

int main(){
    Heap<int> h;
    h.insert(10);
    h.insert(20);
    h.insert(40);
    h.delMax();
    h.delMax();
}