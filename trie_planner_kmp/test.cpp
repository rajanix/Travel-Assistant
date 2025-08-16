#include <iostream>

using namespace std;

int *computeHMatrixForKMP(string pattern) {
    if (pattern.size() == 0) return nullptr;
    int *h=  new int[pattern.size()];
    h[0] = 0;
    for (int i = 1; i < pattern.size(); i++)
    {
        int n = i;
        h[i] = 0;
        while (n >= 1)
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
    for (int i=0;i<pattern.length();i++){
        if (h[i] > 0 && i+1 < pattern.length() && (pattern[h[i]] == pattern[i+1]) ) h[i] = 0;
    }
    return h;
}

int KMPMatch(string text, int *hMatrix, string pattern) {
  int index = 0;
  if (text.length() == 0 || pattern.length() == 0 || pattern.length() > text.length()) return -1;
  for (int i=0;i<text.length();i++){    
    cout << text[i] << " " << index << endl;
    while (index != 0 && toupper(pattern[index]) != toupper(text[i])) index = hMatrix[index-1];
    if (toupper(text[i]) == toupper(pattern[index])) index++;
    if (index ==pattern.length()) return i-pattern.length()+1;//&& index != hMatrix[index]
  }
  return -1;
    //cout << pattern << " " << text << endl;
    /*int index = -1;
    if (text.length() == 0 || pattern.length() == 0) return -1;
    for (int i=0;i<text.length();i++){    
        cout << text[i]<< " "<< index<< endl; 
        while (index != -1 && index != 0 && toupper(pattern[index+1]) != toupper(text[i])) {
            /*if (index == hMatrix[index]){
                break;
            }
            index = hMatrix[index-1];
        }
        if (toupper(text[i]) == toupper(pattern[index+1])) index++;
        if (index ==pattern.length()-1) return i-pattern.length()+1;
    }
    //cout << pattern << " " << text << endl;*/
    //return -1;
}

void my(int* n){
    n = new int(1);
}

void func(string pattern,string text){
    int *h = computeHMatrixForKMP(pattern);
    //for (int i = 0;i<pattern.length();i++)cout <<h[i]<< endl;
    int i=KMPMatch(text,h,pattern);
    //cout << i << endl;
    for (int j = i; j < pattern.length()+i; j++)
    {
        cout << text[j];
    }
    cout << endl;
}

int main()
{
    string pattern = "aabcaa";
    string text = "qrintprinhaabccaaprinupringprinaprinioprinjaabcaa";
    func("aabcaa","qrintprinhaabccaaprinupringprinaprinioprinjaabcaa");
    func("prinu","qrintprinhaabccaaprinupringprinaprinioprinjaabcaa");
    func("qr","qrintprinhaabccaaprinupringprinaprinioprinjaabcaa");
    func("","aepomgombpome");
    func("aefnoino","aef");
    func("iwnronf","");
    func("abaab","baababaabaab");
    /*int *p,*q;
    p = q;
    p = new int();
    *p = 7;
    cout << *q;
    int *n = nullptr ,p=0;
    n = &p;
    cout << *n << endl;
    my(n);
    cout << *n << endl;*/
}