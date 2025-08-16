#ifndef KMP_CPP
#define KMP_CPP

#ifndef PLANNER_H
#include "planner.h"
#endif

using namespace std;

int *Planner::computeHMatrixForKMP(string pattern) {
    //cout << pattern << endl;
    if (pattern.size() == 0) return nullptr;// null case
    int *h=  new int[pattern.size()];
    h[0] = 0;// base case
    for (int i = 1; i < pattern.size(); i++)
    {
        int n = i;
        h[i] = 0;
        while (n >= 1)// while loop which decides the value of h matrix
        {
            int prevh = h[n - 1];
            if (prevh == 0){
                if (pattern[0]==pattern[i]) h[i] = 1;
                else h[i]=0;
                break;
            }
            else if (pattern[prevh] == pattern[i] )
            {
                h[i] = prevh + 1;
                break;
            }
            else
            {
                n = prevh;
            }
        }
    }
    for (int i=0;i<pattern.length();i++){ // making the prefix function into failure functions
        if (h[i] > 0 && i+1 < pattern.length() && (pattern[h[i]] == pattern[i+1]) ) h[i] = 0;
    }
    //cout << pattern << endl;
    return h;
}

int Planner::KMPMatch(string text, int *hMatrix, string pattern) { // regular kmp algorithm
    int index = 0;
    if (text.length() == 0 || pattern.length() == 0 || pattern.length() > text.length()) return -1; // base cases
    for (int i=0;i<text.length();i++){    
        while (index != 0 && toupper(pattern[index]) != toupper(text[i])) index = hMatrix[index-1]; // moving the through pattern
        if (toupper(text[i]) == toupper(pattern[index])) index++;// matching chars
        if (index ==pattern.length()) return i-pattern.length()+1;
    }
    return -1;
}

#endif
