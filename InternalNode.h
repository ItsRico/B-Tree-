  #ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  bool checkFull();
  int findNodeIndex(BTreeNode *node1);
  int getMinimum()const;
  BTreeNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insertNode(BTreeNode *node1);
  InternalNode* insertValue(int value);
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void insertLeft(BTreeNode *node1);
  void insertRight(BTreeNode *node1);
  void refresh();
  void removeNode(BTreeNode *node1);
  BTreeNode* returnChild(BTreeNode* node1);
  void print(Queue <BTreeNode*> &queue);
  InternalNode* split(BTreeNode *node1);
}; // InternalNode class

#endif
