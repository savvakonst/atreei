//
// Created by SVK on 18.07.2022.
//
#ifndef THREADED_AVL_TREE_H
#define THREADED_AVL_TREE_H


#define MAX_AVL_STACK_LENGTH_X64 93
#define MAX_AVL_STACK_LENGTH_X32 47



typedef int tree_key_t;
typedef int tree_data_t;

typedef unsigned char tree_height_t;
typedef char tree_diff_t;

#define AVL_KEY_LESS(A_KEY, B_KEY) ((A_KEY) < (B_KEY))

struct AvlNode {
    tree_key_t key_;
    tree_data_t data_;

    struct AvlNode *left_branch_;  /**< key_ of left_branch_ is less then current */
    struct AvlNode *right_branch_; /**< key_ of right_branch_ is more then current */
    struct AvlNode *previous_;     /**< key_ of previous_ is less then current */
    struct AvlNode *next_;         /**< key_ of next_ is more then current */

    tree_height_t height_; /**< equal 1 when it has no branches*/
};

typedef struct AvlNode **avlNodeStackItem_t;
typedef avlNodeStackItem_t *avlNodeStack_t;


/**
 * type of function which removes only key and data without removing node
 */
typedef void (*deleteKeyAndDataF_t)(struct AvlNode *);


struct AvlTree {
    struct AvlNode *top_node_;
    size_t size_;
    struct AvlNode *first_node_;
    struct AvlNode *last_node_;

    /// first element of node stack always equal NULL
    avlNodeStack_t stack_;
};

struct AvlTree *newAvlTree();

/**
 * frees the memory allocated for node structure.
 * It doesn't free the memory allocated for key_ and data_
 */
void deleteAvlNode(struct AvlNode *node);

/**
 * if equal key exists it removes node from tree structure and returns pointer to the corresponding node
 */
struct AvlNode *findAvlNode(struct AvlTree *avl_tree, tree_key_t key);

/**
 *  if equal key exists it returns node pointer without insertion
 * (developer responsible for changing data/insertion.), otherwise it creates a new
 * node, inserts that node into the, balances the tree and returns NULL.
 */
struct AvlNode *insertAvlNode(struct AvlTree *avl_tree, const tree_key_t *key_p, tree_data_t *data);

/**
 *  if equal key exists it removes node from tree structure and returns pointer to the corresponding node
 *  ( developer is responsible for freeing memory), otherwise it returns NULL.
 */
struct AvlNode *removeAvlNode(struct AvlTree *avl_tree, const tree_key_t *key_p);


#endif