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
#include <wchar.h>

#define KEY_SIZE 1

#define DATA_SIZE 40


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


int walkValidate_(struct AvlNode* node, int* cnt) {
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
    char left_size = node->left_branch_ ? node->left_branch_->size_ : 0;
    char right_size = node->right_branch_ ? node->right_branch_->size_ : 0;

    if (left_size + right_size != node->size_ - 1) {
        PRINT_F("\033[0;31m size_ error on : pos= %d  (top= %d  left= %d  right= %d ) \033[0m\n", (*cnt), node->size_,
                left_size, right_size);
        status = status | 0b0010;
    }
#endif

    if (node->right_branch_) status |= walkValidate_(node->right_branch_, cnt);

    (*cnt)++;
    return status;
}

int walkValidate(struct AvlNode* node) {
    int c = 0;
    return walkValidate_(node, &c);
}



void printNodeDot_(struct AvlNode* node, FILE* stream) {
    fprintf(stream, "    %llu [label = \"%llu, %llu\" ];\n", (uint64_t)node->key_, (uint64_t)node->key_,
            (uint64_t)node->size_);

    if (node->left_branch_) {
        fprintf(stream, "    %llu -> %llu;\n", (uint64_t)node->key_, (uint64_t)node->left_branch_->key_);
        printNodeDot_(node->left_branch_, stream);
    }

    if (node->right_branch_) {
        fprintf(stream, "    %llu -> %llu;\n", (uint64_t)node->key_, (uint64_t)node->right_branch_->key_);
        printNodeDot_(node->right_branch_, stream);
    }
}

void printTreeDot(struct AvlNode* top_node, FILE* stream) {
    fprintf(stream, "digraph BinaryTree {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n rankdir=\"BT\";\n");

    if (!top_node) fprintf(stream, "\n");
    else if (!top_node->right_branch_ && !top_node->left_branch_)
        fprintf(stream, "    %llu;\n", (uint64_t)top_node->key_);
    else printNodeDot_(top_node, stream);

    fprintf(stream, "}\n");
}



typedef uint64_t test_data_type_t;
#include <string.h>
int main(int argc, char* argv[]) {
    struct AvlTree* tree = newAvlTree();

    test_data_type_t* data_array = malloc(sizeof(test_data_type_t) * DATA_SIZE);
    test_data_type_t* ptr = data_array;
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

        test_data_type_t random = (unsigned long)rand();  // NOLINT(cert-msc30-c, cert-msc50-cpp)
        *(ptr++) = random;
        PRINT_F("random N: %llu\n", random);
        insertAvlNode(tree, (const tree_key_t*)&random, NULL);


        char* buf;
        size_t len;
        static char buffer[] = "foobar";
        // fopen_s()

        // FILE* stream = open_memstream(&buf, &len);
        // if (walkValidate(tree->top_node_))
        walkValidate(tree->top_node_);
        printTreeDot(tree->top_node_, stdout);
    }
    PRINT_F(" --------------------------------\n");
    // walkValidate(tree->top_node_);

    PRINT_F(" --------------------------------\n");

    for (int i = DATA_SIZE; i > 0;) {
        uint64_t random_index = ((unsigned long)rand()) % i;  // NOLINT(cert-msc30-c, cert-msc50-cpp)
        i--;
        test_data_type_t temp_data = *(data_array + i);
        *(data_array + i) = *(data_array + random_index);
        *(data_array + random_index) = temp_data;
    }



    for (int i = 0; i < DATA_SIZE; i++) {
        PRINT_F("random N: %llu\n", *(data_array + i));
    }

    PRINT_F(" --------------------------------\n");


    walkPrint(tree->top_node_);
    PRINT_F(" --------------------------------\n");



    if (1) {
        for (int i = 0; i < DATA_SIZE; i++) {
            test_data_type_t temp_data = *(data_array + i);
            removeAvlNode(tree, &temp_data);

            PRINT_F("deletion N: %d\n", i);
            // walkValidate(tree->top_node_);


            PRINT_F(" \n");
        }
    } else {
        struct AvlNode* node = getFirstAvlNode(tree);
        walkValidate(tree->top_node_);
        int cnt = 0;
        while (node && cnt < 20) {
            node = node->next_;
            removeAvlNode(tree, &node->previous_->key_);

            PRINT_F("deletion N: %d\n", cnt);

            walkValidate(tree->top_node_);

            PRINT_F(" \n");

            cnt++;
        }
    }

    PRINT_F(" --------------------------------\n");


    {
        struct AvlNode* node = getFirstAvlNode(tree);
        int cnt = 0;
        while (node) {
            PRINT_F("%04d   %06llu \n", cnt, (uint64_t)node->key_);

            node = node->next_;
            cnt++;
        }
    }


    deleteAvlTree(tree, NULL);



    return 0;
}