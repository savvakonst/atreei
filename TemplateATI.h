//
// Created by SVK on 12.08.2022.
//

#ifndef THREADED_AVL_TREE_TEMPLATEATI_H
#define THREADED_AVL_TREE_TEMPLATEATI_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_AVL_STACK_LENGTH_X64 93
#define MAX_AVL_STACK_LENGTH_X32 47

//#define TEMPLATE_AVL_KEY_LESS(A_KEY, B_KEY) ((A_KEY) < (B_KEY))
//#define TEMPLATE_AVL_ALLOCATE(TYPE) ((TYPE *)malloc(sizeof(TYPE)))
//#define TEMPLATE_AVL_ALLOCATE_ARRAY(TYPE, SIZE) ((TYPE *)malloc(sizeof(TYPE), SIZE))
//#define TEMPLATE_AVL_DELETE(ARG) (free(ARG))
//#define TEMPLATE_AVL_DELETE_ARRAY(TYPE, SIZE) (free(ARG))

typedef unsigned char tree_height_t;
typedef char tree_diff_t;



#define LIGHT_LEFT_ROTATION(NAME, DST, SRC, RIGHT)        \
    struct AvlNode##NAME##*tmp_0 = (RIGHT)->left_branch_; \
    (RIGHT)->left_branch_ = (SRC);                        \
    (SRC)->right_branch_ = tmp_0;                         \
    (DST) = (RIGHT)


#define LEFT_ROTATION(NAME, DST, SRC)                \
                                                     \
    struct AvlNode##NAME##*r = (SRC)->right_branch_; \
    LIGHT_LEFT_ROTATION(NAME, DST, SRC, r);



#define LIGHT_RIGHT_ROTATION(NAME, DST, SRC, LEFT)        \
    struct AvlNode##NAME##*tmp_0 = (LEFT)->right_branch_; \
    (LEFT)->right_branch_ = (SRC);                        \
    (SRC)->left_branch_ = tmp_0;                          \
    (DST) = (LEFT)

#define RIGHT_ROTATION(NAME, DST, SRC)              \
                                                    \
    struct AvlNode##NAME##*l = (SRC)->left_branch_; \
    LIGHT_RIGHT_ROTATION(NAME, DST, SRC, l);


#define defineAvlTree(NAME, DATA_TYPE, KEY_TYPE)                                                                     \
    typedef KEY_TYPE tree_key_##NAME##_t;                                                                            \
    typedef DATA_TYPE tree_data_##NAME##_t;                                                                          \
                                                                                                                     \
                                                                                                                     \
    struct AvlNode##NAME##{                                                                                          \
        tree_key_##NAME##_t key_;                                                                                    \
        tree_data_##NAME##_t data_;                                                                                  \
                                                                                                                     \
        struct AvlNode##NAME##*left_branch_;  /**< key_ of left_branch_ is less then current */                      \
        struct AvlNode##NAME##*right_branch_; /**< key_ of right_branch_ is more then current */                     \
        struct AvlNode##NAME##*previous_;     /**< key_ of previous_ is less then current */                         \
        struct AvlNode##NAME##*next_;         /**< key_ of next_ is more then current */                             \
                                                                                                                     \
        tree_height_t height_; /**< equal 1 when it has no branches*/                                                \
    };                                                                                                               \
                                                                                                                     \
    typedef struct AvlNode##NAME##**avlNode##NAME##StackItem_t;                                                      \
    typedef avlNode##NAME##StackItem_t *avlNode##NAME##Stack_t;                                                      \
                                                                                                                     \
                                                                                                                     \
    /**                                                                                                              \
     * type of function which removes only key and data without removing node                                        \
     */                                                                                                              \
    typedef void (*deleteKeyAndDataF_t)(struct AvlNode##NAME##*);                                                    \
                                                                                                                     \
                                                                                                                     \
    struct AvlTree##NAME##{                                                                                          \
        struct AvlNode##NAME##*top_node_;                                                                            \
        size_t size_;                                                                                                \
        struct AvlNode##NAME##*first_node_;                                                                          \
        struct AvlNode##NAME##*last_node_;                                                                           \
                                                                                                                     \
        /* first element of node stack always equal NULL  */                                                         \
        avlNode##NAME##Stack_t stack_;                                                                               \
    };                                                                                                               \
                                                                                                                     \
    struct AvlTree##NAME##*newAvlTree##NAME##();                                                                     \
                                                                                                                     \
    /**                                                                                                              \
     * frees the memory allocated for node structure.                                                                \
     * It doesn't free the memory allocated for key_ and data_                                                       \
     */                                                                                                              \
    void deleteAvlNode##NAME(struct AvlNode##NAME##*node);                                                           \
                                                                                                                     \
    /**                                                                                                              \
     * if equal key exists it removes node from tree structure and returns pointer to the corresponding node         \
     */                                                                                                              \
    struct AvlNode##NAME##*findAvlNode##NAME(struct AvlTree##NAME##*avl_tree, tree_key_##NAME##_t key);              \
                                                                                                                     \
    /**                                                                                                              \
     *  if equal key exists it returns node pointer without insertion                                                \
     * (developer responsible for changing data/insertion.), otherwise it creates a new                              \
     * node, inserts that node into the, balances the tree and returns NULL.                                         \
     */                                                                                                              \
    struct AvlNode##NAME##*insertAvlNode##NAME(struct AvlTree##NAME##*avl_tree, const tree_key_##NAME##_t *key_p,    \
                                               tree_data_##NAME##_t *data);                                          \
                                                                                                                     \
    /**                                                                                                              \
     *  if equal key exists it removes node from tree structure and returns pointer to the corresponding node        \
     *  ( developer is responsible for freeing memory), otherwise it returns NULL.                                   \
     */                                                                                                              \
    struct AvlNode##NAME##*removeAvlNode##NAME(struct AvlTree##NAME##*avl_tree, const tree_key_##NAME##_t *key_p);   \
                                                                                                                     \
                                                                                                                     \
                                                                                                                     \
    /* Create a node*/                                                                                               \
    struct AvlNode##NAME##*newAvlNode##NAME(tree_key_##NAME##_t key, tree_data_##NAME##_t data) {                    \
        /*struct AvlNode##NAME##*node = (struct AvlNode##NAME##*)malloc(sizeof(struct AvlNode##NAME)); */            \
        struct AvlNode##NAME##*node = TEMPLATE_AVL_ALLOCATE(struct AvlNode##NAME);                                   \
        node->key_ = key;                                                                                            \
        node->data_ = data;                                                                                          \
                                                                                                                     \
        node->left_branch_ = NULL;                                                                                   \
        node->right_branch_ = NULL;                                                                                  \
                                                                                                                     \
        node->right_branch_ = NULL;                                                                                  \
        node->next_ = NULL;                                                                                          \
                                                                                                                     \
        node->height_ = 1;                                                                                           \
                                                                                                                     \
        return (node);                                                                                               \
    }                                                                                                                \
                                                                                                                     \
    struct AvlNode##NAME##*newAvlNode##NAME##Empty() {                                                               \
        struct AvlNode##NAME##*node = TEMPLATE_AVL_ALLOCATE(struct AvlNode##NAME);                                   \
                                                                                                                     \
        node->left_branch_ = NULL;                                                                                   \
        node->right_branch_ = NULL;                                                                                  \
                                                                                                                     \
        node->right_branch_ = NULL;                                                                                  \
        node->next_ = NULL;                                                                                          \
                                                                                                                     \
        node->height_ = 1;                                                                                           \
                                                                                                                     \
        return (node);                                                                                               \
    }                                                                                                                \
                                                                                                                     \
    struct AvlTree##NAME##*newAvlTree##NAME##() {                                                                    \
        struct AvlTree##NAME##*avl_tree = TEMPLATE_AVL_ALLOCATE(struct AvlTree##NAME##);                             \
                                                                                                                     \
        avl_tree->stack_ =                                                                                           \
            (avlNode##NAME##Stack_t)malloc(sizeof(avlNode##NAME##StackItem_t) * MAX_AVL_STACK_LENGTH_X32);           \
        avl_tree->top_node_ = NULL;                                                                                  \
        avl_tree->first_node_ = NULL;                                                                                \
        avl_tree->last_node_ = NULL;                                                                                 \
        avl_tree->size_ = 0;                                                                                         \
        return avl_tree;                                                                                             \
    }                                                                                                                \
                                                                                                                     \
    struct AvlNode##NAME##*findAvlNode##NAME(struct AvlTree##NAME##*avl_tree, tree_key_##NAME##_t key) {             \
        struct AvlNode##NAME##*node = avl_tree->top_node_;                                                           \
        avlNode##NAME##Stack_t node_stack = avl_tree->stack_;                                                        \
                                                                                                                     \
        while (node != NULL) {                                                                                       \
            if (TEMPLATE_AVL_KEY_LESS(key, node->key_)) {                                                            \
                node = node->left_branch_;                                                                           \
            } else if (TEMPLATE_AVL_KEY_LESS(node->key_, key)) {                                                     \
                node = node->left_branch_;                                                                           \
            } else return node;                                                                                      \
        }                                                                                                            \
        return node;                                                                                                 \
    }                                                                                                                \
                                                                                                                     \
                                                                                                                     \
    void deleteAvlTree##NAME##(struct AvlTree##NAME## * avl_tree, deleteKeyAndDataF_t delete_data_f) {               \
        struct AvlNode##NAME##*node = avl_tree->top_node_;                                                           \
                                                                                                                     \
        if (node) {                                                                                                  \
            while (node->left_branch_) node = node->left_branch_;                                                    \
                                                                                                                     \
            while (node) {                                                                                           \
                struct AvlNode##NAME##*tmp = node;                                                                   \
                node = node->next_;                                                                                  \
                if (delete_data_f) delete_data_f(tmp);                                                               \
            }                                                                                                        \
        }                                                                                                            \
        TEMPLATE_AVL_DELETE_ARRAY(avl_tree->stack_);                                                                 \
        TEMPLATE_AVL_DELETE(avl_tree);                                                                               \
    }                                                                                                                \
                                                                                                                     \
    /*void deleteAvlNode##NAME(struct AvlNode##NAME##*node) { free(node); }   */                                     \
                                                                                                                     \
                                                                                                                     \
                                                                                                                     \
    /*                           *                                                                                   \
     *  diff > 0                 *                                                                                   \
     *         (simple rot)      *         (complex rot)                                                             \
     *     \                     *   b  c     a  b                                                                   \
     *      a  b       \ b  c    *  a \/       \/ c       a b c d                                                    \
     *       \/ c  ->   a \/     *   \/ d   ->  \/ d  ->   \/ \/                                                     \
     *        \/         \/      *    \/         \/         \ /                                                      \
     *                                                                                                               \
     *---------------------------+-----------------------------                                                      \
     */                                                                                                              \
                                                                                                                     \
                                                                                                                     \
    /**                                                                                                              \
     *  if equal key exists it returns node pointer without insertion (if equal key                                  \
     * exists, developer responsible for insertion), otherwise it creates a new                                      \
     * node, inserts that node into the, balances the tree and returns NULL.                                         \
     */                                                                                                              \
    struct AvlNode##NAME##*insertAvlNode##NAME(struct AvlTree##NAME##*avl_tree, const tree_key_##NAME##_t *key_p,    \
                                               tree_data_##NAME##_t *data) {                                         \
        avlNode##NAME##Stack_t node_stack = avl_tree->stack_;                                                        \
        avlNode##NAME##StackItem_t stack_item = &(avl_tree->top_node_);                                              \
                                                                                                                     \
        while (*stack_item != NULL) {                                                                                \
            *(++node_stack) = stack_item;                                                                            \
            struct AvlNode##NAME##*node = *stack_item;                                                               \
            if (TEMPLATE_AVL_KEY_LESS(*key_p, node->key_)) stack_item = &(node->left_branch_);                       \
            else if (TEMPLATE_AVL_KEY_LESS(node->key_, *key_p)) stack_item = &(node->right_branch_);                 \
            else return node;                                                                                        \
        }                                                                                                            \
                                                                                                                     \
        /* create node and fill it*/                                                                                 \
        struct AvlNode##NAME##*to_insert = newAvlNode##NAME##Empty();                                                \
        to_insert->key_ = *key_p;                                                                                    \
        to_insert->data_ = *data;                                                                                    \
                                                                                                                     \
        *stack_item = to_insert;                                                                                     \
                                                                                                                     \
        if (*(--node_stack) == NULL) return to_insert;                                                               \
                                                                                                                     \
                                                                                                                     \
                                                                                                                     \
        struct AvlNode##NAME##*node = **node_stack;                                                                  \
                                                                                                                     \
        if (node->left_branch_ == to_insert) {                                                                       \
            to_insert->next_ = node;                                                                                 \
            to_insert->previous_ = node->previous_;                                                                  \
            node->previous_ = to_insert;                                                                             \
        } else {                                                                                                     \
            to_insert->previous_ = node;                                                                             \
            to_insert->next_ = node->next_;                                                                          \
            node->next_ = to_insert;                                                                                 \
        }                                                                                                            \
                                                                                                                     \
                                                                                                                     \
                                                                                                                     \
        if (node->height_ > 1) return NULL;                                                                          \
        node->height_ = 2;                                                                                           \
        node = **(--node_stack);                                                                                     \
                                                                                                                     \
        if (node->height_ > 2) return NULL;                                                                          \
                                                                                                                     \
        if (node->right_branch_ == NULL) {                                                                           \
            struct AvlNode##NAME##*l = node->left_branch_;                                                           \
            if (l->right_branch_) {                                                                                  \
                struct AvlNode##NAME##*r = l->right_branch_;                                                         \
                l->right_branch_ = NULL;                                                                             \
                                                                                                                     \
                r->left_branch_ = l;                                                                                 \
                r->right_branch_ = node;                                                                             \
                                                                                                                     \
                l->height_ = 1;                                                                                      \
                r->height_ = 2;                                                                                      \
                **node_stack = r;                                                                                    \
            } else {                                                                                                 \
                l->right_branch_ = node;                                                                             \
                **node_stack = l;                                                                                    \
            }                                                                                                        \
                                                                                                                     \
            node->left_branch_ = NULL;                                                                               \
            node->height_ = 1;                                                                                       \
            return NULL;                                                                                             \
        } else if (node->left_branch_ == NULL) {                                                                     \
            struct AvlNode##NAME##*r = node->right_branch_;                                                          \
            if (r->left_branch_) {                                                                                   \
                struct AvlNode##NAME##*l = r->left_branch_;                                                          \
                r->left_branch_ = NULL;                                                                              \
                                                                                                                     \
                l->right_branch_ = r;                                                                                \
                l->left_branch_ = node;                                                                              \
                                                                                                                     \
                r->height_ = 1;                                                                                      \
                l->height_ = 2;                                                                                      \
                **node_stack = l;                                                                                    \
            } else {                                                                                                 \
                r->left_branch_ = node;                                                                              \
                **node_stack = r;                                                                                    \
            }                                                                                                        \
                                                                                                                     \
            node->right_branch_ = NULL;                                                                              \
            node->height_ = 1;                                                                                       \
            return NULL;                                                                                             \
        }                                                                                                            \
                                                                                                                     \
        node->height_ = 3;                                                                                           \
        /* node = **(--node_stack);*/                                                                                \
                                                                                                                     \
        while (*(--node_stack)) {                                                                                    \
            node = **node_stack;                                                                                     \
                                                                                                                     \
            int diff = node->left_branch_->height_ - node->right_branch_->height_;                                   \
            if (diff > 0) {                                                                                          \
                if (diff == 1) node->height_ = node->left_branch_->height_ + 1;                                      \
                else {                                                                                               \
                    struct AvlNode##NAME##*l =                                                                       \
                        node->left_branch_; /* TODO define 'r' here, and simplify comparison and LEFT_ROTATION*/     \
                    if (l->left_branch_->height_ < l->right_branch_->height_) { /*(simple rot)*/                     \
                        LEFT_ROTATION(NAME, node->left_branch_, l);                                                  \
                                                                                                                     \
                        l->height_ = r->height_;                                                                     \
                        r->height_ = node->height_;                                                                  \
                        l = r;                                                                                       \
                    }                                                                                                \
                    LIGHT_RIGHT_ROTATION(NAME, **node_stack, node, l);                                               \
                                                                                                                     \
                    node->height_ = node->height_ - 1;                                                               \
                                                                                                                     \
                    return NULL;                                                                                     \
                }                                                                                                    \
            } else if (diff < 0) {                                                                                   \
                if (diff == -1) node->height_ = node->right_branch_->height_ + 1;                                    \
                else {                                                                                               \
                    struct AvlNode##NAME##*r = node->right_branch_;                                                  \
                    if (r->right_branch_->height_ < r->left_branch_->height_) { /*(simple rot)*/                     \
                        RIGHT_ROTATION(NAME, node->right_branch_, r);                                                \
                                                                                                                     \
                        r->height_ = l->height_;                                                                     \
                        l->height_ = node->height_;                                                                  \
                        r = l;                                                                                       \
                    }                                                                                                \
                    LIGHT_LEFT_ROTATION(NAME, **node_stack, node, r);                                                \
                                                                                                                     \
                    node->height_ = node->height_ - 1;                                                               \
                                                                                                                     \
                    return NULL;                                                                                     \
                }                                                                                                    \
            } else return NULL;                                                                                      \
                                                                                                                     \
            /* node = **(node_stack--);*/                                                                            \
        }                                                                                                            \
                                                                                                                     \
        return NULL;                                                                                                 \
    }                                                                                                                \
                                                                                                                     \
                                                                                                                     \
    struct AvlNode##NAME##*removeAvlNode##NAME(struct AvlTree##NAME##*avl_tree, const tree_key_##NAME##_t *key_p) {  \
        avlNode##NAME##Stack_t node_stack = avl_tree->stack_;                                                        \
        avlNode##NAME##StackItem_t stack_item = &(avl_tree->top_node_);                                              \
                                                                                                                     \
                                                                                                                     \
        struct AvlNode##NAME##*node_to_delete = *stack_item;                                                         \
                                                                                                                     \
        while (node_to_delete != NULL) {                                                                             \
            *(++node_stack) = stack_item;                                                                            \
                                                                                                                     \
            if (TEMPLATE_AVL_KEY_LESS(*key_p, node_to_delete->key_)) stack_item = &(node_to_delete->left_branch_);   \
            else if (TEMPLATE_AVL_KEY_LESS(node_to_delete->key_, *key_p))                                            \
                stack_item = &(node_to_delete->right_branch_);                                                       \
            else break;                                                                                              \
            node_to_delete = *stack_item;                                                                            \
        }                                                                                                            \
        if (node_to_delete == NULL) return NULL;                                                                     \
                                                                                                                     \
                                                                                                                     \
        /**/                                                                                                         \
        if (node_to_delete->next_) node_to_delete->next_->previous_ = node_to_delete->previous_;                     \
                                                                                                                     \
        if (node_to_delete->previous_) node_to_delete->previous_->next_ = node_to_delete->next_;                     \
                                                                                                                     \
        /* struct AvlNode##NAME ## * const node_to_delete = node;*/                                                  \
                                                                                                                     \
        /* currently, *stack_item points the node to be removed, so **(node_stack-1) points to the previous node.*/  \
                                                                                                                     \
        tree_height_t h = node_to_delete->height_;                                                                   \
                                                                                                                     \
        if (node_to_delete->left_branch_ == NULL) {                                                                  \
            *stack_item = node_to_delete->right_branch_;                                                             \
            /* if (node_to_delete->right_branch_ == NULL) {*/                                                        \
            /*     stack_item = *(--node_stack);*/                                                                   \
            /*     struct AvlNode##NAME ## *tmp = *stack_item;*/                                                     \
            /*     if ((tmp->left_branch_ == NULL) || (tmp->right_branch_ == NULL)) tmp->height_ = 1;*/              \
            /* }*/                                                                                                   \
        } else if (node_to_delete->right_branch_ == NULL) {                                                          \
            *stack_item = node_to_delete->left_branch_;                                                              \
        } else {                                                                                                     \
            avlNode##NAME##StackItem_t *stack_pos_to_insert = ++node_stack;                                          \
                                                                                                                     \
            struct AvlNode##NAME##*tmp = node_to_delete->left_branch_;                                               \
            while (tmp->right_branch_) {                                                                             \
                *(++node_stack) = &(tmp->right_branch_);                                                             \
                tmp = tmp->right_branch_;                                                                            \
            }                                                                                                        \
                                                                                                                     \
            /* *node_stack points to the replaceable node*/                                                          \
            h = tmp->height_;                                                                                        \
            /* replacing nodes  begin*/                                                                              \
            *stack_pos_to_insert = &(tmp->left_branch_);                                                             \
            tmp->right_branch_ = node_to_delete->right_branch_;                                                      \
            **node_stack = tmp->left_branch_;                                                                        \
            tmp->left_branch_ = node_to_delete->left_branch_;                                                        \
            *stack_item = tmp;                                                                                       \
            /* replacing nodes  finish*/                                                                             \
                                                                                                                     \
            stack_item = *node_stack;                                                                                \
        }                                                                                                            \
                                                                                                                     \
        if (*(--node_stack) == NULL) return node_to_delete;                                                          \
                                                                                                                     \
        struct AvlNode##NAME##*node = **node_stack;                                                                  \
        if (h == 1) {                                                                                                \
            if (node->height_ == 2) {                                                                                \
                if (node->left_branch_ == NULL && node->right_branch_ == NULL) node->height_ = 1;                    \
            } else if (node->left_branch_) {                                                                         \
                struct AvlNode##NAME##*l = node->left_branch_;                                                       \
                if (l->right_branch_) {                                                                              \
                    struct AvlNode##NAME##*r = l->right_branch_;                                                     \
                    l->right_branch_ = NULL;                                                                         \
                                                                                                                     \
                    r->left_branch_ = l;                                                                             \
                    r->right_branch_ = node;                                                                         \
                                                                                                                     \
                    /* TODO: probably, using l->height_ = l->left_branch_? 2 :1;  instead of line below is better*/  \
                    if (l->left_branch_ == NULL) l->height_ = 1;                                                     \
                    r->height_ = l->height_ + 1;                                                                     \
                    **node_stack = r;                                                                                \
                } else {                                                                                             \
                    l->right_branch_ = node;                                                                         \
                    **node_stack = l;                                                                                \
                }                                                                                                    \
                                                                                                                     \
                node->left_branch_ = NULL;                                                                           \
                node->height_ = 1;                                                                                   \
                                                                                                                     \
            } else {                                                                                                 \
                struct AvlNode##NAME##*r = node->right_branch_;                                                      \
                if (r->left_branch_) {                                                                               \
                    struct AvlNode##NAME##*l = r->left_branch_;                                                      \
                    r->left_branch_ = NULL;                                                                          \
                                                                                                                     \
                    l->right_branch_ = r;                                                                            \
                    l->left_branch_ = node;                                                                          \
                                                                                                                     \
                    /* TODO: probably, using r->height_ = r->right_branch_? 2 :1;  instead of line below is better*/ \
                    if (r->right_branch_ == NULL) r->height_ = 1;                                                    \
                    l->height_ = r->height_ + 1;                                                                     \
                    **node_stack = l;                                                                                \
                } else {                                                                                             \
                    r->left_branch_ = node;                                                                          \
                    **node_stack = r;                                                                                \
                }                                                                                                    \
                                                                                                                     \
                node->right_branch_ = NULL;                                                                          \
                node->height_ = 1;                                                                                   \
            }                                                                                                        \
        }                                                                                                            \
                                                                                                                     \
                                                                                                                     \
        while (*(--node_stack)) {                                                                                    \
            node = **node_stack;                                                                                     \
                                                                                                                     \
            int diff = node->left_branch_->height_ - node->right_branch_->height_;                                   \
            if (diff > 0) {                                                                                          \
                if (diff == 1) node->height_ = node->left_branch_->height_ + 1;                                      \
                else {                                                                                               \
                    struct AvlNode##NAME##*l = node->left_branch_;                                                   \
                    tree_height_t h_tmp = node->right_branch_->height_;                                              \
                    /* TODO define 'r' here, and simplify comparison and LEFT_ROTATION*/                             \
                    if (l->left_branch_->height_ < l->right_branch_->height_) { /*(simple rot)*/                     \
                        LEFT_ROTATION(NAME, node->left_branch_, l);                                                  \
                                                                                                                     \
                        l->height_ = r->height_;                                                                     \
                        r->height_ = r->height_ + 1;                                                                 \
                        l = r;                                                                                       \
                    }                                                                                                \
                    LIGHT_RIGHT_ROTATION(NAME, **node_stack, node, l);                                               \
                                                                                                                     \
                    node->height_ = h_tmp + 1;                                                                       \
                }                                                                                                    \
            } else if (diff < 0) {                                                                                   \
                if (diff == -1) node->height_ = node->right_branch_->height_ + 1;                                    \
                else {                                                                                               \
                    struct AvlNode##NAME##*r = node->right_branch_;                                                  \
                    tree_height_t h_tmp = node->left_branch_->height_;                                               \
                    if (r->right_branch_->height_ < r->left_branch_->height_) { /*(simple rot)*/                     \
                        RIGHT_ROTATION(NAME, node->right_branch_, r);                                                \
                                                                                                                     \
                        r->height_ = l->height_;                                                                     \
                        l->height_ = l->height_ + 1;                                                                 \
                        r = l;                                                                                       \
                    }                                                                                                \
                    LIGHT_LEFT_ROTATION(NAME, **node_stack, node, r);                                                \
                                                                                                                     \
                    node->height_ = h_tmp + 1;                                                                       \
                }                                                                                                    \
            } else return NULL;                                                                                      \
                                                                                                                     \
            /* node = **(node_stack--);*/                                                                            \
        }                                                                                                            \
                                                                                                                     \
        return node_to_delete;                                                                                       \
    }                                                                                                                \
    struct AvlNode##NAME##*removeAvlNode##NAME(struct AvlTree##NAME##*avl_tree, const tree_key_##NAME##_t *key_p);


#endif  // THREADED_AVL_TREE_TEMPLATEATI_H
