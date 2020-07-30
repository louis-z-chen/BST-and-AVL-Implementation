#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <algorithm>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node{

public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;

};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL){
}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node(){

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem(){
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue(){
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent){
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left){
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right){
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value){
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree{

public:
    BinarySearchTree(); 
    virtual ~BinarySearchTree(); 
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); 
    virtual void remove(const Key& key); 
    void clear(); 
    bool isBalanced() const; 
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);

public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator{   
    
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; 
    Node<Key, Value> *getSmallestNode() const;  
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); 
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.
    static Node<Key, Value>* successor(Node<Key, Value>* current); 

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    static Node<Key, Value>* getSmallestNodeSubtree(Node<Key, Value>* current);
    static Node<Key, Value>* getLargestNodeSubtree(Node<Key, Value>* current);
    void clear_helper(Node<Key, Value>* current);
    int getHeight(Node<Key, Value>* current) const;
    bool isBalancedHelper(Node<Key, Value>* current) const;

    // node helper functions
    static bool isRoot(Node<Key, Value>* current);
    static bool isLeaf(Node<Key, Value>* current);
    static bool hasLeftChild(Node<Key, Value>* current);
    static bool hasRightChild(Node<Key, Value>* current);
    static bool hasChildren(Node<Key, Value>* current);
    static bool hasOneChild(Node<Key, Value>* current);
    static bool hasTwoChildren(Node<Key, Value>* current);
    static bool isLeftChild(Node<Key, Value>* current, Node<Key, Value>* parent);
    static bool isRightChild(Node<Key, Value>* current, Node<Key, Value>* parent);


protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr){
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(): current_(nullptr) {

}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const{
    return ((current_) == (rhs.current_));
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const{
    return ((current_) != (rhs.current_));
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++(){
    current_ = successor(current_);
    return *this;
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree(): root_(nullptr) {

}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree(){
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* if key is already in the tree, you should overwrite the current
* value with the updated value
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair){
    
    Key new_key = keyValuePair.first;
    Value new_value = keyValuePair.second;

    //start at the root
    Node<Key, Value>* current = root_;

    //if tree is empty
    if(current == nullptr){
        root_ = new Node<Key, Value>(new_key, new_value, current);
        return;
    }

    //walk the tree
    bool node_inserted = false;
    while(node_inserted == false){
        //get the current node's key and compare with the key 
        //that we're trying to insert
        Key current_key = current -> getKey();

        //if the current key is equal
        if(current_key == new_key){
            current -> setValue(new_value);
            node_inserted = true;
        }
        //if less than, go left
        else if(new_key < current_key){
            //if there's an empty spot
            if(hasLeftChild(current) == false){
                Node<Key, Value>* new_node = new Node<Key, Value>(new_key, new_value, current);
                current -> setLeft(new_node);
                node_inserted = true;
            }
            else{
                current = current -> getLeft();
            }
        }
        //if greater than, go right
        else if(new_key > current_key){
            //if there's an empty spot
            if(hasRightChild(current) == false){
                Node<Key, Value>* new_node = new Node<Key, Value>(new_key, new_value, current);
                current -> setRight(new_node);
                node_inserted = true;
            }
            else{
                current = current -> getRight();
            }
        }
    }
    return;
}

/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
* If the key is not already in the tree, this function will do nothing.
* If the node to be removed has two children, swap with its predecessor 
* (not its successor) in the BST removal algorithm. If the node to be 
* removed has exactly one child, you can promote the child. 
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key){
    
    Node<Key, Value>* node_to_remove = internalFind(key);

    //if key is not in tree, do nothing
    if(node_to_remove == nullptr){
        return;
    }

    //if node has no children, just delete it
    if(isLeaf(node_to_remove)){
        if(isRoot(node_to_remove)){
            root_ = nullptr;
        }
        else{
            //fix the parent's pointers
            Node<Key, Value>* parent = node_to_remove -> getParent();
            if(isLeftChild(node_to_remove, parent)){
                parent -> setLeft(nullptr);
            }
            else if(isRightChild(node_to_remove, parent)){
                parent -> setRight(nullptr);
            }
        }
        delete node_to_remove;
    }
    
    //if node has one child, promote the child
    else if(hasOneChild(node_to_remove)){

        Node<Key, Value>* child = nullptr;
        if(hasLeftChild(node_to_remove)){
            child = node_to_remove -> getLeft();
        }
        else{
            child = node_to_remove -> getRight();
        }

        //promote the child
        //if node_to_remove is root
        if(isRoot(node_to_remove)){
            child -> setParent(nullptr);
            root_ = child;
        }
        else{
            Node<Key, Value>* parent = node_to_remove -> getParent();

            if(isLeftChild(node_to_remove, parent)){
                parent -> setLeft(child);
            }
            else if(isRightChild(node_to_remove, parent)){
                parent -> setRight(child);
            }
            child -> setParent(parent);
        }
        delete node_to_remove;
    }
    //if node has two children, swap with predecessor
    else if(hasTwoChildren(node_to_remove)){

        //find predecessor
        Node<Key, Value>* pred = predecessor(node_to_remove);

        //swap node_to_remove with predecessor
        nodeSwap(node_to_remove, pred);

        //check if pred is now root
        if(isRoot(pred)){
            root_ = pred;
        }

        //node_to_remove is now where its predecesspr was
        //check if has any children now
        //it can only have none or one child

        //if no children
        if(isLeaf(node_to_remove)){
            Node<Key, Value>* parent = node_to_remove -> getParent();
            if(isLeftChild(node_to_remove, parent)){
                parent -> setLeft(nullptr);
            }
            else if(isRightChild(node_to_remove, parent)){
                parent -> setRight(nullptr);
            }
        }
        //if one child
        else if(hasOneChild(node_to_remove)){
            Node<Key, Value>* current_node = node_to_remove;
            Node<Key, Value>* current_parent = current_node -> getParent();
            Node<Key, Value>* current_child = nullptr;
            if(hasLeftChild(current_node)){
                current_child = current_node -> getLeft();
            }
            else{
                current_child = current_node -> getRight();
            }

            if(isLeftChild(current_node, current_parent)){
                current_parent -> setLeft(current_child);
            }
            else if(isRightChild(current_node, current_parent)){
                current_parent -> setRight(current_child);
            }
            current_child -> setParent(current_parent);
        }
        delete node_to_remove;
    }
    return;
}

template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current){
    
    Node<Key, Value>* temp =  nullptr;

    // check if it's a valid node
    if(current == nullptr){
        return temp;
    }
    // if left child exists, predecessor is the right most node 
    // of the left subtree
    if(hasLeftChild(current)){
        Node<Key, Value>* left_child = current -> getLeft();
        temp = getLargestNodeSubtree(left_child);
    }
    //else walk up the ancestor chain until you traverse 
    //the first right child pointer
    //if you get to the root without finding a node who
    //is a right child, there is no predecessor
    else{
        Node<Key, Value>* curr = current;
        Node<Key, Value>* parent = current -> getParent();
        while(parent != nullptr && parent -> getLeft() == curr){
            curr = parent;
            parent = curr -> getParent();
        }
        temp = parent;
    }
    return temp;
}

template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current){

    Node<Key, Value>* temp =  nullptr;

    // check if it's a valid node
    if(current == nullptr){
        return temp;
    }
    // if right child exists, successor is the left most node 
    // of the right subtree
    if(hasRightChild(current)){
        Node<Key, Value>* right_child = current -> getRight();
        temp = getSmallestNodeSubtree(right_child);
    }
    //else walk up the ancestor chain until you traverse 
    //the first left child pointer
    //if you get to the root without finding a node who
    //is a left child, there is no successor
    else{
        Node<Key, Value>* curr = current;
        Node<Key, Value>* parent = current -> getParent();
        while(parent != nullptr && parent -> getRight() == curr){
            curr = parent;
            parent = curr -> getParent();
        }
        temp = parent;
    }
    return temp;
}

/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear(){
    clear_helper(root_);
    root_ = nullptr;
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const{
    Node<Key, Value>* temp = getSmallestNodeSubtree(root_);
    return temp;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const{
    Node<Key, Value>* internal_find = nullptr;
    Node<Key, Value>* current = root_;
    bool found_node = false;

    if(current == nullptr){
        return internal_find;
    }

    while(found_node == false){
        Key current_key = current -> getKey();
        
        if(key == current_key){
            internal_find = current;
            found_node = true;
        }
        else if(key < current_key){
            if(hasLeftChild(current)){
                current = current -> getLeft();
            }
            else{
                found_node = true;
            }
        }
        else if(key > current_key){
            if(hasRightChild(current)){
                current = current -> getRight();
            }
            else{
                found_node = true;
            }
        }
    }
    return internal_find;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const{
    return isBalancedHelper(root_);
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2){

    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }

    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }

    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNodeSubtree(Node<Key, Value>* current){
    if(current == nullptr){
        return nullptr;
    }
    Node<Key, Value>* temp = current;
    while(hasLeftChild(temp)){
        temp = temp -> getLeft();
    }
    return temp;
}

template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getLargestNodeSubtree(Node<Key, Value>* current){
    Node<Key, Value>* temp = current;
    while(hasRightChild(temp)){
        temp = temp -> getRight();
    }
    return temp;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear_helper(Node<Key, Value>* current){

    if(current != nullptr){

        Node<Key, Value>* left_child = current -> getLeft();
        clear_helper(left_child);

        Node<Key, Value>* right_child = current -> getRight();
        clear_helper(right_child);

        delete current;
    }
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key, Value>* current) const{

    if(current == nullptr){
        return 0;
    }

    int left_height = getHeight(current -> getLeft());
    int right_height = getHeight(current -> getRight());
    int larger_height = std::max(left_height, right_height);
    int height = larger_height + 1;

    return height;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalancedHelper(Node<Key, Value>* current) const{

    bool temp = false; 

    if(current == nullptr){
        temp = true;
    }
    else{
        Node<Key, Value>* left_child = current -> getLeft();
        Node<Key, Value>* right_child = current -> getRight();

        int left_height = getHeight(left_child);
        int right_height = getHeight(right_child);

        int difference = abs(right_height - left_height);
        bool diff = false;
        if(difference <= 1){
            diff = true;
        }

        bool left_balance = isBalancedHelper(left_child);
        bool right_balance = isBalancedHelper(right_child);

        if(diff && left_balance && right_balance){
            temp = true;
        }
    }
    return temp;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isRoot(Node<Key, Value>* current){
    bool temp = false;
    if(current -> getParent() == nullptr){
        temp = true;
    }
    return temp;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isLeaf(Node<Key, Value>* current){
    bool left = hasLeftChild(current);
    bool right = hasRightChild(current);
    return (!left && !right);
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::hasLeftChild(Node<Key, Value>* current){
    bool temp = false;
    if(current -> getLeft() != nullptr){
        temp = true;
    }
    return temp;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::hasRightChild(Node<Key, Value>* current){
    bool temp = false;
    if(current -> getRight() != nullptr){
        temp = true;
    }
    return temp;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::hasChildren(Node<Key, Value>* current){
    bool left = hasLeftChild(current);
    bool right = hasRightChild(current);
    return (left || right);
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::hasOneChild(Node<Key, Value>* current){
    bool left = hasLeftChild(current);
    bool right = hasRightChild(current);
    bool left_and_not_right = (left && !right);
    bool right_and_not_left = (right && !left);
    return (left_and_not_right || right_and_not_left);
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::hasTwoChildren(Node<Key, Value>* current){
    bool left = hasLeftChild(current);
    bool right = hasRightChild(current);
    return (left && right);
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isLeftChild(Node<Key, Value>* current, 
                                                Node<Key, Value>* parent){
    bool temp = false;
    if(parent -> getLeft() == current){
        temp = true;
    }
    return temp;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isRightChild(Node<Key, Value>* current, 
                                                Node<Key, Value>* parent){
    bool temp = false;
    if(parent -> getRight() == current){
        temp = true;
    }
    return temp;
}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
