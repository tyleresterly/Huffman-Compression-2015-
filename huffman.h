#ifndef HUFFMAN_H
#define HUFFENMAN_H
struct HuffTreeNode
{
  unsigned long frequency;
  unsigned char symbol;
  struct HuffTreeNode* left;
  struct HuffTreeNode* right;
  char code[256];
};

/* This struct is used as a Linked List Node that will be used
 for the priority queue. It contains a Huffman Tree Node and
 a pointer to the next node in the queue*/
struct ListNode
{
  struct HuffTreeNode* node;
  struct ListNode* next;
};
void printArray(unsigned char arr[]);
struct ListNode* createListNode (struct HuffTreeNode* huffNode);
struct HuffTreeNode* createNode(unsigned char symbol);
unsigned short listLength(struct ListNode* head);
void printLeavesHelper (struct HuffTreeNode* root);
void printLeaves (struct HuffTreeNode* root );
void printList (struct ListNode* head);
void removeNode(struct ListNode** headRef);
struct ListNode* insertSorted (struct ListNode* head, struct ListNode* newNode);
struct ListNode* mergeNode(struct ListNode* first, struct ListNode* second, struct ListNode* head);
struct HuffTreeNode* createHuffTree(struct ListNode* head);
void generateCodes (struct HuffTreeNode* root, unsigned char huff[256], int top);
struct HuffTreeNode* findNode (struct HuffTreeNode* root, unsigned char symbol);
void printFrequencyTable(unsigned short count, struct HuffTreeNode* root, FILE* out, unsigned long total);
void freeTree (struct HuffTreeNode* root);

#endif
