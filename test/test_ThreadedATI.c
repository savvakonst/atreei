//
// Created by SVK on 18.07.2022.
//
#include "ThreadedATI.h"
//
#include "test_ati_helper.h"

#define DATA_SIZE 1000



typedef uint64_t test_data_type_t;
#include <string.h>
int main(int argc, char* argv[]) {
    struct Ati* tree = newAti();

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
        //        } while (findAtiNode(tree, name) != NULL);

        test_data_type_t random = (unsigned long)rand();  // NOLINT(cert-msc30-c, cert-msc50-cpp)
        *(ptr++) = random;
        PRINT_F("random N: %llu\n", random);
        insertAtiNode(tree, (const tree_key_t*)&random, NULL);


        char* buf;
        size_t len;
        static char buffer[] = "foobar";
        // fopen_s()

        // FILE* stream = open_memstream(&buf, &len);
        // if (walkValidate(tree->top_node_))
        walkValidate(tree->top_node_);
        // printTreeDot(tree->top_node_, stdout);
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

    {
        struct AtiNode* node = getFirstAtiNode(tree);
        size_t cnt = 0;
        while (node) {
            PRINT_F("%04d   %06llu \n", cnt, (uint64_t)node->key_);
            size_t index = -1;
            findAtiNodeWithIndex(tree, node->key_, &index);
            if (findAtiNodeByIndex(tree, cnt) != node)
                PRINT_F("\033[0;31m error findAtiNodeByIndex(tree,cnt) != node:   \033[0m\n");
            if (index != cnt) PRINT_F("\033[0;31m error index != cnt: %llu != %llu  \033[0m\n", index, cnt);

            node = node->next_;
            cnt++;
        }
    }

    if (1) {
        char cond = 1;
        for (int i = 0; i < DATA_SIZE; i++) {
            test_data_type_t temp_data = *(data_array + i);
            removeAtiNode(tree, &temp_data);

            PRINT_F("deletion N: %d\n", i);
            // walkValidate(tree->top_node_);
            if (cond && walkValidate(tree->top_node_)) cond = 0;
            // PRINT_F(" \n");
        }
    } else {
        struct AtiNode* node = getFirstAtiNode(tree);
        walkValidate(tree->top_node_);
        int cnt = 0;
        while (node && cnt < 20) {
            node = node->next_;
            removeAtiNode(tree, &node->previous_->key_);

            PRINT_F("deletion N: %d\n", cnt);

            walkValidate(tree->top_node_);

            PRINT_F(" \n");

            cnt++;
        }
    }

    PRINT_F(" --------------------------------\n");



    deleteAti(tree, NULL);



    return 0;
}