//
// Created by SVK on 18.07.2022.
//
// AVL tree implementation in C

#include <stdio.h>
#include <stdlib.h>

#include "ThreadedATI.h"

#define MAX_AVL_STACK_LENGTH_X64 93
#define MAX_AVL_STACK_LENGTH_X32 47

#ifdef _DEBUG
#    include <stdio.h>
#    define D_COMMA ,
#    define DEBUG_PRINT(X) \
        printf(X);         \
        fflush(stdout);

#else
#    define D_COMMA
#    define DEBUG_PRINT(X)
#endif

// Create a node
struct ImKeyAtiNode *newImKeyAtiNode(tree_key_t key, tree_data_t data) {
    struct ImKeyAtiNode *node = (struct ImKeyAtiNode *)malloc(sizeof(struct ImKeyAtiNode));

    node->key_ = key;
    node->data_ = data;

    node->left_branch_ = NULL;
    node->right_branch_ = NULL;

    node->right_branch_ = NULL;
    node->next_ = NULL;

    node->height_ = 1;

    return (node);
}

struct ImKeyAtiNode *newImKeyAtiNodeEmpty() {
    struct ImKeyAtiNode *node = (struct ImKeyAtiNode *)malloc(sizeof(struct ImKeyAtiNode));

    node->left_branch_ = NULL;
    node->right_branch_ = NULL;

    node->right_branch_ = NULL;
    node->next_ = NULL;
    node->previous_ = NULL;

    node->height_ = 1;
#ifdef SIZE_SUPPORT
    node->size_ = 1;
#endif


    return (node);
}

struct AvlTree *newAvlTree() {
    struct AvlTree *avl_tree = (struct AvlTree *)malloc(sizeof(struct AvlTree));
    avl_tree->stack_ = (ImKeyAtiNodeStack_t)malloc(sizeof(ImKeyAtiNodeStackItem_t) * MAX_AVL_STACK_LENGTH_X32);
    *avl_tree->stack_ = NULL;
    avl_tree->top_node_ = NULL;
    avl_tree->first_node_ = NULL;
    avl_tree->last_node_ = NULL;
    avl_tree->size_ = 0;
    return avl_tree;
}

void deleteImKeyAtiNode(struct ImKeyAtiNode *node) { free(node); }

void deleteAvlTree(struct AvlTree *avl_tree, deleteKeyAndDataF_t delete_data_f) {
    struct ImKeyAtiNode *node = avl_tree->top_node_;

    if (node) {
        while (node->left_branch_) node = node->left_branch_;

        while (node) {
            struct ImKeyAtiNode *tmp = node;
            node = node->next_;
            if (delete_data_f) delete_data_f(tmp);
        }
    }
    free(avl_tree->stack_);
    free(avl_tree);
}



struct ImKeyAtiNode *findImKeyAtiNode(struct AvlTree *avl_tree, tree_key_t key) {
    struct ImKeyAtiNode *node = avl_tree->top_node_;

    while (node != NULL) {
        if (AVL_KEY_LESS(key, node->key_)) {
            node = node->left_branch_;
        } else if (AVL_KEY_LESS(node->key_, key)) {
            node = node->right_branch_;
        } else return node;
    }
    return node;
}


struct ImKeyAtiNode *getFirstImKeyAtiNode(struct AvlTree *avl_tree) {
    struct ImKeyAtiNode *node = avl_tree->top_node_;

    if (node)
        while (node->left_branch_) node = node->left_branch_;

    return node;
}



#define LITE_LEFT_ROTATION(DST, SRC, RIGHT)             \
    struct ImKeyAtiNode *tmp_0 = (RIGHT)->left_branch_; \
    (RIGHT)->left_branch_ = (SRC);                      \
    (SRC)->right_branch_ = tmp_0;                       \
    (DST) = (RIGHT)


#define LEFT_ROTATION(DST, SRC)                    \
                                                   \
    struct ImKeyAtiNode *r = (SRC)->right_branch_; \
    LITE_LEFT_ROTATION(DST, SRC, r);



#define LITE_RIGHT_ROTATION(DST, SRC, LEFT)             \
    struct ImKeyAtiNode *tmp_0 = (LEFT)->right_branch_; \
    (LEFT)->right_branch_ = (SRC);                      \
    (SRC)->left_branch_ = tmp_0;                        \
    (DST) = (LEFT)

#define RIGHT_ROTATION(DST, SRC)                  \
                                                  \
    struct ImKeyAtiNode *l = (SRC)->left_branch_; \
    LITE_RIGHT_ROTATION(DST, SRC, l);

#ifdef SIZE_SUPPORT
#    define SIZE_INCREMENT(NODE) (NODE)->size_++
#    define SIZE_DECREMENT(NODE) (NODE)->size_--

#    define DECREMENT_NODE_STACK --node_stack

#    define UPDATE_SHORT_DOUBLE_ROTATION_SIZES(X, Y, NODE) \
        (X)->size_--;                                      \
        (Y)->size_ = (NODE)->size_;                        \
        (NODE)->size_ = 1;


#    define UPDATE_SHORT_SINGLE_ROTATION_SIZES(X, NODE) \
        (X)->size_ = 3;                                 \
        (NODE)->size_ = 1;

#    define UPDATE_ROTATION_SIZES(X, DIRECTION, NODE)                  \
        {                                                              \
            size_t temp_size = (NODE)->size_;                          \
            (NODE)->size_ = (NODE)->size_ - (X)->DIRECTION->size_ - 1; \
            (X)->size_ = temp_size;                                    \
        }


#    define UPDATE_DOUBLE_ROTATION_SIZES(X, Y) X->size_ = X->size_ - (Y ? Y->size_ : 0) - 1

#else
#    define SIZE_INCREMENT(NODE)
#    define SIZE_DECREMENT(NODE)

#    define DECREMENT_NODE_STACK --node_stack

#    define UPDATE_SHORT_DOUBLE_ROTATION_SIZES(X)
#    define UPDATE_SHORT_SINGLE_ROTATION_SIZES(X)
#    define UPDATE_ROTATION_SIZES(X, NODE)
#    define UPDATE_DOUBLE_ROTATION_SIZES(X, Y)
#endif


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
struct ImKeyAtiNode *insertImKeyAtiNode(struct AvlTree *avl_tree, const tree_key_t *key_p, tree_data_t data) {
    ImKeyAtiNodeStack_t node_stack = avl_tree->stack_;
    ImKeyAtiNodeStackItem_t stack_item = &(avl_tree->top_node_);
    *(++node_stack) = stack_item;

    while (*stack_item != NULL) {
        struct ImKeyAtiNode *node = *stack_item;
        if (AVL_KEY_LESS(*key_p, node->key_)) stack_item = &(node->left_branch_);
        else if (AVL_KEY_LESS(node->key_, *key_p)) stack_item = &(node->right_branch_);
        else return node;
        *(++node_stack) = stack_item;
    }

    // create node and fill it
    struct ImKeyAtiNode *to_insert = newImKeyAtiNodeEmpty();
    to_insert->key_ = *key_p;
    to_insert->data_ = data;

    *stack_item = to_insert;

    if (*(--node_stack) == NULL) return to_insert;



    struct ImKeyAtiNode *node = **node_stack;

    if (node->left_branch_ == to_insert) {
        to_insert->next_ = node;
        to_insert->previous_ = node->previous_;
        node->previous_ = to_insert;
        if (to_insert->previous_) to_insert->previous_->next_ = to_insert;
    } else {
        to_insert->previous_ = node;
        to_insert->next_ = node->next_;
        node->next_ = to_insert;
        if (to_insert->next_) to_insert->next_->previous_ = to_insert;
    }

#ifdef SIZE_SUPPORT
#    define FINISH_TREATMENT goto size_treatment
#else
#    define FINISH_TREATMENT return NULL
#endif

    if (node->height_ > 1) FINISH_TREATMENT;
    node->height_ = 2;
    // node = **(--node_stack);
    SIZE_INCREMENT(node);
    if (*(--node_stack) == NULL || (**node_stack)->height_ > 2) FINISH_TREATMENT;

    node = **(node_stack);
    SIZE_INCREMENT(node);

    if (node->right_branch_ == NULL) {
        struct ImKeyAtiNode *l = node->left_branch_;
        if (l->right_branch_) {
            struct ImKeyAtiNode *r = l->right_branch_;
            l->right_branch_ = NULL;

            r->left_branch_ = l;
            r->right_branch_ = node;

            l->height_ = 1;
            r->height_ = 2;

            UPDATE_SHORT_DOUBLE_ROTATION_SIZES(l, r, node);

            **node_stack = r;
        } else {
            l->right_branch_ = node;
            UPDATE_SHORT_SINGLE_ROTATION_SIZES(l, node);
            **node_stack = l;
        }

        node->left_branch_ = NULL;
        node->height_ = 1;
        --node_stack;
        FINISH_TREATMENT;
    } else if (node->left_branch_ == NULL) {
        struct ImKeyAtiNode *r = node->right_branch_;
        if (r->left_branch_) {
            struct ImKeyAtiNode *l = r->left_branch_;
            r->left_branch_ = NULL;

            l->right_branch_ = r;
            l->left_branch_ = node;

            r->height_ = 1;
            l->height_ = 2;

            UPDATE_SHORT_DOUBLE_ROTATION_SIZES(r, l, node);

            **node_stack = l;
        } else {
            r->left_branch_ = node;
            UPDATE_SHORT_SINGLE_ROTATION_SIZES(r, node);
            **node_stack = r;
        }

        node->right_branch_ = NULL;
        node->height_ = 1;
        --node_stack;
        FINISH_TREATMENT;
    }

    node->height_ = 3;
    // node = **(--node_stack);

    while (*(--node_stack)) {
        node = **node_stack;
        SIZE_INCREMENT(node);

        int diff = node->left_branch_->height_ - node->right_branch_->height_;
        if (diff > 0) {
            if (diff == 1) node->height_ = node->left_branch_->height_ + 1;
            else {
                // TODO define 'r' here, and simplify comparison and LEFT_ROTATION
                struct ImKeyAtiNode *l = node->left_branch_;
                if (l->left_branch_->height_ < l->right_branch_->height_) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);
                    l->height_ = r->height_;
                    r->height_ = node->height_;
                    UPDATE_DOUBLE_ROTATION_SIZES(l, r->right_branch_);
                    l = r;
                }
                LITE_RIGHT_ROTATION(**node_stack, node, l);

                node->height_ = node->height_ - 1;

                UPDATE_ROTATION_SIZES(l, left_branch_, node);
                DECREMENT_NODE_STACK;
                FINISH_TREATMENT;
            }
        } else if (diff < 0) {
            if (diff == -1) node->height_ = node->right_branch_->height_ + 1;
            else {
                struct ImKeyAtiNode *r = node->right_branch_;
                if (r->right_branch_->height_ < r->left_branch_->height_) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);
                    r->height_ = l->height_;
                    l->height_ = node->height_;
                    UPDATE_DOUBLE_ROTATION_SIZES(r, l->left_branch_);
                    r = l;
                }
                LITE_LEFT_ROTATION(**node_stack, node, r);

                node->height_ = node->height_ - 1;

                UPDATE_ROTATION_SIZES(r, right_branch_, node);
                DECREMENT_NODE_STACK;
                FINISH_TREATMENT;
            }
        } else {
#ifdef SIZE_SUPPORT
            --node_stack;
#endif
            FINISH_TREATMENT;
        }

        // node = **(node_stack--);
    }
#ifdef SIZE_SUPPORT

size_treatment:
    while (*node_stack) {
        node = **node_stack;
        node->size_++;
        node_stack--;
    }
#endif

    return NULL;
#undef FINISH_TREATMENT
}



struct ImKeyAtiNode *removeImKeyAtiNode(struct AvlTree *avl_tree, const tree_key_t *key_p) {
    ImKeyAtiNodeStack_t node_stack = avl_tree->stack_;
    ImKeyAtiNodeStackItem_t stack_item = &(avl_tree->top_node_);

#ifdef SIZE_SUPPORT
#    define FINISH_TREATMENT goto size_treatment
#else
#    define FINISH_TREATMENT return node_to_delete
#endif


    struct ImKeyAtiNode *node_to_delete = *stack_item;

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

    // currently, *stack_item points the node to be removed, so **(node_stack-1) points to the previous node.

    tree_height_t h = node_to_delete->height_;

    if (node_to_delete->left_branch_ == NULL) {
        *stack_item = node_to_delete->right_branch_;
    } else if (node_to_delete->right_branch_ == NULL) {
        *stack_item = node_to_delete->left_branch_;
    } else {
        ImKeyAtiNodeStackItem_t *stack_pos_to_insert = ++node_stack;

        *stack_pos_to_insert = &(node_to_delete->left_branch_);

        struct ImKeyAtiNode *tmp = node_to_delete->left_branch_;
        while (tmp->right_branch_) {
            *(++node_stack) = &(tmp->right_branch_);
            tmp = tmp->right_branch_;
        }

        // *node_stack points to the replaceable node
        h = tmp->height_;
        tmp->height_ = node_to_delete->height_;
        // replacing nodes  begin
        **node_stack = tmp->left_branch_;
        *stack_pos_to_insert = &(tmp->left_branch_);

        tmp->right_branch_ = node_to_delete->right_branch_;
        tmp->left_branch_ = node_to_delete->left_branch_;

#ifdef SIZE_SUPPORT
        tmp->size_ = node_to_delete->size_;
#endif
        *stack_item = tmp;
    }

    if (*(--node_stack) == NULL) return node_to_delete;

    struct ImKeyAtiNode *node = **node_stack;

    if (h == 1) {
        SIZE_DECREMENT(node);
        if (node->height_ == 2) {
            if (node->left_branch_ == NULL && node->right_branch_ == NULL) node->height_ = 1;
        } else if (node->left_branch_) {
            struct ImKeyAtiNode *l = node->left_branch_;
            if (l->right_branch_) {
                struct ImKeyAtiNode *r = l->right_branch_;
                l->right_branch_ = NULL;

                r->left_branch_ = l;
                r->right_branch_ = node;

                // TODO: probably, using l->height_ = l->left_branch_? 2 :1;  instead of line below is better
                if (l->left_branch_ == NULL) l->height_ = 1;
                r->height_ = l->height_ + 1;

                UPDATE_SHORT_DOUBLE_ROTATION_SIZES(l, r, node);
                **node_stack = r;
            } else {
                l->right_branch_ = node;
                UPDATE_SHORT_SINGLE_ROTATION_SIZES(l, node);
                **node_stack = l;
            }

            node->left_branch_ = NULL;
            node->height_ = 1;

        } else {
            struct ImKeyAtiNode *r = node->right_branch_;
            if (r->left_branch_) {
                struct ImKeyAtiNode *l = r->left_branch_;
                r->left_branch_ = NULL;

                l->right_branch_ = r;
                l->left_branch_ = node;

                // TODO: probably, using r->height_ = r->right_branch_? 2 :1;  instead of line below is better
                if (r->right_branch_ == NULL) r->height_ = 1;
                l->height_ = r->height_ + 1;
                UPDATE_SHORT_DOUBLE_ROTATION_SIZES(r, l, node);
                **node_stack = l;
            } else {
                r->left_branch_ = node;
                UPDATE_SHORT_SINGLE_ROTATION_SIZES(r, node);
                **node_stack = r;
            }

            node->right_branch_ = NULL;
            node->height_ = 1;
        }
    } else {
        node_stack++;
    }


    while (*(--node_stack)) {
        node = **node_stack;

        SIZE_DECREMENT(node);

        int diff = node->left_branch_->height_ - node->right_branch_->height_;
        if (diff > 0) {
            if (diff == 1) {
                FINISH_TREATMENT;  // node->height_ = node->left_branch_->height_ + 1;
            } else {
                struct ImKeyAtiNode *l = node->left_branch_;
                tree_height_t h_tmp = l->right_branch_->height_;
                // TODO define 'r' here, and simplify comparison and LEFT_ROTATION
                if (l->left_branch_->height_ < l->right_branch_->height_) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);
                    l->height_ = r->height_;
                    h_tmp = node->right_branch_->height_;
                    UPDATE_DOUBLE_ROTATION_SIZES(l, r->right_branch_);
                    l = r;
                }
                LITE_RIGHT_ROTATION(**node_stack, node, l);
                node->height_ = h_tmp + 1;
                l->height_ = node->height_ + 1;
                UPDATE_ROTATION_SIZES(l, left_branch_, node);
            }
        } else if (diff < 0) {
            if (diff == -1) {
                FINISH_TREATMENT;  // node->height_ = node->right_branch_->height_ + 1;
            } else {
                struct ImKeyAtiNode *r = node->right_branch_;
                tree_height_t h_tmp = r->left_branch_->height_;
                if (r->right_branch_->height_ < r->left_branch_->height_) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);
                    r->height_ = l->height_;
                    h_tmp = node->left_branch_->height_;
                    UPDATE_DOUBLE_ROTATION_SIZES(r, l->left_branch_);
                    r = l;
                }
                LITE_LEFT_ROTATION(**node_stack, node, r);
                node->height_ = h_tmp + 1;
                r->height_ = node->height_ + 1;
                UPDATE_ROTATION_SIZES(r, right_branch_, node);
            }
        } else {
            node->height_ = node->left_branch_->height_ + 1;
        };
    }

#ifdef SIZE_SUPPORT
    return node_to_delete;
size_treatment:
    while (*(--node_stack)) {
        node = **node_stack;
        node->size_--;
    }

#endif
    return node_to_delete;
#undef FINISH_TREATMENT
}



#ifdef SIZE_SUPPORT

struct ImKeyAtiNode *findImKeyAtiNodeWithIndex(struct AvlTree *avl_tree, tree_key_t key, size_t *index) {
    struct ImKeyAtiNode *node = avl_tree->top_node_;
    if (node == NULL) return NULL;

    *index = node->left_branch_ ? node->left_branch_->size_ : 0;

    while (1) {
        if (AVL_KEY_LESS(key, node->key_)) {
            node = node->left_branch_;
            if (node == NULL) return NULL;
            *index -= (node->right_branch_ ? node->right_branch_->size_ : 0) + 1;
        } else if (AVL_KEY_LESS(node->key_, key)) {
            node = node->right_branch_;
            if (node == NULL) return NULL;
            *index += (node->left_branch_ ? node->left_branch_->size_ : 0) + 1;
        } else return node;
    }
    return node;
}


struct ImKeyAtiNode *findImKeyAtiNodeByIndex(struct AvlTree *avl_tree, size_t index) {
    struct ImKeyAtiNode *node = avl_tree->top_node_;

    if (node == NULL || (node->size_ <= index)) return NULL;

    size_t current_index = (node->left_branch_ ? node->left_branch_->size_ : 0);

    while (1) {
        if (index < current_index) {
            node = node->left_branch_;
            current_index -= (node->right_branch_ ? node->right_branch_->size_ : 0) + 1;
        } else if (index > current_index) {
            node = node->right_branch_;
            current_index += (node->left_branch_ ? node->left_branch_->size_ : 0) + 1;
        } else return node;
    }

    return node;
}

#endif  // SIZE_SUPPORT