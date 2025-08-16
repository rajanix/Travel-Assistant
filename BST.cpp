#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "BST.h"

#define PRE_ORDER 0
#define POST_ORDER 1
#define IN_ORDER 2
#define LOAD_FULL_BST 3
#define LOAD_LEFT_SUBTREE 4


// PLEASE FILL IN THE BODIES OF THE FOLLOWING FUNCTIONS

BST::BST(){
    root = nullptr;// nullptr to ensure uniformity and to avoid garbage values
}
BST::BST(TreeNode *root){ 
    this->root=root; // basic constructor.
}

bool BST::checkJCode(int JourneyCode){// it does preorder traversal and checks for the journey code.
    if (root == nullptr) return false;
    if (root->JourneyCode==JourneyCode) return true;
    else {
        BST L(root->left);
        BST R(root->right);
        return (L.checkJCode(JourneyCode) || R.checkJCode(JourneyCode) );// recursion is used.
    }
}
// Returns false, if given JourneyCode is already present in the BST
// Inserts the element and returns True otherwise
bool BST::insert(int JourneyCode, int price) { 
    // iterative procedure
    TreeNode *parent = nullptr;
    TreeNode *child = root;
    if (checkJCode(JourneyCode)) return false;// journey code check.
    if (root == nullptr) {// intial case 
        root = new TreeNode(JourneyCode,price,nullptr,nullptr,nullptr);
        return true;
    }
    while ( (child != nullptr) && (child->JourneyCode != JourneyCode) ){// traverse till you find a leaf 
        if (child->price >= price){ // go to left node 
            parent = child;
            child = child->left;
        }
        else if (child->price < price){ // go to right node
            parent = child;
            child = child->right;
        }
    }
    if (child == nullptr){// insert only if the u had found a leaf.
        TreeNode *new_node = new TreeNode(JourneyCode,price,nullptr,nullptr,parent);
        if (parent->price >= price) parent->left = new_node;
        else parent->right = new_node;
        return true;
    }    
    return false;
}

// Return True, if the ticket with given attributes is found, false otherwise
bool BST::find(int JourneyCode, int price) { 
    // recursion 
    if (root == nullptr) return false;
    if (root->price == price && root->JourneyCode == JourneyCode) return true;
    else if (root->price >= price) {// move to left node.
        BST leftSubTree(root->left);
        return leftSubTree.find(JourneyCode,price);
    }
    else if (root->price < price) {
        BST rightSubTree(root->right);// move to right node.
        return rightSubTree.find(JourneyCode,price);
    }
    return false;
}
	
// Returns false, if JourneyCode isn't present
// Deletes the corresponding entry and returns True otherwise
bool BST::remove(int JourneyCode, int price) { 
    if (root == nullptr) return false;
    if ((root->JourneyCode==JourneyCode) && (root->price==price) ){// node found
        if ( root->left == nullptr && root->right == nullptr){// no child case
            if (root->parent ==  nullptr) {
                root=nullptr;
                return true;
            }
            TreeNode *parent = root-> parent;
            if( parent->price >= price ) parent->left = nullptr; 
            else parent->right = nullptr;
            root = nullptr;
            return true;            
        }
        else if ( root->left ==nullptr ){// only right child case
            if (root->parent ==nullptr){
                root=root->right;// have to take care of the case when the root has no parent
                root->parent = nullptr;                
                return true;
            }
            TreeNode *parent = root->parent;
            if( parent->price >= price ) parent->left = root->right; 
            else parent->right = root->right;
            root->right->parent = parent;
            root =nullptr;
            return true;
        }   
        else if ( root->right ==nullptr ){// only left child case
            if (root->parent ==nullptr){
                root=root->left;// have to take care of the case when the root has no parent
                root->parent = nullptr;                
                return true;
            }
            TreeNode *parent = root->parent;
            if( parent->price >= price ) parent->left = root->left; 
            else parent->right = root->left;
            root->left->parent = parent;
            root =nullptr;
            return true;
        }
        else {// two child case
            TreeNode *child = root->left;
            while (child->right != nullptr){
                child = child->right;
            }
            root->JourneyCode=child->JourneyCode;//coping the predecessor into root.
            root->price = child->price;
            BST leftSubTree(root->left);
            return leftSubTree.remove(root->JourneyCode,root->price);// removing the copy of predecessor
        }
    }
    else if (root->price >= price) {// recursion
        BST leftSubTree(root->left);
        return leftSubTree.remove(JourneyCode,price);
    }
    else if (root->price < price) {
        BST rightSubTree(root->right);
        return rightSubTree.remove(JourneyCode,price);
    }
    return false;
}
	
// Functions that traverse the BST in various ways!
// type is one of PRE_ORDER, POST_ORDER or IN_ORDER
// Print on cout the journey codes at nodes of the BST in the same order as
// they are encountered in the traversal order
// Print one journey code per line

void BST::traverse(int typeOfTraversal) {
    if (typeOfTraversal==  PRE_ORDER){// pre order traversal
        if (root == nullptr) return;// node before children
        cout << root->JourneyCode <<"\n";
        BST leftSubTree(root->left);
        leftSubTree.traverse(PRE_ORDER);
        BST rightSubTree(root->right);
        rightSubTree.traverse(PRE_ORDER);
    }
    else if (typeOfTraversal==  POST_ORDER){// post order traversal
        if (root == nullptr) return;// children before node
        BST leftSubTree(root->left);
        leftSubTree.traverse(POST_ORDER);
        BST rightSubTree(root->right);
        rightSubTree.traverse(POST_ORDER);
        cout << root->JourneyCode<<"\n";
    }
    else if (typeOfTraversal==  IN_ORDER){// in order tranversal
        if (root == nullptr) return;// left child then node then right child.
        BST leftSubTree(root->left);
        leftSubTree.traverse(IN_ORDER);
        cout << root->JourneyCode<<"\n";
        BST rightSubTree(root->right);
        rightSubTree.traverse(IN_ORDER);
    }  
    return; 
}

// Returns the price of the cheapest journey
int BST::getMinimum() {// gets the minimum price of all the nodes in the tree
    TreeNode *child = root;
    while (child ->left != nullptr){
        child =child->left;
    }
    return child->price; 
}

// Part II

// Returns the count of journeys that are at least as expensive
// as lowerPriceBound and at most as expensive as upperPriceBound,
// both bounds included.
int BST::countJourneysInPriceBound(int lowerPriceBound, int upperPriceBound) {
    if (root == nullptr) return 0;// left child then node then right child.
    BST leftSubTree(root->left);// it does inorder traversal to get the number of the prices less than or equal to
    int n = leftSubTree.countJourneysInPriceBound(lowerPriceBound,upperPriceBound);// upperPriceBound and more than or equal to lowerPriceBound.
    if (root->price >= lowerPriceBound && root->price <= upperPriceBound) n++;
    BST rightSubTree(root->right);
    n+= rightSubTree.countJourneysInPriceBound(lowerPriceBound,upperPriceBound);
    return n;
}

// Store the current tree in a file called filename.  You should
// store using a scheme that uses no more disk space than that
// required to store all elements in the BST, plus perhaps a small
// constant times the number of elements.  You should also store
// in a manner such that your implementation of customLoad is as
// memory and time efficient as possible.
void BST::customStore(string filename) { 
    ofstream ofile;// storing in the preorder tranversal method.
    ofile.open(filename,ios::app);
    if (root == nullptr) return;// no tree 
    ofile <<root->JourneyCode<<" "<<root->price<<" ";    // storing both the JourneyCode and price.
    if  ( (root->left == nullptr) && (root->right == nullptr) ) {// stores the info about children
        ofile<<"NN"<<endl;
    }// N denotes no child.
    else if (root->left==nullptr){
        ofile<<"NR"<<endl;
    }// R denotes presence of right child.
    else if (root->right==nullptr){
        ofile<<"LN"<<endl;
    }// L denotes presence of left child. 
    else{
        ofile<<"LR"<<endl;
    }
    BST L(root->left),R(root->right);
    L.customStore(filename);// recursion.
    R.customStore(filename);
    return; 
}
	
// While loading a tree from a file written by customStore, we
// can have one of two modes of loading.  We could either ask
// for the entire tree to be loaded or only the left subtree of
// the root (if it exists) to be loaded.  The loadMode flag
// takes one of two values LOAD_FULL_BST or LOAD_LEFT_SUBTREE,
// that are #defined in BST.cpp.
// If loadMode is LOAD_FULL_BST, customLoad should load the
// entire BST that was stored in filename using customStore.
// If loadMode is LOAD_LEFT_SUBTREE, customLoad should load
// only the left subtree of the root of the BST that was
// stored in filename using customStore.
// Your implementation of customLoad should be efficient --
// its time and space complexity should be linear in the size
// of the tree that is being loaded.  So, for example, if the
// left sub-tree of the root is small compared to the right
// sub-tree, customLoad's time and space complexity should
// be small even though the entire BST may be very large.
 
void BST::customLoad (int flag, string filename) { 
    ifstream infile;// it gets the info from the file and  pushes that into a vector.
    infile.open(filename);
    int jc,price;string child;
    vector<TreeNode* > Nodes;
    TreeNode* Dummy = new TreeNode(-1,-1);  // dummy node used to denote presence of left and right node.  
    while (true){// it is safe to use the dummy node since , both the journey code and price is going to postive numbers.
        infile>>jc>>price>>child;
        //cout << jc << " "<< price << " "<< child << endl;
        if (infile.eof()) break;// breaking it when it reaches the end of the file.
        TreeNode *node = new TreeNode(jc,price);
        if(child[0]=='N')node->left=nullptr;
        else node->left=Dummy;
        if(child[1]=='N')node->right=nullptr;
        else node->right=Dummy;
        Nodes.push_back(node);
    }
    if (flag == LOAD_FULL_BST) load(Nodes);// calling the appropriate function.
    if (flag==LOAD_LEFT_SUBTREE) load_left(Nodes);
    ofstream ofile;
    ofile.open(filename,ios::trunc); // to overwrite the file and make it empty.
    ofile.close();
}

void BST::load(vector<TreeNode* > nodes){    // iterative method 
    root = nodes[0];// it load the root first.
    TreeNode *curr = root;
    if (nodes.size()==1) return;// incase of only root.
    TreeNode *child = new TreeNode(nodes[1]->JourneyCode,nodes[1]->price,nodes[1]->left,nodes[1]->right,root); // the next node.    
    // copying the entire node but the parent is changed.
    if (nodes.size()==2) return;// incase of 2 nodes.
    vector<TreeNode* > unfilled;// to store nodes that have their right node unfilled if it is not nullptr.
    //why right node ? it is because since the preorder traversal is stored . the left child might be filled first if not then right child gets filled 
    // So,the node can be unfilled only if the right node is empty.
    if (root->right!=nullptr && root->right-> price == -1) unfilled.push_back(root);// pushing back the unfilled nodes.
    if (child->right!= nullptr && child->right-> price == -1) unfilled.push_back(child); 
    if (root->price >= child->price) root->left=child;// assign the child pointer.
    else root->right = child;
    for (int i=2;i<nodes.size();i++){// going through the entire vector.
    // child is the last added node. and curr is the parent of the child.
        if (child->left == nullptr && child->right == nullptr){
            // case-1 :if the child has no child then move to the last unfilled space.
            child=unfilled[unfilled.size()-1];
            unfilled.pop_back();// removing that element from the vector.
            curr=(child->parent!=nullptr) ? child->parent : child;   // choosing the parent in case if the parent of the child.
            child->right=nodes[i];//placing the child.
            curr=child;// changing the curr
            child=child->right;// moving the child.              
        }
        else if (child->price >= nodes[i]->price && child->left != nullptr) {
            //case-2 :for placing the left child.
            child->left = nodes[i];//placing the 
            curr=child;
            // std::find is to find if the node is already present in the vector.
            if (curr->right!=nullptr && curr->right->price==-1 && std::find(unfilled.begin(),unfilled.end(),curr) == unfilled.end() ) unfilled.push_back(curr);
            child=child->left;
        }
        else if (child->price < nodes[i]->price && child->right != nullptr) {
            //case-3 :for placing the right child. 
            child->right= nodes[i];
            curr=child;
            // removing the nodes in the vector unfilled.
            std::remove(unfilled.begin(),unfilled.end(),curr);
            child=child->right;
        }
        child->parent=curr;// assigning the parent to the child.
    }
}

void BST::load_left(vector<TreeNode* > nodes){    
    root = nodes[0];
    TreeNode *curr = root;
    if (nodes.size()==1) {// if number of nodes is 1 the left subtree is not there. 
        root=nullptr;
        return;
    }
    TreeNode *child = new TreeNode(nodes[1]->JourneyCode,nodes[1]->price,nodes[1]->left,nodes[1]->right,root);     
    if (nodes.size()==2) {//if the second node is left child of the first one,then last node is the root .
        root= ( (root->left != nullptr ) ? root->left : nullptr );
        return;
    } 
    vector<TreeNode* > unfilled;// same logic as before function.
    if (root->right!=nullptr && root->right-> price == -1) unfilled.push_back(root);
    if (child->right!= nullptr && child->right-> price == -1) unfilled.push_back(child); 
    if (root->price >= child->price) root->left=child;
    else root->right = child;
    for (int i=2;i<nodes.size();i++){
        if (nodes[i]->price > root->price) {
            root = root->left;// this is done to stop the for loop and change the root to root of the left subtree.
            return;
        }
        if (child->left == nullptr && child->right == nullptr){// for leafs
            child=unfilled[unfilled.size()-1];
            unfilled.pop_back();
            curr=(child->parent!=nullptr) ? child->parent : child;   
            child->right=nodes[i];
            curr=child;
            child=child->right;         
        }
        else if (child->price >= nodes[i]->price && child->left != nullptr) {//for left child.
            child->left = nodes[i];
            curr=child;
            if (curr->right!=nullptr && curr->right->price==-1 && std::find(unfilled.begin(),unfilled.end(),curr) == unfilled.end() ) unfilled.push_back(curr);
            child=child->left;
        }
        else if (child->price < nodes[i]->price && child->right != nullptr) {// for right child.
            child->right= nodes[i];
            curr=child;
            std::remove(unfilled.begin(),unfilled.end(),curr);
            child=child->right;
        }
        child->parent=curr;// assigning the parent.
    }
}

// ************************************************************
// DO NOT CHANGE ANYTHING BELOW THIS LINE

// Adapted from Adrian Schneider's code on StackOverflow
void BST::printBST(const string& prefix, bool isLeft=false)
{
    if( root != nullptr )
    {
        cout << prefix;

        cout << (isLeft ? "|--" : "|__" );

        // print the value of the node
        cout << root->JourneyCode << endl;
        TreeNode *curr = root;
        root = root->left;
        // enter the next tree level - left and right branch
        printBST( prefix + (isLeft ? "│   " : "    "), true);
        root = curr->right;
        printBST( prefix + (isLeft ? "│   " : "    "), false);
        root = curr;
    }
}

void BST::getBST(const string& prefix,  bool isLeft=false)
{
    if( root != nullptr )
    {
        result.push_back(prefix);

        result.push_back(isLeft ? "|--" : "|__" );

        // print the value of the node
        result.push_back(to_string(root->JourneyCode) + "\n");
        TreeNode *curr = root;
        root = root->left;
        // enter the next tree level - left and right branch
        getBST( prefix + (isLeft ? "│   " : "    "), true);
        root = curr->right;
        getBST( prefix + (isLeft ? "│   " : "    "), false);
        root = curr;
    }
}

void BST::clearResult(){
    result.clear();
}

vector<string> BST::getResult(){
    return result;
}
