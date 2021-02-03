#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
// #include "algorithms.c" // LZW compression

#define MAX_TREE_HT 50

FILE *inputFile;
FILE *outputFile;

//*********************************
//*********************************
//**************HUFFMAN ENCODING***************


struct MinHNode {
  char item;
  unsigned freq;
  struct MinHNode *left, *right;
};

struct MinHeap {
  unsigned size;
  unsigned capacity;
  struct MinHNode **array;
};

// Create nodes
struct MinHNode *newNode(char item, unsigned freq) {
  struct MinHNode *temp = (struct MinHNode *)malloc(sizeof(struct MinHNode));

  temp->left = temp->right = NULL;
  temp->item = item;
  temp->freq = freq;

  return temp;
}

// Create min heap
struct MinHeap *createMinH(unsigned capacity) {
  struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

  minHeap->size = 0;

  minHeap->capacity = capacity;

  minHeap->array = (struct MinHNode **)malloc(minHeap->capacity * sizeof(struct MinHNode *));
  return minHeap;
}

// Function to swap
void swapMinHNode(struct MinHNode **a, struct MinHNode **b) {
  struct MinHNode *t = *a;
  *a = *b;
  *b = t;
}

// Heapify
void minHeapify(struct MinHeap *minHeap, int idx) {
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
    smallest = left;

  if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
    smallest = right;

  if (smallest != idx) {
    swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

// Check if size if 1
int checkSizeOne(struct MinHeap *minHeap) {
  return (minHeap->size == 1);
}

// Extract min
struct MinHNode *extractMin(struct MinHeap *minHeap) {
  struct MinHNode *temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return temp;
}

// Insertion function
void insertMinHeap(struct MinHeap *minHeap, struct MinHNode *minHeapNode) {
  ++minHeap->size;
  int i = minHeap->size - 1;

  while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
  {
    minHeap->array[i] = minHeap->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }
  minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {
  int n = minHeap->size - 1;
  int i;

  for (i = (n - 1) / 2; i >= 0; --i)
    minHeapify(minHeap, i);
}

int isLeaf(struct MinHNode *root) {
  return !(root->left) && !(root->right);
}

struct MinHeap *createAndBuildMinHeap(char item[], int freq[], int size) {
  struct MinHeap *minHeap = createMinH(size);
int i;
  for (i = 0; i < size; ++i)
    minHeap->array[i] = newNode(item[i], freq[i]);

  minHeap->size = size;
  buildMinHeap(minHeap);

  return minHeap;
}

struct MinHNode *buildHuffmanTree(char item[], int freq[], int size) {
  struct MinHNode *left, *right, *top;
  struct MinHeap *minHeap = createAndBuildMinHeap(item, freq, size);

  while (!checkSizeOne(minHeap)) {
    left = extractMin(minHeap);
    right = extractMin(minHeap);

    top = newNode('$', left->freq + right->freq);

    top->left = left;
    top->right = right;

    insertMinHeap(minHeap, top);
  }
  return extractMin(minHeap);
}

// Print the array
void printArray(int arr[], int n) {
  int i;
  for (i = 0; i < n; ++i)
    printf("%d", arr[i]);

  printf("\n");
}

void printHCodes(struct MinHNode *root, int arr[], int top) {
  if (root->left) {
    arr[top] = 0;
    printHCodes(root->left, arr, top + 1);
  }
  if (root->right) {
    arr[top] = 1;
    printHCodes(root->right, arr, top + 1);
  }
  if (isLeaf(root)) {
    printf("  %c   | ", root->item);
    printArray(arr, top);
  }
}

// Wrapper function
void HuffmanCodes(char item[], int freq[], int size) {
  struct MinHNode *root = buildHuffmanTree(item, freq, size);

  int arr[MAX_TREE_HT], top = 0;

  printHCodes(root, arr, top);
}



void huffmanEncoding(char text[]) {
  // char text[500];
	// printf("--------------------------------------Enter the text you want to compress-----------------------------------------\n\n");
	// scanf("%[^\t]%*c", text); // take a full string input and paragraph


    int freq[256] = {0};
    int i;

    //Calculating frequency of each character.
    for( i = 0; text[i] != '\0'; i++)
    {
        freq[text[i]]++;
    }

		char arr[100] = {0};
		int freq1[100];
		int x = 0;

    //Printing frequency of each character.
    for(i = 0; i < 256; i++)
    {
        if(freq[i] != 0)
        {

						arr[x] = i;
						freq1[x] = freq[i];
						x++;
        }
    }

    int n = x;

arr[x+1] = '\0';

  int size = sizeof(arr) / sizeof(arr[0]);
  printf(" Char | Huffman code ");
  printf("\n--------------------\n");

  clock_t t;
  t = clock();

  HuffmanCodes(arr, freq1, n);

  t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("\n\n\nHuffman took %f seconds to execute \n", time_taken);

}





//*********************************
//*********************************
//**************HUFFMAN ENCODING***************





//*********************************
//*********************************
//**************RUNLENGTH******************

void runLength(char text[])
{
  // Calculate the time taken by fun()
     clock_t t;
     t = clock();


  int inputCodeLength = strlen(text);



  char current = text[0];

  int count = 1;

  int x = 0;
  int i;



  for(i = 1; text[i]!='\0'; i++)
  {
    if(text[i] == current)
    count++;
    else
    {
      printf( "%c%d", current, count );
      x = x + 2;
      current = text[i];
      count = 1;
    }
  }

    printf( "%c%d", current, count );

    x = x + 2;

     t = clock() - t;
       double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

       printf("\n\n\nRunLength took %f seconds to execute \n", time_taken);


    printf("\n");

    if(x >= inputCodeLength)
    {
      printf("\n\n*String not compressed efficiently* \n\n encode in Huffman......\n\n");

      huffmanEncoding(text);


    }

    else{
          printf("  done!  \n\n\n");
    }


  }







//*********************************
//*********************************
//**************RUNLENGTH******************










//*********************************
//*********************************
//**************LEMPEL ZIV WELCH***************


//will write the file
void writeBinary(FILE * output, int code);
// int readBinary(FILE * input);

int leftover = 0;
int leftoverBits;

void writeBinary(FILE * output, int code) {
    if (leftover > 0) {
        int previousCode = (leftoverBits << 4) + (code >> 8);

        fputc(previousCode, output);
        fputc(code, output);

        leftover = 0; // no leftover now
    } else {
        leftoverBits = code & 0xF; // save leftover, the last 00001111
        leftover = 1;

        fputc(code >> 4, output);
    }
}



//singly linked list based dictionary
enum {
    emptyPrefix = -1 // empty prefix for ASCII characters
};

// the "string" in the dictionary consists of the last byte of the string and an index to a prefix for that string
struct DictNode {
    int value; // the position in the list
    int prefix; // prefix for byte > 255
    int character; // the last byte of the string
    struct DictNode *next;
};

void dictionaryInit();
void appendNode(struct DictNode *node);
void dictionaryDestroy();
int dictionaryLookup(int prefix, int character);
int dictionaryPrefix(int value);
int dictionaryCharacter(int value);
void dictionaryAdd(int prefix, int character, int value);

// the dictionary
struct DictNode *dictionary, *tail;

// initialize the dictionary of ASCII characters @12bits
void dictionaryInit() {
    int i;
    struct DictNode *node;
    for (i = 0; i < 256; i++) { // ASCII
        node = (struct DictNode *)malloc(sizeof(struct DictNode));
        node->prefix = emptyPrefix;
        node->character = i;
        appendNode(node);
    }
}

// add node to the list
void appendNode(struct DictNode *node) {
    if (dictionary != NULL) tail->next = node;
    else dictionary = node;
    tail = node;
    node->next = NULL;
}

// destroy the whole dictionary down to NULL
void dictionaryDestroy() {
    while (dictionary != NULL) {
        dictionary = dictionary->next; /* the head now links to the next element */
    }
}

// is prefix + character in the dictionary?
int dictionaryLookup(int prefix, int character) {
    struct DictNode *node;
    for (node = dictionary; node != NULL; node = node->next) { // ...traverse forward
        if (node->prefix == prefix && node->character == character) return node->value;
    }
    return emptyPrefix;
}

int dictionaryPrefix(int value) {
    struct DictNode *node;
    for (node = dictionary; node != NULL; node = node->next) { // ...traverse forward
        if (node->value == value) return node->prefix;
    }
    return -1;
}

int dictionaryCharacter(int value) {
    struct DictNode *node;
    for (node = dictionary; node != NULL; node = node->next) { // ...traverse forward
        if (node->value == value) {
            //printf("\nNODE %i %i %i\n", node->value, node->prefix, node->character);
            return node->character;
        }
    }
    return -1;
}

// add prefix + character to the dictionary
void dictionaryAdd(int prefix, int character, int value) {
    struct DictNode *node;
    node = (struct DictNode *)malloc(sizeof(struct DictNode));
    node->value = value;
    node->prefix = prefix;
    node->character = character;
    //printf("\n(%i) = (%i) + (%i)\n", node->value, node->prefix, node->character);
    appendNode(node);
}






//the algorithm

enum {
    dictionarySize = 4095, // maximum number of entries defined for the dictionary (2^12 = 4096)
    codeLength = 12, // the codes which are taking place of the substrings
    maxValue = dictionarySize - 1
};

// function declarations
void compress(FILE *inputFile, FILE *outputFile);

// compression
void compress(FILE *inputFile, FILE *outputFile) {
    int prefix = getc(inputFile);
    if (prefix == EOF) {
        return;
    }
    int character;

    int nextCode;
    int index;

    // LZW starts out with a dictionary of 256 characters (in the case of 8 codeLength) and uses those as the "standard"
    //  character set.
    nextCode = 256; // next code is the next available string code
    dictionaryInit();

    // while (there is still data to be read)
    while ((character = getc(inputFile)) != (unsigned)EOF) { // ch = read a character;

        // if (dictionary contains prefix+character)
        if ((index = dictionaryLookup(prefix, character)) != -1) prefix = index; // prefix = prefix+character
        else { // ...no, try to add it
            // encode s to output file
            writeBinary(outputFile, prefix);

            // add prefix+character to dictionary
            if (nextCode < dictionarySize) dictionaryAdd(prefix, character, nextCode++);

            // prefix = character
            prefix = character; //... output the last string after adding the new one
        }
    }
    // encode s to output file
    writeBinary(outputFile, prefix); // output the last code

    if (leftover > 0) fputc(leftoverBits << 4, outputFile);

    // free the dictionary here
    dictionaryDestroy();
}




//*********************************
//*********************************
//**************LEMPEL ZIV WELCH***************





int main(int argc, char** argv) { // number of arguments, arguments array
    // do we have correct # of arguments?
    if (argc > 2) {
        if (strcmp(argv[1], "c") == 0) { // compression
            inputFile = fopen(argv[2], "r"); // read from the input file (HTML)
            outputFile = fopen(strcat(argv[2], ".lzw"), "w+b"); // binary write to output file

            if (outputFile == NULL || inputFile == NULL) {
                printf("There is no file\n'"); return 0;
            }

            clock_t t; //to time everything;
            t = clock();

            compress(inputFile, outputFile); //main file

            t = clock() - t;
              double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

              printf("\n\n\nLempel Ziv took %f seconds to execute \n", time_taken);

          }

        fclose(inputFile); fclose(outputFile); // close handles
    }


     else {  //options

        // printf("\n-------- Compress your file -------");
        // printf("\n\n the command is - lzw c file_name");
        int i;
        printf("***********\n");
        for(i = 0; i < 3;i++)
        {
          printf("*                             *\n");
        }
          printf("*           WELCOME!          *\n");

          for(i = 0; i < 3;i++)
          {
            printf("*                             *\n");
          }
            printf("***********\n");


    printf("\n Hello User! \n");

    printf("\n Please select what you want to perform. \n");

    printf("select : \n\n 1 - To compress text \n 2 - to compress file \n\n");

    int n;

    scanf("%d", &n); // the choice

    printf("\n\n You have selected %d \n\n", n);



if(n == 1)
{

  printf("\nYou have chosen TEXT COMPRESSION\n");


  char text[500];
  printf("\n*Enter the text you want to compress*\n\n");
  scanf("%[^\t]%*c", text); // take a full string input and paragraph

  int inputCodeLength = strlen(text);

  printf("\nEncoding in RLE.....\n\n");

  runLength(text);


  //starter for text compression

}

if(n == 2)
{
  //starter for file compression

  printf("\n you have chosen FILE COMPRESSION\n");

  printf("please enter in the following format to continue file compression: \n\n minorCompress <c> <filename> \n\n");


}



    }

    return 0;
}
