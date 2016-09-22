#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

/*Helper function that converts an integer to binary*/
void intToBinary(unsigned int n, char bits[9])
{
  int i;
  int powerOfTwo;
  powerOfTwo = 128;
  for (i = 0; i < 8; i++)
  {
    if (n & powerOfTwo)
    {
      bits[i] = '1';
    }
    else
    {
      bits[i] = '0';
    }
    powerOfTwo = powerOfTwo >> 1;
  }
}
/*Function that prints the frequency table after the Huffman Tree has been generated.*/
void printFrequencyTableDecoded(unsigned short count, struct HuffTreeNode* root, FILE* out, unsigned long total)
{
  unsigned char value;
  int i;
  struct HuffTreeNode* nodes[256];
  value = 0;
   for(i = 0; i < count; i++)
   {
     while (findNode(root,value) == NULL || findNode(root,value)->symbol == 3)
     {
      value++;
     }
     nodes[i] = findNode(root,value);
     value++;
   }
  printf("Symbol\tFreq\tCode\n");
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

  }
  printf("Total chars: %lu", total);
}

/*Function that reads the frequency table used for generating the Huffman tree*/
unsigned short readFrequencyTable(FILE* in,struct HuffTreeNode* huffArray[256])
{
  int i;
  unsigned short total;
  fread(&total,sizeof(unsigned short),1,in);
  for (i = 0; i < total; i++)
  {
    huffArray[i] = createNode(0);
  }
  for (i = 0; i < total; i++)
  {
    fread(&huffArray[i]->symbol,sizeof(unsigned char),1,in);
    fread(&huffArray[i]->frequency,sizeof(unsigned long),1,in);
  }
  return total;
}

/*Helper function that puts all the available tree nodes in an array*/
void nodesToArray(struct HuffTreeNode* huffArray[256], struct HuffTreeNode* root, unsigned short count)
{
  unsigned char value;
  int i;
  value = 0;

  for(i = 0; i < count; i++)
  {
    while (findNode(root,value) == NULL || findNode(root,value)->symbol == 3)
    {
      value++;
    }
    huffArray[i] = findNode(root,value);
    value++;
   }
}

/*Function that actually handles the decoding of the file*/
void decodeFile(FILE* in, FILE* out, struct HuffTreeNode* root, unsigned short count)
{
  unsigned int c;
  int j;
  int i;
  int k;
  struct HuffTreeNode* nodes[256];
  char codes[256];
  char bits[9];
  int newSymbol;
  bits[8] = '\0';
  j = 0;
  nodesToArray(nodes,root,count);
  newSymbol = 0;
  while ((c = getc(in)) != EOF)
  {
     intToBinary(c,bits);
     for (i = 7; i >= 0; i--)
     {
      if (newSymbol)
      {
        for (k = 0; k < j; k++)
        {
          codes[k] = '\0';
        }
        newSymbol = 0;
        j = 0;
      }
      codes[j] = bits[i];
      codes[j+1] = '\0';
      j++;
       for (k = 0; k < count; k++)
       {
         if (!strcmp(nodes[k]->code,codes))
         {
           putc(nodes[k]->symbol,out);
           newSymbol = 1;
           break;
         }
       }
     }
  }
}


int main(int argc, char** argv)
{
  int i;
  char* infile;
  char* outfile;
  struct ListNode* head = NULL;
  struct HuffTreeNode* root = NULL;
  unsigned short symbolCount;
  struct HuffTreeNode* huffArray[256];
  unsigned char code[256];
  unsigned long total;
  FILE* in;
  FILE* out;
  infile = argv[1];
  outfile = argv[2];
  in = fopen(infile, "rb");
  out = fopen(outfile, "wb");
  symbolCount = readFrequencyTable(in,huffArray);
  fread(&total,sizeof(unsigned long),1,in);
  for (i = 0; i < symbolCount; i++)
  {
    struct ListNode* temp = createListNode(huffArray[i]);
  	head = insertSorted(head, temp);
  }
  root = createHuffTree(head);
  generateCodes(root,code,0);
  printFrequencyTableDecoded(symbolCount,root,out,total);
  decodeFile(in,out,root,symbolCount);
  fclose(in);
  fclose(out);
  return 0;
}

