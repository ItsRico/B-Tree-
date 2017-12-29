#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::findNodeIndex(BTreeNode *node1)
{
  int i;
  for (i = 0; i < internalSize; i++)
  if (children[i]->getMinimum() == node1->getMinimum())
  return i;
  return 0;
} //checkFull

int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


BTreeNode* InternalNode::insert(int value)
{
   BTreeNode *node1;
   
  int i;
  if (keys[0] > value) //if value belongs in first child
    node1 = children[0]->insert(value);
  else
  {
    for (i = 1; i < count; i++)//starting at second child, insert into child before if min is greater
    {
      if (keys[i] > value)
      {
        node1 = children[i-1]->insert(value);
        break;
      }
      
    }
    if (i == count)
    node1 = children[i-1]->insert(value);//if loop plays through, it belongs in last node
  }
  
  if ((node1) && (count < internalSize))//if new root needs to happen
  {
    insertNode(node1);
  }
  else if ((node1) && (count == internalSize))
  {
    if (((leftSibling) && !(leftSibling->getCount() == internalSize)))
    {
      insertLeft(node1);
    }
    else if ((rightSibling) && !(rightSibling->getCount() == internalSize))
    {
      insertRight(node1);
    }
    else 
    return split (node1); 
  }
  refresh();
  return NULL; // to avoid warnings for now.
}

 

void InternalNode::insertLeft(BTreeNode* node1)
{
  int i;
  ((InternalNode*)leftSibling)->insertNode(children[0]);//insert min into the left sibling
  removeNode(children[0]);
  count--;//decrement count + replace with null
  
  for (i = 0; i < count; i++) //shifts everything over to the left 1
  {
  children[i] = children[i+1];
  }
  children[count] = '\0';
  insertNode(node1); // inserts value
}//insertLeft

void InternalNode::insertRight(BTreeNode* node1)
{
  if (node1->getMinimum() > children[count-1]->getMinimum())
  {
      ((InternalNode*)(rightSibling))->insertNode(node1);
      return;
  }
    
  ((InternalNode*)(rightSibling))->insertNode(children[count-1]); //inserts max into right sibling and decrements count
  count--;
  removeNode(children[count]); //replaces max with null
  insertNode(node1);//inserts new value
}//insertRight
InternalNode* InternalNode::insertValue(int value)
{
 return NULL;
} // InternalNode::insertValue()

void InternalNode::insertNode(BTreeNode *node1) //inserts new node into children
{
  int i;
  BTreeNode* temp;
  node1->setParent(this);
  for (i = 0; i < internalSize; i++)
  {
    if (!(children[i])) // if child doesnt exist yet
      break;
    if ((node1->getMinimum() < (children[i]->getMinimum())))//if minimum of node is smaller than current child
    {
  
    temp = children[i]; // new node is children[i]
    children[i] = node1; 
    if (children[i+1])
      children[i]->setRightSibling(children[i+1]);

    if (children[i-1] && (i > 1))
      children[i]->setLeftSibling(children[i-1]);
    
    keys[i] = node1->getMinimum();//add new key
    node1 = temp;
    }
  }
  children[i] = node1; //last spot will be last value
  keys[i] = children[i]->getMinimum();
  count++;//increment count
  refresh();
  return;
  
} // LeafNode::reorder



void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ 
  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count =2;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  // students may write this
} // InternalNode::insert()

void InternalNode::refresh()
{
  
  int i;//updates all keys
  for (i = 0; i < internalSize; i++)
  {
    if (keys[i])
    {
    keys[i] = children[i]->getMinimum  ();
    }
    if (rightSibling)
    ((InternalNode*)(rightSibling))->refresh();
  }
  
  return;
}
void InternalNode::removeNode(BTreeNode *node1)
{
  int j = findNodeIndex(node1);
  int i;
  //children[j-1]->setRightSibling(NULL); //'removes node' from children for now
  children[j] = '\0';
  keys[j] = '\0';
  
  refresh();
}
void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()
InternalNode* InternalNode::split(BTreeNode *node1)
{
  
  int i; 
  InternalNode *sibling = new InternalNode(internalSize,leafSize, parent, this ,rightSibling);//make new sibling to the right of current
  for (i = internalSize +1 ; i > (((internalSize+1)/2)+1); i--)//insert at least last value, will have half 
  {
    
    sibling->insertNode(children[i-2]);
    removeNode(children[i-2]);
    
    
    count--;//decrement count
  }
  
  if (node1->getMinimum() < keys[count-1]) //if new value belongs in original node and not new one
  {
    sibling->insertNode(children[count-1]);//add one more to the new sibling, then insert new
    removeNode(children[count-1]);
    keys[count-1] = '\0';

    count--;
    insertNode(node1);
    
  }
  else
  {
    sibling->insertNode(node1);//new value belongs in sibling
  }
    if (rightSibling)
    {
      rightSibling->setLeftSibling(sibling);//make right sibling's left sibling you
    }
    
    setRightSibling(sibling);//set right sibling
  
  return sibling;

} //split()

