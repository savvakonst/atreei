//
// Created by SVK on 07.09.2022.
//

#ifndef ATI_TEST_ATI_HELPER_H
#define ATI_TEST_ATI_HELPER_H


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PRINT_F(...)         \
    {                        \
        printf(__VA_ARGS__); \
        fflush(stdout);      \
    }

int walkPrint(struct AtiNode* node) {
    if (node->left_branch_) walkPrint(node->left_branch_);

    PRINT_F("w %04lld \n", (uint64_t)node->key_);


    if (node->right_branch_) walkPrint(node->right_branch_);
}


int walkValidate_(struct AtiNode* node, int* cnt) {
    if (node == NULL) return 0;

    int status = 0;
    if (node->left_branch_) status |= walkValidate_(node->left_branch_, cnt);


    char left = node->left_branch_ ? node->left_branch_->height_ : 0;
    char right = node->right_branch_ ? node->right_branch_->height_ : 0;


    if ((max(left, right) + 1) != (node->height_)) {
        PRINT_F("\033[0;31m height_ error on : pos= %d  (top= %d  left= %d  right= %d ) \033[0m\n", (*cnt),
                node->height_, left, right);
        status = status | 0b0001;
    }

#ifdef SIZE_SUPPORT
    size_t left_size = node->left_branch_ ? node->left_branch_->key_ : 0;
    size_t right_size = node->right_branch_ ? node->right_branch_->key_ : 0;

    if (left_size + right_size != node->key_ - 1) {
        PRINT_F("\033[0;31m size_ error on : pos= %d  (top= %llu  left= %llu  right= %llu ) \033[0m\n", (*cnt),
                node->key_, left_size, right_size);
        status = status | 0b0010;
    }
#endif

    if (node->right_branch_) status |= walkValidate_(node->right_branch_, cnt);

    (*cnt)++;
    return status;
}

int walkValidate(struct AtiNode* node) {
    int c = 0;
    return walkValidate_(node, &c);
}



void printNodeDot_(struct AtiNode* node, FILE* stream) {
    fprintf(stream, "    %llu [label = \"%llu, %llu\" ];\n", (uint64_t)node->key_, (uint64_t)node->key_,
            (uint64_t)node->height_);

    if (node->left_branch_) {
        fprintf(stream, "    %llu -> %llu;\n", (uint64_t)node->key_, (uint64_t)node->left_branch_->key_);
        printNodeDot_(node->left_branch_, stream);
    }

    if (node->right_branch_) {
        fprintf(stream, "    %llu -> %llu;\n", (uint64_t)node->key_, (uint64_t)node->right_branch_->key_);
        printNodeDot_(node->right_branch_, stream);
    }
}

void printTreeDot(struct AtiNode* top_node, FILE* stream) {
    fprintf(stream, "digraph BinaryTree {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n rankdir=\"BT\";\n");

    if (!top_node) fprintf(stream, "\n");
    else if (!top_node->right_branch_ && !top_node->left_branch_)
        fprintf(stream, "    %llu;\n", (uint64_t)top_node->key_);
    else printNodeDot_(top_node, stream);

    fprintf(stream, "}\n");
}

#endif  // ATI_TEST_ATI_HELPER_H
