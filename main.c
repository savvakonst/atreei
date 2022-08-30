//
// Created by SVK on 18.07.2022.
//
#include "ThreadedAvlTree.h"
//
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>

#define KEY_SIZE 1

#define DATA_SIZE 50


#define PRINT_F(...)         \
    {                        \
        printf(__VA_ARGS__); \
        fflush(stdout);      \
    }

int walkPrint(struct AvlNode* node) {
    if (node->left_branch_) walkPrint(node->left_branch_);

    PRINT_F("w %04d \n", (uint64_t)node->key_);


    if (node->right_branch_) walkPrint(node->right_branch_);
}

int walkValidateHeight(struct AvlNode* node, int* cnt) {
    if (node->left_branch_) walkValidateHeight(node->left_branch_, cnt);
    char left = node->left_branch_ ? node->left_branch_->height_ : 0;
    char right = node->right_branch_ ? node->right_branch_->height_ : 0;
    if ((max(left, right) + 1) != (node->height_)) {
        PRINT_F("\033[0;31m error on : pos= %d  (top= %d  left= %d  right= %d ) \033[0m\n", (*cnt), node->height_, left,
                right);
    }
    if (node->right_branch_) walkValidateHeight(node->right_branch_, cnt);

    (*cnt)++;
    return 0;
}



int main(int argc, char* argv[]) {
    struct AvlTree* tree = newAvlTree();

    for (int i = 0; i < DATA_SIZE; i++) {
        // unsigned char* name = (unsigned char*)malloc(KEY_SIZE+1);
        // do {
        //     for (int j = 0; j < KEY_SIZE; j++)
        //         name[j] = 'A' + (unsigned long)rand() * (unsigned long)('Z' - 'A') / RAND_MAX;
        //         //name[j] ='A' + (unsigned char)((unsigned char)rand() % ('Z' - 'A'));  // NOLINT(cert-msc30-c,
        //         cert-msc50-cpp)
        //
        //            name[KEY_SIZE] = 0;
        //        } while (findAvlNode(tree, name) != NULL);
        uint64_t random = (unsigned long)rand();

        insertAvlNode(tree, &random, NULL);
    }



    struct AvlNode* node = getFirstAvlNode(tree);
    walkPrint(tree->top_node_);
    PRINT_F(" --------------------------------\n");

    {
        int c = 0;
        walkValidateHeight(tree->top_node_, &c);
    }

    PRINT_F(" --------------------------------\n");

    int cnt = 0;
    while (node && cnt < 20) {
        // printf("%04d", (uint64_t)node->key_);
        // printf("   %d \n",cnt);

        node = node->next_;
        removeAvlNode(tree, &node->previous_->key_);
        int c = 0;

        PRINT_F("deletion N: %d\n", cnt);



        walkValidateHeight(tree->top_node_, &c);

        PRINT_F(" \n");



        cnt++;
    }


    PRINT_F(" --------------------------------\n");


    node = getFirstAvlNode(tree);
    cnt = 0;
    while (node) {
        PRINT_F("%04d   %04d \n", cnt, (uint64_t)node->key_);

        node = node->next_;
        cnt++;
    }


    deleteAvlTree(tree, NULL);



    return 0;
}