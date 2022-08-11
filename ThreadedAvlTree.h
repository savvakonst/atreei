//
// Created by SVK on 18.07.2022.
//
#ifndef THREADED_AVL_TREE_H
#define THREADED_AVL_TREE_H


typedef void * object_t;

struct AvlNode{
    AvlNode* left_branch_;
    AvlNode* right_branch_;
    object_t object_;
    int height_;
};


#endif