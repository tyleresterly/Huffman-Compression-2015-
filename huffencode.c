#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

/* Struct that is used a node in a Huffman tree. It contains a symbol,
the symbols frequency, the symbol's generated Huffman code, as well as
left and right child nodes.*/



/*Function that reads characters in from a given file, creates a HuffTreeNode
whenever it encounters a new character, and then counts how many times that
symbol repeats itself, ie the frequency of that symbol. It returns the total
amount of symbols read in by getc*/
int generateFrequencies(struct HuffTreeNode *huffArray[256], FILE* in)
{
  unsigned int c;
  int i;
  int newNodeFlag;
  int symbolCount;
  symbolCount = 0;
  while ((c = getc(in)) != EOF)
  {
    newNodeFlag = 1;
    for (i = 0; i < symbolCount; i++)
    {
      if ((huffArray[i])->symbol == c)
      {
        (huffArray[i])->frequency++;
        newNodeFlag = 0;
        break;
      }
    }
    if (newNodeFlag)
    {
    huffArray[symbolCount] = createNode(c);
      (huffArray[symbolCount])->frequency++;
      symbolCount++;
    }
  }
  return symbolCount;
}




/*Function that actually creates the Huffman tree from a linked list of
HuffTreeNodes*/

/*Function that actually encodes the input file using the Huffman tree and generated
codes. It rewinds the input file, and for every character read by getc() it traverses
the Huffman tree until it finds the correct binary code.*/
void encodeFile(FILE* in, FILE* out, struct HuffTreeNode* root)
{
  struct HuffTreeNode* current;
  unsigned long c;
  int j;
  int i;
  char *ptr;
  char bytes[9];
  unsigned char byte;
  bytes[8] = '\0';
  rewind(in);
  j = 7;
  i = 0;
  while ((c = getc(in)) != EOF)
  {
    current = findNode(root,c);
    while (current->code[i] != '\0')
    {
      bytes[j] = current->code[i];
      i++;
      j--;
      if (j < 0)
      {
        byte =  strtol(bytes,&ptr,2);
        putc(byte, out);
        for (j = 0; j < 7; j++)
        {
          bytes[j] = '0';
        }
      }
    }
    i = 0;
  }
  if (c == '\0' && j < 7)
  {
    byte = strtol(bytes,&ptr,2);
    putc(byte,out);
  }
}

int main(int argc, char** argv)
{
  char* infile;
  char* outfile;
  FILE* in;
  FILE* out;
  unsigned char code[256];
  struct HuffTreeNode *huffman[256];
  struct ListNode* head;
  struct HuffTreeNode* root;
  int i;
  int symbolCount;
  unsigned long total;
  struct ListNode* temp;
  unsigned long frequency;
  infile = argv[1];
  outfile = argv[2];
  in = fopen(infile, "rb");
  out = fopen(outfile, "wb");
  total = 0;
  symbolCount = generateFrequencies(huffman, in);
  head = NULL;
  for (i = 0; i < symbolCount; i++)
  {
    frequency = huffman[i]->frequency;
    total += frequency;
    temp = createListNode(huffman[i]);
  	head = insertSorted(head, temp);
  }
  root = createHuffTree(head);
  generateCodes(root,code,0);
  printFrequencyTable(symbolCount,root,out,total);
  encodeFile(in,out,root);
  fclose(in);
  fclose(out);
  return 0;
}
