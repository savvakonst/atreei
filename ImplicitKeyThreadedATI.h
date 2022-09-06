//
// Created by SVK on 18.07.2022.
//
#ifndef THREADED_AVL_TREE_H
#define THREADED_AVL_TREE_H

#include <stdint.h>
#include <string.h>

typedef char *tree_key_t;
typedef char *tree_data_t;

typedef unsigned char tree_height_t;
typedef char tree_diff_t;



#define AVL_KEY_LESS(A_KEY, B_KEY) ((uint64_t)(A_KEY) < (uint64_t)(B_KEY))
//#define AVL_KEY_LESS(A_KEY, B_KEY) (strcmp(A_KEY, B_KEY))


#define SIZE_SUPPORT

struct ImKeyAtiNode {
    tree_key_t key_;
    tree_data_t data_;

#ifdef SIZE_SUPPORT
    size_t size_;
#endif


    struct ImKeyAtiNode *left_branch_;  /**< key_ of left_branch_ is less then current */
    struct ImKeyAtiNode *right_branch_; /**< key_ of right_branch_ is more then current */
    struct ImKeyAtiNode *previous_;     /**< key_ of previous_ is less then current */
    struct ImKeyAtiNode *next_;         /**< key_ of next_ is more then current */

    tree_height_t height_; /**< equal 1 when it has no branches*/
};

typedef struct ImKeyAtiNode **ImKeyAtiNodeStackItem_t;
typedef ImKeyAtiNodeStackItem_t *ImKeyAtiNodeStack_t;


/**
 * type of function which removes only key and data without removing node
 */
typedef void (*deleteKeyAndDataF_t)(struct ImKeyAtiNode *);


struct AvlTree {
    struct ImKeyAtiNode *top_node_;
    size_t size_;
    struct ImKeyAtiNode *first_node_;
    struct ImKeyAtiNode *last_node_;

    /// first element of node stack always equal NULL
    ImKeyAtiNodeStack_t stack_;
};

struct AvlTree *newAvlTree();


/**
 * frees the memory allocated for avl_tree and its associated nodes, keys and data.
 */
void deleteAvlTree(struct AvlTree *avl_tree, deleteKeyAndDataF_t delete_data_f);

/**
 * frees the memory allocated for node structure.
 * It doesn't free the memory allocated for key_ and data_
 */
void deleteImKeyAtiNode(struct ImKeyAtiNode *node);


struct ImKeyAtiNode *getFirstImKeyAtiNode(struct AvlTree *avl_tree);

/**
 * if equal key exists it removes node from tree structure and returns pointer to the corresponding node
 */
struct ImKeyAtiNode *findImKeyAtiNode(struct AvlTree *avl_tree, tree_key_t key);

/**
 *  if equal key exists it returns node pointer without insertion
 * (developer responsible for changing data/insertion.), otherwise it creates a new
 * node, inserts that node into the, balances the tree and returns NULL.
 */
struct ImKeyAtiNode *insertImKeyAtiNode(struct AvlTree *avl_tree, const tree_key_t *key_p, tree_data_t data);

/**
 *  if equal key exists it removes node from tree structure and returns pointer to the corresponding node
 *  ( developer is responsible for freeing memory), otherwise it returns NULL.
 */
struct ImKeyAtiNode *removeImKeyAtiNode(struct AvlTree *avl_tree, const tree_key_t *key_p);



#ifdef SIZE_SUPPORT

struct ImKeyAtiNode *findImKeyAtiNodeWithIndex(struct AvlTree *avl_tree, tree_key_t key, size_t *index);

struct ImKeyAtiNode *findImKeyAtiNodeByIndex(struct AvlTree *avl_tree, size_t index);

#endif

#endif