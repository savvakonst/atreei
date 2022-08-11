//
// Created by SVK on 18.07.2022.
//
// AVL tree implementation in C

#include <stdio.h>
#include <stdlib.h>

#define MAX_AVL_STACK_LENGTH_X64 93
#define MAX_AVL_STACK_LENGTH_X32 47

typedef int object_t;
typedef unsigned char tree_height_t;
typedef char height_diff_t;
#define AVL_KEY_LESS(A_KEY, B_KEY) ((A_KEY) < (B_KEY))

struct AvlNode {
    struct AvlNode *left_branch_;  /**< key_ of left_branch_ is less then current */
    struct AvlNode *right_branch_; /**< key_ of right_branch_ is more then current */
    struct AvlNode *previous_;     /**< key_ of previous_ is less then current */
    struct AvlNode *next_;         /**< key_ of next_ is more then current */

    object_t key_;


    tree_height_t height_; /**< equal 1 when it has no branches*/
    height_diff_t diff_;
};

typedef struct AvlNode **avlNodeStackItem_t;
typedef avlNodeStackItem_t *avlNodeStack_t;

struct AvlTree {
    size_t size_;
    struct AvlNode *top_node_;
    struct AvlNode *first_node_;
    struct AvlNode *last_node_;

    /// first element of node stack always equal NULL
    avlNodeStack_t stack_;
};

//
// Calculate height
int height(struct AvlNode *N) {
    if (N == NULL) return 0;
    return N->height_;
}

// Create a node
struct AvlNode *newAvlNode(object_t key) {
    struct AvlNode *node = (struct AvlNode *) malloc(sizeof(struct AvlNode));

    node->key_ = key;

    node->left_branch_ = NULL;
    node->right_branch_ = NULL;

    node->right_branch_ = NULL;
    node->next_ = NULL;

    node->height_ = 1;
    node->diff_ = 0;

    return (node);
}

struct AvlNode *newAvlNodeEmpty() {
    struct AvlNode *node = (struct AvlNode *) malloc(sizeof(struct AvlNode));

    node->left_branch_ = NULL;
    node->right_branch_ = NULL;

    node->right_branch_ = NULL;
    node->next_ = NULL;

    node->height_ = 1;

    return (node);
}

struct AvlTree *newAvlTree() {
    struct AvlTree *avl_tree = (struct AvlTree *) malloc(sizeof(struct AvlTree));
    avl_tree->stack_ = (avlNodeStack_t) malloc(sizeof(avlNodeStackItem_t) * MAX_AVL_STACK_LENGTH_X32);
    avl_tree->top_node_ = NULL;
    avl_tree->first_node_ = NULL;
    avl_tree->last_node_ = NULL;
    return avl_tree;
}

struct AvlNode *findAvlNode(struct AvlTree *avl_tree, object_t key) {
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



/*
#define LEFT_ROTATION(DST, SRC)                   \
    {                                             \
        struct AvlNode *r = (SRC)->right_branch_; \
        struct AvlNode *tmp_0 = r->left_branch_;  \
                                                  \
        r->left_branch_ = (SRC);                  \
        (SRC)->right_branch_ = tmp_0;             \
                                                  \
        (DST) = r;                                \
    }
*/
#define LIGHT_LEFT_ROTATION(DST, SRC, RIGHT)       \
    struct AvlNode *tmp_0 = (RIGHT)->left_branch_; \
    (RIGHT)->left_branch_ = (SRC);                 \
    (SRC)->right_branch_ = tmp_0;                  \
    (DST) = (RIGHT)


#define LEFT_ROTATION(DST, SRC)                   \
                                                 \
        struct AvlNode *r = (SRC)->right_branch_; \
        LIGHT_LEFT_ROTATION(DST, SRC, r);         \



#define LIGHT_RIGHT_ROTATION(DST, SRC, LEFT)       \
    struct AvlNode *tmp_0 = (LEFT)->right_branch_; \
    (LEFT)->right_branch_ = (SRC);                 \
    (SRC)->left_branch_ = tmp_0;                   \
    (DST) = (LEFT)

#define RIGHT_ROTATION(DST, SRC)                 \
                                                \
        struct AvlNode *l = (SRC)->left_branch_; \
        LIGHT_RIGHT_ROTATION(DST, SRC, l);       \


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

struct AvlNode *insertAvlNodeH(struct AvlTree *avl_tree, const object_t *key_p) {
    avlNodeStack_t node_stack = avl_tree->stack_;
    avlNodeStackItem_t stack_item = &(avl_tree->top_node_);

    while (*stack_item != NULL) {
        *(++node_stack) = stack_item;
        struct AvlNode *node = *stack_item;
        if (AVL_KEY_LESS(*key_p, node->key_)) stack_item = &(node->left_branch_);
        else if (AVL_KEY_LESS(node->key_, *key_p)) stack_item = &(node->right_branch_);
        else return node;
    }

    // get node to inset
    struct AvlNode *to_insert = newAvlNodeEmpty();
    to_insert->key_ = *key_p;
    *stack_item = to_insert;



    struct AvlNode *node = **(--node_stack);

    if (node->height_ > 1) return NULL;

    while (node) {
        int diff = node->left_branch_->height_ - node->right_branch_->height_;
        if (diff > 0) {
            if (diff == 1) node->height_ = node->left_branch_->height_ + 1;
            else {
                struct AvlNode *l = node->left_branch_;
                tree_height_t temp_h = node->height_;

                if (l->left_branch_->height_ < l->right_branch_->height_) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);
                    l = r;
                }
                LIGHT_RIGHT_ROTATION(**node_stack, node, l);

                node->height_ = node->right_branch_->height_ + 1;
                l->height_ = temp_h;

                return NULL;
            }
        } else if (diff < 0) {
            if (diff == -1) node->height_ = node->right_branch_->height_ + 1;
            else {
                struct AvlNode *r = node->right_branch_;
                tree_height_t temp_h = node->height_;

                if (r->right_branch_->height_ < r->left_branch_->height_) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);
                    r = l;
                }
                LIGHT_LEFT_ROTATION(**node_stack, node, r);

                node->height_ = node->left_branch_->height_ + 1;
                r->height_ = temp_h;

                return NULL;
            }
        } else return NULL;

        node = **(node_stack--);
    }

    return NULL;
}


/**
 *  if equal key exists it returns node pointer without insertion (if equal key
 * exists, developer responsible for insertion), otherwise it creates a new
 * node, inserts that node into the, balances the tree and returns NULL.
 */
struct AvlNode *insertAvlNodeD(struct AvlTree *avl_tree, const object_t *key_p) {
    avlNodeStack_t node_stack = avl_tree->stack_;
    avlNodeStackItem_t stack_item = &(avl_tree->top_node_);

    while (*stack_item != NULL) {
        *(++node_stack) = stack_item;
        struct AvlNode *node = *stack_item;
        if (AVL_KEY_LESS(*key_p, node->key_)) stack_item = &(node->left_branch_);
        else if (AVL_KEY_LESS(node->key_, *key_p)) stack_item = &(node->right_branch_);
        else return node;
    }

    // get node to inset
    struct AvlNode *last_node = newAvlNodeEmpty();
    last_node->key_ = *key_p;
    *stack_item = last_node;

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

    struct AvlNode *node = **(--node_stack);


    if (node->height_ > 1) return NULL;

    while (node) {
        height_diff_t diff = node->diff_ + (height_diff_t) ((node->left_branch_ == last_node) ? 1 : -1);

        if (diff > 0) {
            if (diff == 1) node->diff_ = 1;
            else {
                struct AvlNode *l = node->left_branch_;
                node->diff_ = 0;

                if (l->diff_ < 0) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);

                    if(r->diff_ > 0){
                        l->diff_ = 0 ; // -1 -> 0
                        node->diff_ = -1; // 1 -> -1
                    }else{
                        l->diff_ = 1 ; // -1 -> 0
                        node->diff_ = 0; // 1 -> -1
                    };

                    l = r;

                }
                LIGHT_RIGHT_ROTATION(**node_stack, node, l);


                l->diff_ = 0;

                return NULL;
            }
        } else if (diff < 0) {
            if (diff == -1) node->diff_ = -1;
            else {
                struct AvlNode *r = node->right_branch_;


                if (r->diff_ > 0) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);

                    if(l->diff_ > 0){
                        r->diff_ = -1 ;
                        node->diff_ = 0;
                    }else{
                        r->diff_ = 0 ;
                        node->diff_ = 1;
                    };
                    r = l;
                }
                LIGHT_LEFT_ROTATION(**node_stack, node, r);

                node->diff_ = 0;
                r->diff_ = 0;

                return NULL;
            }
        } else {
            node->diff_ = 0;
            return NULL;
        }

        last_node = node;
        node = **(node_stack--);
    }

    return NULL;
}


struct AvlNode *deleteAvlNode(struct AvlTree *avl_tree, const object_t *key_p) {
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

    *(++node_stack) = stack_item;
    // struct AvlNode * const node_to_delete = node;

    // currently, *stack_item points the node to be removed, so **(node_stack-1) points to the previous node.

    tree_height_t l_h = 0, r_h = 0;

    if (node_to_delete->left_branch_ == NULL) {
        *stack_item = node_to_delete->right_branch_;
        if (node_to_delete->right_branch_ == NULL) {
            stack_item = *(--node_stack);
            struct AvlNode *tmp = *stack_item;
            if ((tmp->left_branch_ == NULL) || (tmp->right_branch_ == NULL)) tmp->height_ = 1;
        }
    } else if (node_to_delete->right_branch_ == NULL) {
        *stack_item = node_to_delete->left_branch_;
    } else {
        avlNodeStackItem_t *stack_pos_to_insert = ++node_stack;

        struct AvlNode *tmp = node_to_delete->left_branch_;
        while (tmp->right_branch_) {
            *(++node_stack) = &(tmp->right_branch_);
            tmp = tmp->right_branch_;
        }

        // replacing nodes  begin
        *stack_pos_to_insert = &(tmp->left_branch_);
        tmp->right_branch_ = node_to_delete->right_branch_;
        **node_stack = tmp->left_branch_;
        tmp->left_branch_ = node_to_delete->left_branch_;
        *stack_item = tmp;
        // replacing nodes  finish

        stack_item = *node_stack;


        tmp = **(--node_stack);
        l_h = tmp->left_branch_ ? tmp->left_branch_->height_ : 0;
        r_h = tmp->right_branch_ ? tmp->right_branch_->height_ : 0;

    }


    return node_to_delete;
}

char getCustomBalance(struct AvlNode *a, struct AvlNode *b) {
    return a == b->left_branch_ ? 1 : -1;
}

char getCustomBalance2(struct AvlNode *a, struct AvlNode *b) {
    //a->right_branch_
    return a == b ? 1 : -1;
}





