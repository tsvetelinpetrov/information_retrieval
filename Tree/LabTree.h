#pragma once

#include <iostream>
#include <fstream>
#include "LabTree.h"

typedef int node_data;
typedef struct node {
	node_data item;
	struct node* parent;
	struct node* left;
	struct node* right;
};

/**
  * @brief Initializing the root element with data from a file.
  *
  * @param node** - The tree root in which the numbers will be added.
  * @param char* file - The file location form which the data will be extracted.
  *
  * @return void
  */
void init_from_file(node**, const char*);


/**
  * @brief Adding a node to the tree root
  *
  * @param node** - The tree root in which the node will be added.
  * @param node_data* - The node element which will be added to the tree.
  * @param node* - The parent node to which the node element will be added.
  *
  * @return void
  */
void add_node(node** t, node_data item, node* parent);


/**
  * @brief Adding a node to the tree root
  *
  * @param node** - The tree root in which the node will be added.
  * @param node_data* - The node element which will be added to the tree.
  * @param node* - The parent node to which the node element will be added.
  *
  * @return void
  */
node* find_node(node* t, node_data i);


/**
  * @brief Searching for the minimal node element in the tree
  *
  * @param node** - The tree root in which the minimal element will be searched.
  *
  * @return node*
  */
node* find_min(node* t);


/**
  * @brief Searching for the maximal node element in the tree
  *
  * @param node** - The tree root in which the maximal element will be searched.
  *
  * @return node*
  */
node* find_max(node* t);


/**
  * @brief Deleting a node from a tree
  *
  * @param node** - The tree root in which the node element will be deleted.
  * @param node_data - The node which will be deleted from the tree.
  *
  * @return void
  */
void del_node(node* t, node_data item);


/**
  * @brief Printing the tree in a preordered order
  *
  * @param node** - The tree root in which will be printed
  *
  * @return void
  */
void preorder(node* t);


/**
  * @brief Printing the tree in a inordered order
  *
  * @param node** - The tree root in which will be printed
  *
  * @return void
  */
void inorder(node* t);


/**
  * @brief Printing the tree in a postordered order
  *
  * @param node** - The tree root in which will be printed
  *
  * @return void
  */
void postorder(node* t);


/**
  * @brief Returning the levels number of the tree
  *
  * @param node** - The tree root
  *
  * @return int
  */
int numLevels(node* t);