//
// Created by SVK on 18.07.2022.
//
#ifndef ATI_THREADED_ATI_H
#define ATI_THREADED_ATI_H

#include <stdint.h>
#include <string.h>

typedef void *tree_key_t;
typedef void *tree_data_t;

typedef unsigned char tree_height_t;
typedef char tree_diff_t;



#define AVL_KEY_LESS(A_KEY, B_KEY) ((uint64_t)(A_KEY) < (uint64_t)(B_KEY))
//#define AVL_KEY_LESS(A_KEY, B_KEY) (strcmp(A_KEY, B_KEY))


#define SIZE_SUPPORT

struct AtiNode {
    tree_key_t key_;
    tree_data_t data_;


    struct AtiNode *left_branch_;  /**< key_ of left_branch_ is less then current */
    struct AtiNode *right_branch_; /**< key_ of right_branch_ is more then current */
    struct AtiNode *previous_;     /**< key_ of previous_ is less then current */
    struct AtiNode *next_;         /**< key_ of next_ is more then current */

#ifdef SIZE_SUPPORT
    size_t size_;
#endif

    tree_height_t height_; /**< equal 1 when it has no branches*/
};

typedef struct AtiNode **atiNodeStackItem_t;
typedef atiNodeStackItem_t *atiNodeStack_t;


/**
 * type of function which removes only key and data without removing node
 */
typedef void (*deleteKeyAndDataF_t)(struct AtiNode *);


struct Ati {
    struct AtiNode *top_node_;


    /// first element of node stack always equal NULL
    atiNodeStack_t stack_;
};

struct Ati *newAti();

/**
 * frees the memory allocated for avl_tree and its associated nodes, keys and data.
 */
void deleteAti(struct Ati *avl_tree, deleteKeyAndDataF_t delete_data_f);

/**
 * frees the memory allocated for node structure.
 * It doesn't free the memory allocated for key_ and data_
 */
void deleteAtiNode(struct AtiNode *node);


struct AtiNode *getFirstAtiNode(struct Ati *avl_tree);

/**
 * if equal key exists it removes node from tree structure and returns pointer to the corresponding node
 */
struct AtiNode *findAtiNode(struct Ati *avl_tree, tree_key_t key);

/**
 *  if equal key exists it returns node pointer without insertion
 * (developer responsible for changing data/insertion.), otherwise it creates a new
 * node, inserts that node into the, balances the tree and returns NULL.
 */
struct AtiNode *insertAtiNode(struct Ati *avl_tree, const tree_key_t *key_p, tree_data_t data);

/**
 *  if equal key exists it removes node from tree structure and returns pointer to the corresponding node
 *  ( developer is responsible for freeing memory), otherwise it returns NULL.
 */
struct AtiNode *removeAtiNode(struct Ati *avl_tree, const tree_key_t *key_p);



#ifdef SIZE_SUPPORT

struct AtiNode *findAtiNodeWithIndex(struct Ati *avl_tree, tree_key_t key, size_t *index);

struct AtiNode *findAtiNodeByIndex(struct Ati *avl_tree, size_t index);

#endif

#endif