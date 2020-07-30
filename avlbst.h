#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>{

public:

    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:

    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0){

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode(){

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance){
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff){
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>{

public:

    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO

protected:

    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key,Value>* node);
    void zig_zig(AVLNode<Key,Value>* node);
    void zig_zag(AVLNode<Key,Value>* node);
    void insert_fix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
    void remove_fix(AVLNode<Key,Value>* node, char diff);
    static AVLNode<Key,Value>* get_taller_child(AVLNode<Key,Value>* current);
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item){
    
    Key new_key = new_item.first;
    Value new_value = new_item.second;

    //start at the root
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key,Value>*>(this -> root_);

    //if tree is empty
    if(current == nullptr){
        AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_key, new_value, current);
        new_node -> setBalance(0);
        BinarySearchTree<Key,Value>::root_ = new_node;
        return;
    }

    bool node_inserted = false;
    bool left_child = false;
    bool right_child = false;

    //walk the tree
    while(node_inserted == false){
        //get the current node's key and compare with the key 
        //that we're trying to insert
        Key current_key = current -> getKey();

        //if the current key is equal
        if(current_key == new_key){
            current -> setValue(new_value);
            node_inserted = true;
            return;
        }
        //if less than, go let
        else if(new_key < current_key){
            //if there's an empty spot
            if(BinarySearchTree<Key,Value>::hasLeftChild(current) == false){
                AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_key, new_value, current);
                new_node -> setBalance(0);
                current -> setLeft(new_node);
                node_inserted = true;
                left_child = true;
            }
            else{
                current = current -> getLeft();
            }
        }
        //if greater than, go right
        else if(new_key > current_key){
            //if there's an empty spot
            if(BinarySearchTree<Key,Value>::hasRightChild(current) == false){
                AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_key, new_value, current);
                new_node -> setBalance(0);
                current -> setRight(new_node);
                node_inserted = true;
                right_child = true;
            }
            else{
                current = current -> getRight();
            }
        }
    }

    //update ancestor's balances
    //in the while loop above, I use current to find the parent so 
    //current is still a pointer to the inserted node's parent
    AVLNode<Key, Value>* parent = current;
    char parent_balance = parent -> getBalance();

    if(parent_balance == -1 || parent_balance == 1){
        parent -> setBalance(0);
    }
    else if(parent_balance == 0){

        AVLNode<Key, Value>* inserted_node = nullptr;
        if(left_child){
            inserted_node = parent -> getLeft();
        }
        else if(right_child){
            inserted_node = parent -> getRight();
        }

        if(BinarySearchTree<Key, Value>::isLeftChild(inserted_node,parent)){
            parent -> setBalance(-1);
        }
        else if(BinarySearchTree<Key, Value>::isRightChild(inserted_node,parent)){
            parent -> setBalance(1);
        }
        insert_fix(parent, inserted_node);
    }
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* parent, 
                                        AVLNode<Key,Value>* node){

    //if p is null or parent(p) is null, return
    if(parent == nullptr || BinarySearchTree<Key, Value>::isRoot(parent)){
        return;
    }

    AVLNode<Key,Value>* grandparent = parent -> getParent();

    if(BinarySearchTree<Key, Value>::isLeftChild(parent, grandparent)){

        grandparent -> updateBalance(-1);
        char g_balance = grandparent -> getBalance();

        //Case 1
        if(g_balance == 0){
            return;
        }
        //Case 2
        else if(g_balance == -1){
            insert_fix(grandparent, parent);
        }
        //Case 3
        else if(g_balance == -2){
            //if zig-zig
            if(BinarySearchTree<Key, Value>::isLeftChild(node, parent)){
                rotateRight(grandparent);
                grandparent -> setBalance(0);
                parent -> setBalance(0);
            }
            //if zig-zag
            else{
                rotateLeft(parent);
                rotateRight(grandparent);
                char n_balance = node -> getBalance();

                //case 3a
                if(n_balance == -1){
                    grandparent -> setBalance(1);
                    parent -> setBalance(0);
                    node -> setBalance(0);
                }
                //case 3b
                else if(n_balance == 0){
                    grandparent -> setBalance(0);
                    parent -> setBalance(0);
                    node -> setBalance(0);
                }
                //case 3c
                else if(n_balance == 1){
                    grandparent -> setBalance(0);
                    parent -> setBalance(-1);
                    node -> setBalance(0);
                }
            }
        }
    }
    else if(BinarySearchTree<Key, Value>::isRightChild(parent, grandparent)){

        grandparent -> updateBalance(1);
        char g_balance = grandparent -> getBalance();

        //Case 1
        if(g_balance == 0){
            return;
        }
        //Case 2
        else if(g_balance == 1){
            insert_fix(grandparent, parent);
        }
        //Case 3
        else if(g_balance == 2){
            //if zig-zig
            if(BinarySearchTree<Key, Value>::isRightChild(node, parent)){
                rotateLeft(grandparent);
                grandparent -> setBalance(0);
                parent -> setBalance(0);
            }
            //if zig-zag
            else{
                rotateRight(parent);
                rotateLeft(grandparent);
                char n_balance = node -> getBalance();

                //case 3a
                if(n_balance == 1){
                    grandparent -> setBalance(-1);
                    parent -> setBalance(0);
                    node -> setBalance(0);
                }
                //case 3b
                else if(n_balance == 0){
                    grandparent -> setBalance(0);
                    parent -> setBalance(0);
                    node -> setBalance(0);
                }
                //case 3c
                else if(n_balance == -1){
                    grandparent -> setBalance(0);
                    parent -> setBalance(+1);
                    node -> setBalance(0);
                }
            }
        }
    }
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key){
    
    //find the node to remove
    Node<Key, Value>* found_node = BinarySearchTree<Key,Value>::internalFind(key);

    //cast the found node to an AVLNode
    //did this in one line before but this way is easier
    //for me to read and understand
    AVLNode<Key, Value>* node_to_remove = static_cast<AVLNode<Key,Value>*>(found_node);

    //if node doesn't exist
    if(node_to_remove == nullptr){
        return;
    }

    AVLNode<Key, Value>* parent = node_to_remove -> getParent();
    char diff = 0;
    //calculate balance changes
    //calculating the balance change has to be before
    //you delete the node
    if(BinarySearchTree<Key,Value>::isRoot(node_to_remove) == false){
        if(BinarySearchTree<Key,Value>::isLeftChild(node_to_remove, parent)){
            diff = 1;
        }
        else if(BinarySearchTree<Key,Value>::isRightChild(node_to_remove, parent)){
            diff = -1;
        }
    }

    //if node has no children
    if(BinarySearchTree<Key,Value>::isLeaf(node_to_remove)){
        //fix where root points
        if(BinarySearchTree<Key,Value>::isRoot(node_to_remove)){
            BinarySearchTree<Key,Value>::root_ = nullptr;
        }
        else{
            //fix the parent's pointers
            if(BinarySearchTree<Key,Value>::isLeftChild(node_to_remove, parent)){
                parent -> setLeft(nullptr);
            }
            else if(BinarySearchTree<Key,Value>::isRightChild(node_to_remove, parent)){
                parent -> setRight(nullptr);
            }
        }
    }

    //if node has one child
    else if(BinarySearchTree<Key,Value>::hasOneChild(node_to_remove)){

        //get pointer to child
        Node<Key, Value>* child = nullptr;
        if(BinarySearchTree<Key,Value>::hasLeftChild(node_to_remove)){
            child = node_to_remove -> getLeft();
        }
        else{
            child = node_to_remove -> getRight();
        }

        //promote the child
        //if node_to_remove is root
        if(BinarySearchTree<Key,Value>::isRoot(node_to_remove)){
            child -> setParent(parent);
            BinarySearchTree<Key,Value>::root_ = child;
        }
        else{
            //fix the parent's pointers
            if(BinarySearchTree<Key,Value>::isLeftChild(node_to_remove, parent)){
                parent -> setLeft(child);
            }
            else if(BinarySearchTree<Key,Value>::isRightChild(node_to_remove, parent)){
                parent -> setRight(child);
            }
            child -> setParent(parent);
        }
    }

    //if node has two children
    else if(BinarySearchTree<Key,Value>::hasTwoChildren(node_to_remove)){
        //find predecessor
        Node<Key, Value>* find = BinarySearchTree<Key,Value>::predecessor(node_to_remove);
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key,Value>*>(find);

        //swap node_to_remove with predecessor
        nodeSwap(node_to_remove, pred);

        //check if pred is now root
        if(BinarySearchTree<Key,Value>::isRoot(pred)){
            BinarySearchTree<Key,Value>::root_ = pred;
        }

        //node_to_remove is now where its predecesspr was
        //check if has any children now
        //it can only have none or one child

        //update parent
        parent = node_to_remove -> getParent();

        //update balances
        if(BinarySearchTree<Key,Value>::isLeftChild(node_to_remove, parent)){
            diff = 1;
        }
        else if(BinarySearchTree<Key,Value>::isRightChild(node_to_remove, parent)){
            diff = -1;
        }

        //if no children
        if(BinarySearchTree<Key,Value>::isLeaf(node_to_remove)){
            AVLNode <Key,Value>* new_parent = node_to_remove -> getParent();
            if(BinarySearchTree<Key,Value>::isLeftChild(node_to_remove, new_parent)){
                new_parent -> setLeft(nullptr);
            }
            else if(BinarySearchTree<Key,Value>::isRightChild(node_to_remove, new_parent)){
                new_parent -> setRight(nullptr);
            }
        }
        //if one child
        else if(BinarySearchTree<Key,Value>::hasOneChild(node_to_remove)){
            AVLNode<Key, Value>* current_node = node_to_remove;
            AVLNode<Key, Value>* current_parent = current_node -> getParent();
            AVLNode<Key, Value>* current_child = nullptr;
            if(BinarySearchTree<Key,Value>::hasLeftChild(current_node)){
                current_child = current_node -> getLeft();
            }
            else{
                current_child = current_node -> getRight();
            }

            if(BinarySearchTree<Key,Value>::isLeftChild(current_node, current_parent)){
                current_parent -> setLeft(current_child);
            }
            else if(BinarySearchTree<Key,Value>::isRightChild(current_node, current_parent)){
                current_parent -> setRight(current_child);
            }
            current_child -> setParent(current_parent);
        }
    }

    delete node_to_remove;

    //patch the balances of the tree
    remove_fix(parent, diff);
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key,Value>* node, char diff){

    //if n is null, return
    if(node == nullptr){
        return;
    }
    //Compute next recursive call's arguments before altering the tree
    AVLNode<Key,Value>* parent = node -> getParent();
    char next_diff = 0;
    if(BinarySearchTree<Key,Value>::isRoot(node) == false){
        if(BinarySearchTree<Key,Value>::isLeftChild(node, parent)){
            next_diff = 1;
        }
        else if(BinarySearchTree<Key,Value>::isRightChild(node, parent)){
            next_diff = -1;
        }
    }

    //find which case for remove fix
    char n_balance = node -> getBalance();
    char new_balance = n_balance + diff;

    if(diff == -1){

        //Case 1
        if(new_balance == -2){

            AVLNode<Key,Value>* child = node -> getLeft();
            char child_balance;
            if(child != nullptr){
                child_balance = child -> getBalance();
            }

            //Case 1a
            if(child_balance == -1){
                rotateRight(node);
                node -> setBalance(0);
                child -> setBalance(0);
                remove_fix(parent, next_diff);
            }

            //Case 1b
            else if(child_balance == 0){
                rotateRight(node);
                node -> setBalance(-1);
                child -> setBalance(+1);
            }

            //Case 1c
            else if(child_balance == 1){
                AVLNode<Key,Value>* grandchild = child -> getRight();
                char grandchild_balance;
                if(grandchild != nullptr){  
                    grandchild_balance = grandchild -> getBalance();
                }

                rotateLeft(child);
                rotateRight(node);

                if(grandchild_balance == 1){
                    node -> setBalance(0);
                    child -> setBalance(-1);
                    grandchild -> setBalance(0);
                }
                else if(grandchild_balance == 0){
                    node -> setBalance(0);
                    child -> setBalance(0);
                    grandchild -> setBalance(0);
                }
                else if(grandchild_balance == -1){
                    node -> setBalance(1);
                    child -> setBalance(0);
                    grandchild -> setBalance(0);
                }
                remove_fix(parent, next_diff);
            }
        }

        //Case 2
        if(new_balance == -1){
            node -> setBalance(-1);
        }

        //Case 3
        if(new_balance == 0){
            node -> setBalance(0);
            remove_fix(parent, next_diff);
        }
    }
    
    else if(diff == 1){
        //Case 1
        if(new_balance == 2){

            AVLNode<Key,Value>* child = node -> getRight();
            char child_balance;
            if(child != nullptr){
                child_balance = child -> getBalance();
            }

            //Case 1a
            if(child_balance == 1){
                rotateLeft(node);
                node -> setBalance(0);
                child -> setBalance(0);
                remove_fix(parent, next_diff);
            }

            //Case 1b
            else if(child_balance == 0){
                rotateLeft(node);
                node -> setBalance(1);
                child -> setBalance(-1);
            }

            //Case 1c
            else if(child_balance == -1){
                AVLNode<Key,Value>* grandchild = child -> getLeft();
                char grandchild_balance;
                if(grandchild != nullptr){  
                    grandchild_balance = grandchild -> getBalance();
                }
 
                rotateRight(child);
                rotateLeft(node);

                if(grandchild_balance == -1){
                    node -> setBalance(0);
                    child -> setBalance(1);
                    grandchild -> setBalance(0);
                }
                else if(grandchild_balance == 0){
                    node -> setBalance(0);
                    child -> setBalance(0);
                    grandchild -> setBalance(0);
                }
                else if(grandchild_balance == 1){
                    node -> setBalance(-1);
                    child -> setBalance(0);
                    grandchild -> setBalance(0);
                }
                remove_fix(parent, next_diff);
            }
        }

        //Case 2
        if(new_balance == 1){
            node -> setBalance(1);
        }

        //Case 3
        if(new_balance == 0){
            node -> setBalance(0);
            remove_fix(parent, next_diff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2){
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){

    if(BinarySearchTree<Key,Value>::isLeaf(node)){
        return;
    }
    if(BinarySearchTree<Key,Value>::hasRightChild(node) == false){
        return;
    }

    //get the pointers to the node to push down, its parent, its child
    //and its child's right child
    AVLNode<Key,Value>* current = node;
    AVLNode<Key,Value>* parent = current -> getParent();
    AVLNode<Key,Value>* child = current -> getRight();
    AVLNode<Key,Value>* childs_leftchild = child -> getLeft();

    //6 pointers to change in total
    //first pair is the original parent and the child 
    //of the current node
    //if current is the root, the parent is nullptr
    if(BinarySearchTree<Key,Value>::isRoot(current) == false){
        if(BinarySearchTree<Key,Value>::isLeftChild(current,parent)){
            parent -> setLeft(child);
        }
        else if(BinarySearchTree<Key,Value>::isRightChild(current,parent)){
            parent -> setRight(child);
        }
    }
    else{
        BinarySearchTree<Key,Value>::root_ = child;
    }
    child -> setParent(parent);

    //second pair of pointers between current and its child
    child -> setLeft(current);
    current -> setParent(child);

    //third pair is between current and current's child's original child
    current -> setRight(childs_leftchild);
    if(childs_leftchild != nullptr){
        childs_leftchild -> setParent(current);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node){

    if(BinarySearchTree<Key,Value>::isLeaf(node)){
        return;
    }
    if(BinarySearchTree<Key,Value>::hasLeftChild(node) == false){
        return;
    }

    //get the pointers to the node to push down, its parent, its child
    //and its child's left child
    AVLNode<Key,Value>* current = node;
    AVLNode<Key,Value>* parent = current -> getParent();
    AVLNode<Key,Value>* child = current -> getLeft();
    AVLNode<Key,Value>* childs_rightchild = child -> getRight();

    //6 pointers to change in total
    //first pair is the original parent and the child 
    //of the current node
    //if current is the root, the parent is nullptr
    if(BinarySearchTree<Key,Value>::isRoot(current) == false){
        if(BinarySearchTree<Key,Value>::isLeftChild(current,parent)){
            parent -> setLeft(child);
        }
        else if(BinarySearchTree<Key,Value>::isRightChild(current,parent)){
            parent -> setRight(child);
        }
    }
    else{
        BinarySearchTree<Key,Value>::root_ = child;
    }
    child -> setParent(parent);

    //second pair of pointers between current and its child
    child -> setRight(current);
    current -> setParent(child);

    //third pair is between current and current's child's original child
    current -> setLeft(childs_rightchild);
    if(childs_rightchild != nullptr){
        childs_rightchild -> setParent(current);
    }
}


//wrote a function to find the taller child of a given node
//ended up not using this function
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::get_taller_child(AVLNode<Key, Value>* current){

    AVLNode<Key, Value>* temp = nullptr;
    if(current == nullptr){
        return temp;
    }

    if(BinarySearchTree<Key,Value>::hasChildren(current) == false){
        return temp;
    }
    else if(BinarySearchTree<Key,Value>::hasOneChild(current)){
        if(BinarySearchTree<Key,Value>::hasLeftChild(current)){
            temp = current -> getLeft();
        }
        else if(BinarySearchTree<Key,Value>::hasRightChild(current)){
            temp = current -> getRight();
        }
    }
    else if(BinarySearchTree<Key,Value>::hasTwoChildren(current)){
        char l_height = current -> getLeft() -> getBalance();
        char r_height = current -> getRight() -> getBalance();
        int left_height = l_height;
        if(left_height == 255){
            left_height = -1;
        }
        int right_height = r_height;
        if(right_height == 255){
            right_height = -1;
        }
        left_height = abs(left_height);
        right_height = abs(right_height);
        if(left_height == right_height){
            temp = current -> getLeft();
        }
        else if(left_height > right_height){
            temp = current -> getLeft();
        }
        else if(right_height > left_height){
            temp = current -> getRight();
        }
    }
    return temp;
}

#endif
