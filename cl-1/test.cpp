#include<iostream>

using namespace std;

void f(int i,int &n){
    if (i > 3) n++;
    for (int j=1;j<3;j++) f(i+j,n);
}

int main(){
    int n = 0;
    f(0,n);
    cout << n << endl;
}