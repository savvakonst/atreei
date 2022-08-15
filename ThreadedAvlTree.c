//
// Created by SVK on 18.07.2022.
//
// AVL tree implementation in C

#include "ThreadedAvlTree.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_AVL_STACK_LENGTH_X64 93
#define MAX_AVL_STACK_LENGTH_X32 47


//
// Calculate height
int height(struct AvlNode *N) {
    if (N == NULL) return 0;
    return N->height_;
}

// Create a node
struct AvlNode *newAvlNode(tree_key_t key, tree_data_t data) {
    struct AvlNode *node = (struct AvlNode *)malloc(sizeof(struct AvlNode));

    node->key_ = key;
    node->data_ = data;

    node->left_branch_ = NULL;
    node->right_branch_ = NULL;

    node->right_branch_ = NULL;
    node->next_ = NULL;

    node->height_ = 1;

    return (node);
}

struct AvlNode *newAvlNodeEmpty() {
    struct AvlNode *node = (struct AvlNode *)malloc(sizeof(struct AvlNode));

    node->left_branch_ = NULL;
    node->right_branch_ = NULL;

    node->right_branch_ = NULL;
    node->next_ = NULL;

    node->height_ = 1;

    return (node);
}

struct AvlTree *newAvlTree() {
    struct AvlTree *avl_tree = (struct AvlTree *)malloc(sizeof(struct AvlTree));
    avl_tree->stack_ = (avlNodeStack_t)malloc(sizeof(avlNodeStackItem_t) * MAX_AVL_STACK_LENGTH_X32);
    avl_tree->top_node_ = NULL;
    avl_tree->first_node_ = NULL;
    avl_tree->last_node_ = NULL;
    avl_tree->size_ = 0;
    return avl_tree;
}

struct AvlNode *findAvlNode(struct AvlTree *avl_tree, tree_key_t key) {
    struct AvlNode *node = avl_tree->top_node_;
    avlNodeStack_t node_stack = avl_tree->stack_;

    while (node != NULL) {
        if (AVL_KEY_LESS(key, node->key_)) {
            node = node->left_branch_;
        } else if (AVL_KEY_LESS(node->key_, key)) {
            node = node->left_branch_;
        } else return node;
    }
    return node;
}


void deleteAvlTree(struct AvlTree *avl_tree, deleteKeyAndDataF_t delete_data_f) {
    struct AvlNode *node = avl_tree->top_node_;

    if (node) {
        while (node->left_branch_) node = node->left_branch_;

        while (node) {
            struct AvlNode *tmp = node;
            node = node->next_;
            if (delete_data_f) delete_data_f(tmp);
        }
    }
    free(avl_tree->stack_);
    free(avl_tree);
}

void deleteAvlNode(struct AvlNode *node) { free(node); }



#define LIGHT_LEFT_ROTATION(DST, SRC, RIGHT)       \
    struct AvlNode *tmp_0 = (RIGHT)->left_branch_; \
    (RIGHT)->left_branch_ = (SRC);                 \
    (SRC)->right_branch_ = tmp_0;                  \
    (DST) = (RIGHT)


#define LEFT_ROTATION(DST, SRC)               \
                                              \
    struct AvlNode *r = (SRC)->right_branch_; \
    LIGHT_LEFT_ROTATION(DST, SRC, r);



#define LIGHT_RIGHT_ROTATION(DST, SRC, LEFT)       \
    struct AvlNode *tmp_0 = (LEFT)->right_branch_; \
    (LEFT)->right_branch_ = (SRC);                 \
    (SRC)->left_branch_ = tmp_0;                   \
    (DST) = (LEFT)

#define RIGHT_ROTATION(DST, SRC)             \
                                             \
    struct AvlNode *l = (SRC)->left_branch_; \
    LIGHT_RIGHT_ROTATION(DST, SRC, l);


/*                           *
 *  diff > 0                 *
 *         (simple rot)      *         (complex rot)
 *     \                     *   b  c     a  b
 *      a  b       \ b  c    *  a \/       \/ c       a b c d
 *       \/ c  ->   a \/     *   \/ d   ->  \/ d  ->   \/ \/
 *        \/         \/      *    \/         \/         \ /
 *
 *---------------------------+-----------------------------
 */


/**
 *  if equal key exists it returns node pointer without insertion (if equal key
 * exists, developer responsible for insertion), otherwise it creates a new
 * node, inserts that node into the, balances the tree and returns NULL.
 */
struct AvlNode *insertAvlNode(struct AvlTree *avl_tree, const tree_key_t *key_p, tree_data_t *data) {
    avlNodeStack_t node_stack = avl_tree->stack_;
    avlNodeStackItem_t stack_item = &(avl_tree->top_node_);

    while (*stack_item != NULL) {
        *(++node_stack) = stack_item;
        struct AvlNode *node = *stack_item;
        if (AVL_KEY_LESS(*key_p, node->key_)) stack_item = &(node->left_branch_);
        else if (AVL_KEY_LESS(node->key_, *key_p)) stack_item = &(node->right_branch_);
        else return node;
    }

    // create node and fill it
    struct AvlNode *to_insert = newAvlNodeEmpty();
    to_insert->key_ = *key_p;
    to_insert->data_ = *data;

    *stack_item = to_insert;

    if (*(--node_stack) == NULL) return to_insert;



    struct AvlNode *node = **node_stack;

    if (node->left_branch_ == to_insert) {
        to_insert->next_ = node;
        to_insert->previous_ = node->previous_;
        node->previous_ = to_insert;
    } else {
        to_insert->previous_ = node;
        to_insert->next_ = node->next_;
        node->next_ = to_insert;
    }



    if (node->height_ > 1) return NULL;
    node->height_ = 2;
    node = **(--node_stack);

    if (node->height_ > 2) return NULL;

    if (node->right_branch_ == NULL) {
        struct AvlNode *l = node->left_branch_;
        if (l->right_branch_) {
            struct AvlNode *r = l->right_branch_;
            l->right_branch_ = NULL;

            r->left_branch_ = l;
            r->right_branch_ = node;

            l->height_ = 1;
            r->height_ = 2;
            **node_stack = r;
        } else {
            l->right_branch_ = node;
            **node_stack = l;
        }

        node->left_branch_ = NULL;
        node->height_ = 1;
        return NULL;
    } else if (node->left_branch_ == NULL) {
        struct AvlNode *r = node->right_branch_;
        if (r->left_branch_) {
            struct AvlNode *l = r->left_branch_;
            r->left_branch_ = NULL;

            l->right_branch_ = r;
            l->left_branch_ = node;

            r->height_ = 1;
            l->height_ = 2;
            **node_stack = l;
        } else {
            r->left_branch_ = node;
            **node_stack = r;
        }

        node->right_branch_ = NULL;
        node->height_ = 1;
        return NULL;
    }

    node->height_ = 3;
    // node = **(--node_stack);

    while (*(--node_stack)) {
        node = **node_stack;

        int diff = node->left_branch_->height_ - node->right_branch_->height_;
        if (diff > 0) {
            if (diff == 1) node->height_ = node->left_branch_->height_ + 1;
            else {
                struct AvlNode *l =
                    node->left_branch_;  // TODO define 'r' here, and simplify comparison and LEFT_ROTATION
                if (l->left_branch_->height_ < l->right_branch_->height_) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);

                    l->height_ = r->height_;
                    r->height_ = node->height_;
                    l = r;
                }
                LIGHT_RIGHT_ROTATION(**node_stack, node, l);

                node->height_ = node->height_ - 1;

                return NULL;
            }
        } else if (diff < 0) {
            if (diff == -1) node->height_ = node->right_branch_->height_ + 1;
            else {
                struct AvlNode *r = node->right_branch_;
                if (r->right_branch_->height_ < r->left_branch_->height_) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);

                    r->height_ = l->height_;
                    l->height_ = node->height_;
                    r = l;
                }
                LIGHT_LEFT_ROTATION(**node_stack, node, r);

                node->height_ = node->height_ - 1;

                return NULL;
            }
        } else return NULL;

        // node = **(node_stack--);
    }

    return NULL;
}


struct AvlNode *removeAvlNode(struct AvlTree *avl_tree, const tree_key_t *key_p) {
    avlNodeStack_t node_stack = avl_tree->stack_;
    avlNodeStackItem_t stack_item = &(avl_tree->top_node_);


    struct AvlNode *node_to_delete = *stack_item;

    while (node_to_delete != NULL) {
        *(++node_stack) = stack_item;

        if (AVL_KEY_LESS(*key_p, node_to_delete->key_)) stack_item = &(node_to_delete->left_branch_);
        else if (AVL_KEY_LESS(node_to_delete->key_, *key_p)) stack_item = &(node_to_delete->right_branch_);
        else break;
        node_to_delete = *stack_item;
    }
    if (node_to_delete == NULL) return NULL;


    //
    if (node_to_delete->next_) node_to_delete->next_->previous_ = node_to_delete->previous_;

    if (node_to_delete->previous_) node_to_delete->previous_->next_ = node_to_delete->next_;

    // struct AvlNode * const node_to_delete = node;

    // currently, *stack_item points the node to be removed, so **(node_stack-1) points to the previous node.

    tree_height_t h = node_to_delete->height_;

    if (node_to_delete->left_branch_ == NULL) {
        *stack_item = node_to_delete->right_branch_;
        // if (node_to_delete->right_branch_ == NULL) {
        //     stack_item = *(--node_stack);
        //     struct AvlNode *tmp = *stack_item;
        //     if ((tmp->left_branch_ == NULL) || (tmp->right_branch_ == NULL)) tmp->height_ = 1;
        // }
    } else if (node_to_delete->right_branch_ == NULL) {
        *stack_item = node_to_delete->left_branch_;
    } else {
        avlNodeStackItem_t *stack_pos_to_insert = ++node_stack;

        struct AvlNode *tmp = node_to_delete->left_branch_;
        while (tmp->right_branch_) {
            *(++node_stack) = &(tmp->right_branch_);
            tmp = tmp->right_branch_;
        }

        // *node_stack points to the replaceable node
        h = tmp->height_;
        // replacing nodes  begin
        *stack_pos_to_insert = &(tmp->left_branch_);
        tmp->right_branch_ = node_to_delete->right_branch_;
        **node_stack = tmp->left_branch_;
        tmp->left_branch_ = node_to_delete->left_branch_;
        *stack_item = tmp;
        // replacing nodes  finish

        stack_item = *node_stack;
    }

    if (*(--node_stack) == NULL) return node_to_delete;

    struct AvlNode *node = **node_stack;

    if (h == 1) {
        if (node->height_ == 2) {
            if (node->left_branch_ == NULL && node->right_branch_ == NULL) node->height_ = 1;
        } else if (node->left_branch_) {
            struct AvlNode *l = node->left_branch_;
            if (l->right_branch_) {
                struct AvlNode *r = l->right_branch_;
                l->right_branch_ = NULL;

                r->left_branch_ = l;
                r->right_branch_ = node;

                // TODO: probably, using l->height_ = l->left_branch_? 2 :1;  instead of line below is better
                if (l->left_branch_ == NULL) l->height_ = 1;
                r->height_ = l->height_ + 1;
                **node_stack = r;
            } else {
                l->right_branch_ = node;
                **node_stack = l;
            }

            node->left_branch_ = NULL;
            node->height_ = 1;

        } else {
            struct AvlNode *r = node->right_branch_;
            if (r->left_branch_) {
                struct AvlNode *l = r->left_branch_;
                r->left_branch_ = NULL;

                l->right_branch_ = r;
                l->left_branch_ = node;

                // TODO: probably, using r->height_ = r->right_branch_? 2 :1;  instead of line below is better
                if (r->right_branch_ == NULL) r->height_ = 1;
                l->height_ = r->height_ + 1;
                **node_stack = l;
            } else {
                r->left_branch_ = node;
                **node_stack = r;
            }

            node->right_branch_ = NULL;
            node->height_ = 1;
        }
    }


    while (*(--node_stack)) {
        node = **node_stack;

        int diff = node->left_branch_->height_ - node->right_branch_->height_;
        if (diff > 0) {
            if (diff == 1) node->height_ = node->left_branch_->height_ + 1;
            else {
                struct AvlNode *l = node->left_branch_;
                tree_height_t h_tmp = node->right_branch_->height_;
                // TODO define 'r' here, and simplify comparison and LEFT_ROTATION
                if (l->left_branch_->height_ < l->right_branch_->height_) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);

                    l->height_ = r->height_;
                    r->height_ = r->height_ + 1;
                    l = r;
                }
                LIGHT_RIGHT_ROTATION(**node_stack, node, l);

                node->height_ = h_tmp + 1;
            }
        } else if (diff < 0) {
            if (diff == -1) node->height_ = node->right_branch_->height_ + 1;
            else {
                struct AvlNode *r = node->right_branch_;
                tree_height_t h_tmp = node->left_branch_->height_;
                if (r->right_branch_->height_ < r->left_branch_->height_) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);

                    r->height_ = l->height_;
                    l->height_ = l->height_ + 1;
                    r = l;
                }
                LIGHT_LEFT_ROTATION(**node_stack, node, r);

                node->height_ = h_tmp + 1;
            }
        } else return NULL;

        // node = **(node_stack--);
    }

    return node_to_delete;
}
