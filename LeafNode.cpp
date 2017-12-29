#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()

LeafNode* LeafNode::insert(int value)
{
  
  if (leafSize > count) // if there is space left in the node
  {
    insertValue(values, value); //insert the value
    return NULL;
  }
  if (count == leafSize)//node is full
  {
    if ((leftSibling) && !(leftSibling->getCount() == leafSize))//left sibling exists and isnt full
    {
      insertLeft(value);
    }//check left sibling
    else if (((LeafNode*)rightSibling) && !(rightSibling->getCount() == leafSize))//right sibling exists and isnt full
    {
      insertRight(value);
    }//check right sibling
    else
    return split(value);//split
  }//leaf is full
  return NULL;
}  // LeafNode::insert()

void LeafNode::insertLeft(int value)
{
    int i;
    leftSibling->insert(values[0]);//insert min into the left sibling
    count--;//decrement count + replace with null
    values[0] = '\0';
    for (i = 0; i < count; i++) //shifts everything over to the left 1
    {
    values[i] = values[i+1];
    }
    values[count] = '\0';
    insertValue(values, value); // inserts value

} // insert left

void LeafNode::insertRight(int value)
{
    
    if (value > values[count-1])
    {
      rightSibling->insert(value);
      
      return;
    }
    rightSibling->insert(values[count-1]); //inserts max into right sibling and decrements count
    count--;
    values[count] = '\0'; //replaces max with null
    insertValue(values,value);//inserts new value
    
} // insert right

void LeafNode::insertValue(int *values, int value)
{
  int i;
  int temp;
  if (!(*values))//if nothing is in node yet
  {
    *values = value;
    count++;
    return;
  } // for first value
  
  for (i = 0; i < leafSize; i++)//find place in array
  {
    if (!(values[i]))//if spot hasnt been initialized yet
      break;
    if ((value < values[i]))//if value is smaller than current spot
    {
    temp = values[i];//insert value, temp becomes old value
    values[i] = value; 
    value = temp;
    }
  }
  values[i] = value;//last spot will be last value
  count++;
  return;
  
} // LeafNode::reorder

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

LeafNode* LeafNode::split(int value)
{ 
  
  int i; 
  LeafNode *sibling = new LeafNode(leafSize, parent, this ,rightSibling);//make new sibling to the right of current
  for (i = leafSize +1 ; i > (((leafSize+1)/2)+1); i--)//insert at least last value, will have half 
  {
    sibling->insert(values[i-2]);
    values[i-2] = '\0';
    
    count--;//decrement count
  }
  if (value < values[count-1]) //if new value belongs in original node and not new one
  {
    sibling->insert(values[count-1]);//add one more to the new sibling, then insert new
    values[count-1] = '\0';
    count--;
    
    insertValue(values, value);
    
  }
  else
  {
    sibling->insert(value);//new value belongs in sibling
  }
    if (rightSibling)
    {
      rightSibling->setLeftSibling(sibling);//make right sibling's left sibling you
    }
    
    setRightSibling(sibling);//set right sibling
  
  return sibling;
} // split()














