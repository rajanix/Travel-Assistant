#ifndef TRIE_CPP
#define TRIE_CPP

#ifndef STD_HEADERS
#include "std_headers.h"
#endif

#ifndef DICTIONARY_H
#include "dictionary.h"
#endif

#ifndef TRIE_H
#include "Trie.h"
#endif

using namespace std;

Trie::Trie(Dictionary<int> *nameToIndex)
{
  nameDict = nameToIndex;
  root = new TrieNode();
  if (root == nullptr)
  {
    cout << "Memory allocation failure." << endl;
    exit(-1);
  }
}

Trie::~Trie()
{
  recursiveDelete(root);
}

void Trie::recursiveDelete(TrieNode *node)
{
  if (node == nullptr)
  {
    return;
  }
  else
  {
    for (int i = 0; i < NUM_CHARS; i++)
    {
      TrieNode *currNode = node->branch[i];
      recursiveDelete(currNode);
      node->branch[i] = nullptr;
    }
    if (node != root)
    {
      delete node;
    }
    return;
  }
}

bool Trie::insert(string wordInName, int indexOfName)
{
  // inserting the word into the trie
  TrieNode *currNode = root;
  int len = wordInName.length();
  string word;
  for (auto x : wordInName)
  {
    word.push_back(toupper(x));// converting the string into the uppercase
  }
  // loop to insert the word
  for (int i = 0; i <= len; i++)
  {
    if (i == len) // final node
    {
      if (currNode->indices == nullptr) // if the linked list is null
        currNode->indices = new listOfObjects<int>(indexOfName);
      else // if the linked list is not null
      {
        listOfObjects<int> *leaf = currNode->indices;// move till the end
        listOfObjects<int> *parent = nullptr;
        while (leaf != nullptr && nameDict->getKeyAtIndex(leaf->object) != wordInName)
        {
          parent = leaf;
          leaf = leaf->next;// moving across the node
        }
        if (leaf == nullptr){// if the word is not already found then insert it into the linked list.
          leaf = new listOfObjects<int>(indexOfName);
          parent ->next = leaf;// linking to the parent
        }
        else if (nameDict->getKeyAtIndex(leaf->object) == wordInName)
        {
          cout << "Insertion already done" << endl;
          return true;
        }
        // this case won't occur as it gets handled by the planner.cpp itself
      }
      currNode->isWord = true;
    } // moving across the trie
    else if (currNode->branch[word[i] - 'A'] == nullptr)
    {
      currNode->branch[word[i] - 'A'] = new TrieNode();// if the branch was not created at first.
      currNode->numNonNullBranches++; // the number of the null branches has to be increased
    }
    // moving incase if the branch was created already or created just now
    currNode = currNode->branch[word[i] - 'A'];//moving down the trie
  }
  return true;
}

bool Trie::del(string wordInName, string name)
{
  //cout << name<< " name of the string " << endl;
  TrieNode *currNode = root;
  TrieNode *lastBranchNode = root;// the node till which you have to delete in certain cases
  int lastBranchIndex = 0;
  int len = wordInName.length();// length of the string
  string word;
  for (auto x : wordInName) word.push_back(toupper(x));// converting the string to uppercase entirely
  wordInName = word;
  for (int i = 0; i < len; i++)
  {
    if (currNode->branch[wordInName[i] - 'A'] == nullptr) // word not found
    { // this case also doesn't occur case handled by the planner.cpp itself.
      cout << " Not Found " << wordInName << " in  " << name << endl;
      return false;
    }
    else
    {
      if (currNode->numNonNullBranches > 1 || currNode->isWord ) // to get the lastBranchNode and the lastBranchIndex
      {
        lastBranchNode = currNode;
        lastBranchIndex = i;
        //cout << wordInName[i] << " char in string "<< endl;
      }
      currNode = currNode->branch[wordInName[i] - 'A']; // moving to the next node
    }
  }
  if (!currNode->isWord) // this case doesn't occur (handled by the planner itself)
  { // word not found it is prefix mostly
    cout << " Not Found " << wordInName << " in  " << name << endl;
    return false;
  }
  listOfObjects<int> *leaf = currNode->indices;
  listOfObjects<int> *prev = nullptr;
  while (leaf != nullptr) // travels till it find the node
  {
    if (nameDict->getKeyAtIndex(leaf->object) == name) // finding the same
    {
      break;
    }
    prev = leaf; 
    leaf = leaf->next;// moving across the linked list
  }
  if (leaf == nullptr) // string not found case
  {// handled by the planner.cpp 
    cout << "Couldn't find " << wordInName << " in  " << name << endl;
    return false;
  }
  if (prev == nullptr) // deleting from the node from the linked list
  {
    currNode->indices = leaf->next;// link to direct to the node
  }
  else
  {
    prev->next = leaf->next;// link directly to the previous node
  }
  delete leaf;// delete  the leaf
  /*
    now if the current node has no indices then have to check for the branches 
    there is no branches then go to the last branch node and delete this branch
  */
  if (currNode->indices != nullptr)
    return true;
  else if (currNode->indices == nullptr)// the currNode->indices is empty
  {
    currNode->isWord = false;// then it is not a finish point
    if (currNode->numNonNullBranches > 0) return true; // if there are branches 
    lastBranchNode->numNonNullBranches--;// no branches 
    recursiveDelete(lastBranchNode->branch[wordInName[lastBranchIndex]-'A']);
    lastBranchNode->branch[wordInName[lastBranchIndex]-'A'] = nullptr;// this node will be left out.
    return true;
  }
  return false;
}

listOfObjects<string> *Trie::completions(string prefix)
{
  // find the completions of the prefix
  TrieNode *currNode = root;
  int len = prefix.length();
  listOfObjects<string> *currCompletions = nullptr;// intialisation
  for (int i = 0; i < prefix.length(); i++)
  {
    int letter = toupper(prefix[i]) - 'A';// it is the letter changed to uppercase
    if (currNode->branch[letter] == nullptr)// if the prefix is not found
      return nullptr;
    currNode = currNode->branch[letter];
  }// move till the branch and change it.
  if (!recursiveAddCompletions(currNode, currCompletions))// it takes care of the all the completions
    return nullptr;
  return currCompletions;
}

bool Trie::recursiveAddCompletions(TrieNode *node, listOfObjects<string>* &currCompletions)
{// recursively adds all the words with the prefix provided the node has same prefix.
  if (node->isWord)
  {
    if (!addCompletions(node, currCompletions))// add only if it is isWord is True
    {
      return false;
    }
  }
  TrieNode *currNode = node;
  for (int i = 0; i < NUM_CHARS; i++)
  {
    if (currNode->branch[i] != nullptr)
    {
      if (!recursiveAddCompletions(currNode->branch[i], currCompletions)) // checks all the branches
      {
        return false;
      }
    }
  }
  return true;
}

bool Trie::addCompletions(TrieNode *node, listOfObjects<string> * &currCompletions)
{
  listOfObjects<string> *stnames = currCompletions;
  listOfObjects<int> *list = node->indices;
  if (list == nullptr) // list shouldn't be empty
    return false;
  while (list != nullptr)
  {
    string st = nameDict->getKeyAtIndex(list->object);// get the string
    /*if (stnames != nullptr){
      listOfObjects<string> *sts = stnames;
      while (sts!= nullptr && sts->object != st) sts = sts->next;
      if (sts!= nullptr && sts->object == st) return true;
    }*/
    currCompletions = new listOfObjects<string> (st);// linking the new node in the head itself
    currCompletions->next = stnames;
    stnames = currCompletions;// linking the entire linked list
    list = list->next; // move to new node
  }
  //currCompletions = stnames;
  return true;
}

bool Trie::isPresentInCompletions(listOfObjects<string> *completions, string name)
{
  listOfObjects<string> *currCompletion = completions;
  bool isPresent = false;
  while (currCompletion != nullptr)
  {
    if (currCompletion->object == name)
    {
      isPresent = true;
      break;
    }
    currCompletion = currCompletion->next;
  }
  return isPresent;
}

void Trie::freeCompletionList(listOfObjects<string> *completions)
{
  listOfObjects<string> *currCompl = completions;
  while (currCompl != nullptr)
  {
    listOfObjects<string> *toDeleteCompl = currCompl;
    currCompl = currCompl->next;
    delete toDeleteCompl;
  }
  return;
}

bool Trie::isPresent(string wordInName)
{
  TrieNode *currNode = root;
  int len = wordInName.length();

  for (int i = 0; i < len; i++)
  {
    int letter = toupper(wordInName.at(i)) - 'A';

    if (currNode->branch[letter] == nullptr)
    {
      return false;
    }
    currNode = currNode->branch[letter];
  }

  return currNode->isWord;
}

// Edit version 1: Added printTrie and recursivePrintTrie functions
// to help in debugging

void Trie::printTrie()
{
  recursivePrintTrie(root, "", -1, false);
}

// Adapted from Adrian Schneider's code on StackOverflow
// Basically a variation of the function we've been using
// print binary search trees in previous labs

void Trie::recursivePrintTrie(TrieNode *node, const string &prefix, int branchId, bool siblingsToRight)
{
  if (node != nullptr)
  {
    cout << prefix;

    cout << ((branchId == 0) ? "|-" : (branchId == -1) ? " *"
                                                       : "|_")
         << (char)((branchId != -1) ? 'A' + branchId : '*');
    cout << ((branchId == 0) ? '-' : (branchId == -1) ? '*'
                                                      : '_');

    // If this trie node has isWord set to true, print "(isWord)" and
    // also the list of complete station names (accessed through indices
    // in nameDict) associated with this trie node.
    if (node->isWord)
    {
      cout << "(isWord)";
      listOfObjects<int> *currIndices = node->indices;
      while (currIndices != nullptr)
      {
        cout << endl
             << prefix << "     " << nameDict->getKeyAtIndex(currIndices->object);
        currIndices = currIndices->next;
      }
    }
    else
    {
      cout << "()";
    }
    cout << endl;
    for (int i = 0; i < NUM_CHARS; i++)
    {
      TrieNode *currNode = node->branch[i];
      bool childrenToRight = false;
      for (int j = i + 1; j < NUM_CHARS; j++)
      {
        if (node->branch[j] != nullptr)
        {
          childrenToRight = true;
          break;
        }
      }
      if (currNode != nullptr)
      {
        recursivePrintTrie(currNode, prefix + ((siblingsToRight) ? "│   " : "    "), i, childrenToRight);
      }
    }
  }
}
// End edit version 1

#endif
