//
// Created by SVK on 18.07.2022.
//
// AVL tree implementation in C

#include "ThreadedAvlTree.h"

#include <stdio.h>
#include <stdlib.h>

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
    node->previous_ = NULL;

    node->height_ = 1;
#ifdef SIZE_SUPPORT
    node->size_ = 1;
#endif


    return (node);
}

struct AvlTree *newAvlTree() {
    struct AvlTree *avl_tree = (struct AvlTree *)malloc(sizeof(struct AvlTree));
    avl_tree->stack_ = (avlNodeStack_t)malloc(sizeof(avlNodeStackItem_t) * MAX_AVL_STACK_LENGTH_X32);
    *avl_tree->stack_ = NULL;
    avl_tree->top_node_ = NULL;
    avl_tree->first_node_ = NULL;
    avl_tree->last_node_ = NULL;
    avl_tree->size_ = 0;
    return avl_tree;
}

void deleteAvlNode(struct AvlNode *node) { free(node); }

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


struct AvlNode *getFirstAvlNode(struct AvlTree *avl_tree) {
    struct AvlNode *node = avl_tree->top_node_;

    if (node)
        while (node->left_branch_) node = node->left_branch_;

    return node;
}



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
struct AvlNode *insertAvlNode(struct AvlTree *avl_tree, const tree_key_t *key_p, tree_data_t data) {
    avlNodeStack_t node_stack = avl_tree->stack_;
    avlNodeStackItem_t stack_item = &(avl_tree->top_node_);
    *(++node_stack) = stack_item;

    while (*stack_item != NULL) {
        struct AvlNode *node = *stack_item;
        if (AVL_KEY_LESS(*key_p, node->key_)) stack_item = &(node->left_branch_);
        else if (AVL_KEY_LESS(node->key_, *key_p)) stack_item = &(node->right_branch_);
        else return node;
        *(++node_stack) = stack_item;
    }

    // create node and fill it
    struct AvlNode *to_insert = newAvlNodeEmpty();
    to_insert->key_ = *key_p;
    to_insert->data_ = data;

    *stack_item = to_insert;

    if (*(--node_stack) == NULL) return to_insert;



    struct AvlNode *node = **node_stack;

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
#    define SIZE_INCREMENT node->size_++
#    define UPDATE_SIZE(X) ((X)->size_ = (X)->right_branch_->size_ + (X)->left_branch_->size_ + 1)
#else
#    define FINISH_TREATMENT return NULL
#    define SIZE_INCREMENT node->size_++
#    define UPDATE_SIZE(ARG)
#endif

    if (node->height_ > 1) FINISH_TREATMENT;
    node->height_ = 2;
    // node = **(--node_stack);

    if (*(--node_stack) == NULL || (**node_stack)->height_ > 2) FINISH_TREATMENT;

    node = **(node_stack);
    SIZE_INCREMENT;

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
        FINISH_TREATMENT;
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
        FINISH_TREATMENT;
    }

    node->height_ = 3;
    // node = **(--node_stack);



    while (*(--node_stack)) {
        node = **node_stack;
        SIZE_INCREMENT;

        int diff = node->left_branch_->height_ - node->right_branch_->height_;
        if (diff > 0) {
            if (diff == 1) node->height_ = node->left_branch_->height_ + 1;
            else {
                // TODO define 'r' here, and simplify comparison and LEFT_ROTATION
                struct AvlNode *l = node->left_branch_;
                if (l->left_branch_->height_ < l->right_branch_->height_) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);
                    l->height_ = r->height_;
                    r->height_ = node->height_;
                    l->size_ = l->size_ - (r->right_branch_ ? r->right_branch_->size_ : 0) - 1;
                    // UPDATE_SIZE(l);
                    l = r;
                }
                LIGHT_RIGHT_ROTATION(**node_stack, node, l);

                node->height_ = node->height_ - 1;
                size_t temp_size = node->size_;
                node->size_ = node->size_ - l->left_branch_->size_ - 1;
                l->size_ = temp_size;
                // UPDATE_SIZE(node);
                //  UPDATE_SIZE(l);

                FINISH_TREATMENT;
            }
        } else if (diff < 0) {
            if (diff == -1) node->height_ = node->right_branch_->height_ + 1;
            else {
                struct AvlNode *r = node->right_branch_;
                if (r->right_branch_->height_ < r->left_branch_->height_) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);
                    r->height_ = l->height_;
                    l->height_ = node->height_;
                    r->size_ = r->size_ - (l->left_branch_ ? l->left_branch_->size_ : 0) - 1;
                    // UPDATE_SIZE(r);
                    r = l;
                }
                LIGHT_LEFT_ROTATION(**node_stack, node, r);

                node->height_ = node->height_ - 1;
                // r->size_ = node->size_;
                size_t temp_size = node->size_;
                node->size_ = node->size_ - r->right_branch_->size_ - 1;
                r->size_ = temp_size;
                // UPDATE_SIZE(node);
                //  UPDATE_SIZE(r);

                FINISH_TREATMENT;
            }
        } else FINISH_TREATMENT;

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

    // currently, *stack_item points the node to be removed, so **(node_stack-1) points to the previous node.

    tree_height_t h = node_to_delete->height_;

    DEBUG_PRINT("deletion type determination: ");
    if (node_to_delete->left_branch_ == NULL) {
        DEBUG_PRINT("left branch is empty\n");
        *stack_item = node_to_delete->right_branch_;
    } else if (node_to_delete->right_branch_ == NULL) {
        DEBUG_PRINT("right branch is empty\n");
        *stack_item = node_to_delete->left_branch_;
    } else {
        DEBUG_PRINT("both subbranches exist\n");
        avlNodeStackItem_t *stack_pos_to_insert = ++node_stack;

        *stack_pos_to_insert = &(node_to_delete->left_branch_);

        struct AvlNode *tmp = node_to_delete->left_branch_;
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
        *stack_item = tmp;
    }

    if (*(--node_stack) == NULL) return node_to_delete;

    struct AvlNode *node = **node_stack;
    DEBUG_PRINT("first step: ");
    if (h == 1) {
        DEBUG_PRINT("h == 1\n");
        if (node->height_ == 2) {
            DEBUG_PRINT("simple no rotation \n");
            if (node->left_branch_ == NULL && node->right_branch_ == NULL) node->height_ = 1;
        } else if (node->left_branch_) {
            struct AvlNode *l = node->left_branch_;
            if (l->right_branch_) {
                DEBUG_PRINT("simple double right rotation \n");
                struct AvlNode *r = l->right_branch_;
                l->right_branch_ = NULL;

                r->left_branch_ = l;
                r->right_branch_ = node;

                // TODO: probably, using l->height_ = l->left_branch_? 2 :1;  instead of line below is better
                if (l->left_branch_ == NULL) l->height_ = 1;
                r->height_ = l->height_ + 1;
                **node_stack = r;
            } else {
                DEBUG_PRINT("simple right rotation \n");
                l->right_branch_ = node;
                **node_stack = l;
            }

            node->left_branch_ = NULL;
            node->height_ = 1;

        } else {
            struct AvlNode *r = node->right_branch_;
            if (r->left_branch_) {
                DEBUG_PRINT("simple double left rotation \n");
                struct AvlNode *l = r->left_branch_;
                r->left_branch_ = NULL;

                l->right_branch_ = r;
                l->left_branch_ = node;

                // TODO: probably, using r->height_ = r->right_branch_? 2 :1;  instead of line below is better
                if (r->right_branch_ == NULL) r->height_ = 1;
                l->height_ = r->height_ + 1;
                **node_stack = l;
            } else {
                DEBUG_PRINT("simple left rotation \n");
                r->left_branch_ = node;
                **node_stack = r;
            }

            node->right_branch_ = NULL;
            node->height_ = 1;
        }
    } else {
        DEBUG_PRINT("h == %d\n" D_COMMA h);
        node_stack++;
    }


    while (*(--node_stack)) {
        DEBUG_PRINT("next step: ");
        node = **node_stack;

        int diff = node->left_branch_->height_ - node->right_branch_->height_;
        if (diff > 0) {
            if (diff == 1) {
                DEBUG_PRINT("|diff| == 1\n");
                return node_to_delete;  // node->height_ = node->left_branch_->height_ + 1;

            } else {
                struct AvlNode *l = node->left_branch_;
                tree_height_t h_tmp = l->right_branch_->height_;
                // TODO define 'r' here, and simplify comparison and LEFT_ROTATION
                if (l->left_branch_->height_ < l->right_branch_->height_) {  //(simple rot)
                    LEFT_ROTATION(node->left_branch_, l);
                    DEBUG_PRINT("double ");
                    l->height_ = r->height_;
                    h_tmp = node->right_branch_->height_;

                    l = r;
                }
                LIGHT_RIGHT_ROTATION(**node_stack, node, l);
                DEBUG_PRINT("right rotation\n");
                node->height_ = h_tmp + 1;
                l->height_ = node->height_ + 1;
            }
        } else if (diff < 0) {
            if (diff == -1) {
                DEBUG_PRINT("|diff| == 1\n");
                return node_to_delete;  // node->height_ = node->right_branch_->height_ + 1;
            } else {
                struct AvlNode *r = node->right_branch_;
                tree_height_t h_tmp = r->left_branch_->height_;
                if (r->right_branch_->height_ < r->left_branch_->height_) {  //(simple rot)
                    RIGHT_ROTATION(node->right_branch_, r);
                    DEBUG_PRINT("double ");
                    r->height_ = l->height_;
                    h_tmp = node->left_branch_->height_;
                    r = l;
                }
                LIGHT_LEFT_ROTATION(**node_stack, node, r);
                DEBUG_PRINT("left rotation\n");
                node->height_ = h_tmp + 1;
                r->height_ = node->height_ + 1;
            }
        } else {
            DEBUG_PRINT("diff == 0\n");
            node->height_ = node->left_branch_->height_ + 1;
        };
    }

    DEBUG_PRINT("exit from root : \n");
    return node_to_delete;
}
