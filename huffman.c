/*Tyler Esterly
   11-29-2015*/

#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

/*global variable that gets turned on if a merged node is being inserted
into a priority queue.*/
int mergedNode = 0;

int const NULLNODE = -1;
void printList (struct ListNode* head)
{
  struct ListNode *current = head;
  while (current != NULL)
  {
    if (current->node->symbol == 0) printf("merged: %lu\n",current->node->frequency);
    else
    {
    printf("%c: %lu\n", current->node->symbol, current->node->frequency);
    }
    current = current->next;
  }
  printf ("\n");
}

/*Helper function that prints an array of chars. used to print a
symbol's code.*/
void printArray(unsigned char arr[])
{
    int i;
    for (i = 0; i < 8; i++)
    printf("%c", arr[i]);
    printf("\n");
}


/* This function creates a ListNode for the priority queue */
struct ListNode* createListNode (struct HuffTreeNode* huffNode)
{
  struct ListNode* newNode = malloc(sizeof(struct ListNode*));
  newNode->node = huffNode;
  newNode->next = NULL;
  return newNode;
}

/*Function that creates a HuffTreeNode*/
struct HuffTreeNode* createNode(unsigned char symbol)
{
  int i;
  struct HuffTreeNode* newNode = malloc(sizeof(struct HuffTreeNode));
  newNode->symbol = symbol;
  newNode->frequency = 0;
  newNode->left = NULL;
  newNode->right = NULL;
  for (i = 0; i < 256; i++)
  {
    newNode->code[i] = '\0';
  }
  return newNode;
}

/* Function that returns the length of a Linked List*/
unsigned short listLength(struct ListNode* head)
{
  struct ListNode* current;
  unsigned long count;
  current = head;
  count = 0;
  while (current != NULL)
  {
    count++;
    current = current->next;
  }
  return count;
}


/*Helper function that prints the elements of a Linked List*/

/*Function that removes and frees nodes after they're binary trees have been
merged in the creation of a huffman tree*/
void removeNode(struct ListNode** headRef)
{
  struct ListNode* node = *headRef;
  *headRef = node->next;
  free(node);
}

/* Inserts HuffMan nodes into the priority queue based on the frequency data
   contained inside of the given  Huffman Tree Node.*/

struct ListNode* insertSorted (struct ListNode* head, struct ListNode* newNode)
{
  struct ListNode* current = head;
  struct HuffTreeNode* leafNode;
  /**/
  if( current == NULL || newNode->node->frequency < current->node->frequency
     || (current->node->frequency == newNode->node->frequency &&
     newNode->node->symbol < current->node->symbol))
  {
    newNode->next = current;
    return newNode;
  }
  else
  {
    /* find the right most child if the inserted node is a merged node*/
    if (mergedNode)
    {
      leafNode = newNode->node;
      while (leafNode->right != NULL)
      {
        leafNode = leafNode->right;
      }
    }
    while (current->next != NULL && current->next->node->frequency < newNode->node->frequency)
    {
      current = current->next;
    }
    /*if the new node's frequency is the same as the current node's frequency, insert it based on it's
    ASCII value*/
    if (current->next != NULL && current->next->node->frequency == newNode->node->frequency)
    {
      /*if we're inserting a merged node, we need to use the rightmost child's symbol in the case
      of a frequency tie.
      Note: The while loop in this if statement and the following else statement do the same thing and
      could've just been merged int one while loop, however I just made two separate loops to make it easier
      to read.*/
      if (mergedNode)
      {
        while (current->next != NULL && current->next->node->symbol < leafNode->symbol
               && current->next->node->frequency == newNode->node->frequency)
        {
          current = current->next;
        }
      }
      /* If it's not a merged node, then the List Node should contain a Tree Node with an actual symbol*/
      else
      {
        while (current->next != NULL && current->next->node->symbol < newNode->node->symbol
               && current->next->node->frequency == newNode->node->frequency)
        {
        current = current->next;
        }
      }
    }
    newNode->next = current->next;
    current->next = newNode;
  return head;
  }
}

/* Function that merges the binary trees contained by two listNodes and stores the merged tree in a
new listNode*/
struct ListNode* mergeNode(struct ListNode* first, struct ListNode* second, struct ListNode* head)
{
  struct ListNode* newNode = malloc(sizeof(struct ListNode*));
  newNode->node = createNode(NULLNODE);
  newNode->node->left = first->node;
  newNode->node->right = second->node;
  newNode->node->frequency = (first->node->frequency + second->node->frequency);
  return newNode;
}

struct HuffTreeNode* createHuffTree(struct ListNode* head)
{
  struct ListNode* temp;
  struct HuffTreeNode* root;
  while (listLength(head) > 1)
  {
    temp = mergeNode(head,head->next, head);
    removeNode(&head);
    removeNode(&head);
    mergedNode = 1;
    head = insertSorted(head,temp);
  }
  mergedNode = 0;
  root = head->node;
  return root;
}
/*Function that traverses a Huffman tree and generates the Huffman codes
for every symbol in the tree*/
void generateCodes (struct HuffTreeNode* root, unsigned char huff[256], int top)
{
  int i;
  if (root->left)
  {
    huff[top] = '0';
    root->code[top] = huff[top];
    generateCodes(root->left, huff, top+1);
  }
  if (root->right)
  {
    huff[top]= '1';
    root->code[top] = huff[top];
    generateCodes(root->right, huff, top+1);
  }
  for (i = 0; i < top; i++)
  {
    root->code[i] = huff[i];
  }
  root->code[top + 1] = '\0';
}

/*Helper function that searches a binary tree and returns whichever node contains
the given symbol.*/
struct HuffTreeNode* findNode (struct HuffTreeNode* root, unsigned char symbol)
{
  if (root != NULL)
  {
    if (root->symbol == symbol)
    {
      return root;
    }
    else
    {
      struct HuffTreeNode *node;
      node = findNode(root->left,symbol);
      if (node)
        return node;
      node = findNode(root->right,symbol);
      if (node)
        return node;
    }
  }

  return NULL;
}

/*This function prints the symbols, their frequencies, and their codes in a table.
it also writes the total number of symbols, the symbols followed by their frequency,
and the total number of encoded symbols to the output file.*/
void printFrequencyTable(unsigned short count, struct HuffTreeNode* root, FILE* out, unsigned long total)
{
  unsigned char value;
  int i;
  struct HuffTreeNode* nodes[256];
  value = 0;
   for(i = 0; i < count; i++)
   {
     while (findNode(root,value) == NULL || findNode(root,value)->symbol == NULLNODE)
     {
      value++;
     }
     nodes[i] = findNode(root,value);
     value++;
   }
  printf("Symbol\tFreq\tCode\n");
  fwrite(&count,sizeof(unsigned short),1,out);
  for (i = 0; i < count; i++)
  {
    if (nodes[i]->symbol < 33 || nodes[i]->symbol > 126)
    {
      printf("=%d\t",nodes[i]->symbol);
    }
    else
    {
      printf("%c\t",nodes[i]->symbol);
    }
    printf("%lu\t%s\n",nodes[i]->frequency,nodes[i]->code);
    fwrite(&nodes[i]->symbol,sizeof(unsigned char),1,out);
    fwrite(&nodes[i]->frequency,sizeof(unsigned long), 1, out);
  }
  printf("Total chars = %lu",total);
  fwrite(&total,sizeof(unsigned long),1,out);
}
