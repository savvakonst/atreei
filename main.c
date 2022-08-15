//
// Created by SVK on 18.07.2022.
//
#include "ThreadedAvlTree.h"
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>


int main(int argc, char* argv[]) {
    struct AvlTree* tree = newAvlTree();


    deleteAvlTree(tree, NULL);
    return 0;
}