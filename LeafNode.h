#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  bool checkFull();
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void insertLeft(int value);
  void insertRight(int value);
  void insertValue(int *values, int value);
  LeafNode* split(int value);
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
