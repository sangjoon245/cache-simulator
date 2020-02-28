#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int logger(unsigned int n){
  return (n > 1) ? 1 + logger(n / 2) : 0;
}

int powerOfTwo(int n){
  if(n == 0){
    return 0;
  } else {
    while(n != 1){
      if(n % 2 != 0){
        return 0;
      }
      n = n / 2;
    }
    return 1;
  }
}

int isAssocNumber(char *a){
  if(strlen(a) < 7){
    return 0;
  }
  if(strncmp(a, "assoc:", 6) == 0){
    int i;
    for(i = 0; a[i + 6] != '\0'; i++){
      if(!(a[i + 6] >= '0' && a[i + 6] <= '9')){
        return 0;
      }
    }
  }
return 1;
}

int isFull(unsigned long long int *x, int max){      //Returns 1 if set if full, 0 otherwise
  int i;
  for(i = 0;i<max;i++){
    if(x[i] == 1111){
      return 0;
    }
  }
  return 1;
}

void storeInside(unsigned long long int t, unsigned long long int *x, int *a, int max){
  int i;
  for(i=0; x[i] != 1111; i++){
  }
  x[i] = t;
}


void doReplace(unsigned long long int *x, int *l, int max, unsigned long long int t){
  int rIndex, i;
    rIndex = 0;
    for(i=0; i < max; i++){
      if(l[i] > l[rIndex])
	    rIndex = i;
    }
  l[rIndex] = 1;
  x[rIndex] = t;
}

void refreshFIFO(unsigned long long int *x, int *l, unsigned long long int t, int max){
  int i;
  for(i = 0; i<max; i++){
    if(x[i]!= 1111){
      l[i]++;
    }
  }
}

int main(int argc, char **argv){

  FILE *f1 = fopen(argv[6], "r");
  if(f1 == NULL){
   return 0;
  }

  if(strcmp(argv[3], "p0") == 0){

    int csize = atoi(argv[1]);
    int blockSize = atoi(argv[5]);
    if((csize == 0) || (powerOfTwo(csize) == 0)){
      printf("cache size error lol");
      return 0;
    }
    if((strcmp(argv[2],"direct") != 0) && (strcmp(argv[2],"assoc") != 0) && (isAssocNumber(argv[2]) == 0)) {
      printf("associativity error lol");
      return 0;
    }
    if(strcmp(argv[4],"fifo") != 0){
      printf("lru error");
      return 0;
    }
    if(blockSize == 0 || (powerOfTwo(atoi(argv[1])) == 0)){
      printf("block size error");
      return 0;
    }

    int cacheSize = 0;
    int nWay = 0;
    int assoc = 0; //0 = direct, 1 = assoc, 2 = assoc:n

    cacheSize = atoi(argv[1]);
    blockSize = atoi(argv[5]);


    if(strcmp(argv[2],"direct") == 0){
      assoc = 0;
    } else
    if(strcmp(argv[2],"assoc") == 0){
      assoc = 1;
    } else {
      assoc = 2;
      sscanf(argv[2],"assoc:%d",&nWay);
    }
  //-------------------------------------------------------------------------------------------

  //ALL VARIABLES NEEDED THROUGHOUT CODE ADD HERE----------------------------------------------
    int numLines = cacheSize/blockSize;
    int memoryRead = 0;
    int memoryWrite = 0;
    int cacheHit = 0;
    int cacheMiss = 0;
    int numOfSets, numOfIndexBits, numOfBlocks, n;
    char operation;
    unsigned long long int address, set, currentTag, mask;
    char firstAdd[30];
    numOfBlocks = (logger(blockSize)/logger(2));

  //-----------------------------------------------------------------------------------------


  //MAKING 2D ARRAY (depending on 0, 1, or 2) --------------------------------------------------------------------------
    if(assoc == 0){ //direct
  //    printf("assoc == 0!\n");
      n = 1;
      numOfSets = numLines;
    }

    if(assoc == 1){ //fully associ
  //    printf("assoc == 1!\n");
      numOfSets = 1;
      n = numLines;
    }

    if(assoc == 2){ //n-way associ
  //    printf("assoc == 2!\n");
      numOfSets = numLines / nWay;
      n = nWay;
    }

    numOfIndexBits = (logger(numOfSets)/logger(2));
  //  printf("numLines is %d\n", numLines);
  //  printf("numOfIndexBits is %d\n", numOfIndexBits);
  //  printf("numOfBlocks is %d\n", numOfBlocks);
  //  printf("numOfSets is %d\n", numOfSets);
  //  printf("n is %d\n", n);
  //  printf("\n\n");
    unsigned long long int **tags = (unsigned long long int **)malloc(numOfSets * sizeof(unsigned long long int *));
    int i, j;
    for(i = 0; i < numOfSets; i++){
      tags[i] = (unsigned long long int *)malloc(n * sizeof(unsigned long long int)); //it's 1 * sizeof since it's direct (one list)
    }

    int **accessed = (int **)malloc(numOfSets * sizeof(int *));
    int **lastAccessed = (int **)malloc(numOfSets * sizeof(int *));
    for(i = 0; i < numOfSets; i++){
      lastAccessed[i] = (int *)malloc(n * sizeof(int));
    }

    for(i = 0; i < numOfSets; i++){
      accessed[i] = (int *)malloc(n * sizeof(int));
    }

    for(i = 0; i < numOfSets; i++){
      for(j = 0; j < n; j++){
        tags[i][j] = 1111;
        lastAccessed[i][j] = 1;
        accessed[i][j] = 1;
      }
    }
    while(1){
  //    printf("Testing...\n");
      fscanf(f1, "%s ", firstAdd);
      if(strcmp(firstAdd, "#eof") == 0){
  //      printf("eof reached\n");
        break;
      }
  //    printf("Address is: %s\n", firstAdd);

      fscanf(f1, "%c ", &operation);
      fscanf(f1, "%llx", &address);
  //    printf("operation is: %c\n", operation);
      currentTag = address >> (numOfIndexBits + numOfBlocks);
      address = (address >> numOfBlocks);
      mask = (1 << numOfIndexBits) - 1;
      set = address & mask;
  //    printf("currentTag is: %llx\n", set);
      if(operation == 'R'){
        int i;
        int found = 0;
        unsigned long long int *a = tags[set];
        unsigned long long int b = currentTag;
        for(i = 0; i < n; i++){
          if(a[i] == b){
  //          printf("Cache HIT!\n");
            refreshFIFO(tags[set], lastAccessed[set], currentTag, n);
            cacheHit++;
            found = 1;
            break;
          }
        }
        if(found == 0){
          cacheMiss++;
          memoryRead++;
          if(isFull(tags[set], n)){
            doReplace(tags[set], lastAccessed[set], n, currentTag);
          } else {
            storeInside(currentTag, tags[set], accessed[set], n);
          }
          refreshFIFO(tags[set], lastAccessed[set], currentTag, n);
        }
      }
      if(operation == 'W'){
        int i;
        int found = 0;
        unsigned long long int *a = tags[set];
        unsigned long long int b = currentTag;
        for(i = 0; i < n; i++){
          if(a[i] == b){
            refreshFIFO(tags[set], lastAccessed[set], currentTag, n); //cache hit!
            cacheHit++;
            memoryWrite++;
            found = 1;
            break;
          }
        }
        if(found == 0){

          cacheMiss++;
          memoryRead++;
          memoryWrite++;
          if(isFull(tags[set], n)){  //cache miss!
            doReplace(tags[set], lastAccessed[set], n, currentTag);
          }
          else {
            storeInside(currentTag, tags[set], accessed[set], n);
          }
          refreshFIFO(tags[set], lastAccessed[set], currentTag, n);

        }
      }
    }

    printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", memoryRead, memoryWrite, cacheHit, cacheMiss);
  }

  else { //IF It is p1






  //READING INPUT ARGV'S ----------------------------------------------------------------------

    FILE *f2 = fopen(argv[6], "r");
    if(f2 == NULL){
     return 0;
    }

    int csize2 = atoi(argv[1]);
    int blockSize2 = atoi(argv[5]);
    if((csize2 == 0) || (powerOfTwo(csize2) == 0)){
      printf("cache size error lol");
      return 0;
    }
    if((strcmp(argv[2],"direct") != 0) && (strcmp(argv[2],"assoc") != 0) && (isAssocNumber(argv[2]) == 0)) {
      printf("associativity error lol");
      return 0;
    }
    if(strcmp(argv[4],"fifo") != 0){
      printf("lru error");
      return 0;
    }
    if(blockSize2 == 0 || (powerOfTwo(atoi(argv[1])) == 0)){
      printf("block size error");
      return 0;
    }

    int cacheSize2 = 0;
    int nWay2 = 0;
    int assoc2 = 0; //0 = direct, 1 = assoc, 2 = assoc:n

    cacheSize2 = atoi(argv[1]);
    blockSize2 = atoi(argv[5]);


    if(strcmp(argv[2],"direct") == 0){
      assoc2 = 0;
    } else
    if(strcmp(argv[2],"assoc") == 0){
      assoc2 = 1;
    } else {
      assoc2 = 2;
      sscanf(argv[2],"assoc:%d",&nWay2);
    }
  //-------------------------------------------------------------------------------------------



  //ALL VARIABLES NEEDED THROUGHOUT CODE ADD HERE----------------------------------------------
    int numLines2 = cacheSize2/blockSize2;
    int memoryRead2 = 0;
    int memoryWrite2 = 0;
    int cacheHit2 = 0;
    int cacheMiss2 = 0;
    int numOfSets2, numOfIndexBits2, numOfBlocks2, n2;
    char operation2;
    unsigned long long int address2, set2, currentTag2, mask2, previousAddress2;
    char firstAdd2[30];
    numOfBlocks2 = (logger(blockSize2)/logger(2));

  //-----------------------------------------------------------------------------------------


  //MAKING 2D ARRAY (depending on 0, 1, or 2) --------------------------------------------------------------------------
    if(assoc2 == 0){ //direct
  //    printf("assoc == 0!\n");
      n2 = 1;
      numOfSets2 = numLines2;
    }

    if(assoc2 == 1){ //fully associ
  //    printf("assoc == 1!\n");
      numOfSets2 = 1;
      n2 = numLines2;
    }

    if(assoc2 == 2){ //n-way associ
  //    printf("assoc == 2!\n");
      numOfSets2 = numLines2 / nWay2;
      n2 = nWay2;
    }

    numOfIndexBits2 = (logger(numOfSets2)/logger(2));

    unsigned long long int **tags2 = (unsigned long long int **)malloc(numOfSets2 * sizeof(unsigned long long int *));
    int i;
    for(i = 0; i < numOfSets2; i++){
      tags2[i] = (unsigned long long int *)malloc(n2 * sizeof(unsigned long long int)); //it's 1 * sizeof since it's direct (one list)
    }

    int **lastAccessed2 = (int **)malloc(numOfSets2 * sizeof(int *));
    int **accessed2 = (int **)malloc(numOfSets2 * sizeof(int *));
    for(i = 0; i < numOfSets2; i++){
      lastAccessed2[i] = (int *)malloc(n2 * sizeof(int));
      accessed2[i] = (int *)malloc(n2 * sizeof(int));
    }
    int j;
    for(i = 0; i < numOfSets2; i++){
      for(j = 0; j < n2; j++){
        tags2[i][j] = 1111;
        lastAccessed2[i][j] = 1;
        accessed2[i][j] = 1;
      }
    }
    while(1){
  //    printf("Testing...\n");
      fscanf(f2, "%s ", firstAdd2);
      if(strcmp(firstAdd2, "#eof") == 0){
  //      printf("eof reached\n");
        break;
      }
  //    printf("Address is: %s\n", firstAdd2);

      fscanf(f2, "%c ", &operation2);
      fscanf(f2, "%llx", &address2);
  //    printf("operation is: %c\n", operation2);
      currentTag2 = address2 >> (numOfIndexBits2 + numOfBlocks2);
      previousAddress2 = address2;
      address2 = (address2 >> numOfBlocks2);
      mask2 = (1 << numOfIndexBits2) - 1;
      set2 = address2 & mask2;
  //    printf("currentTag is: %llx\n", set2);

      if(operation2 == 'R'){
  //      printf("OPERATION IS A R PRETECH\n");
        int i;
        int found2 = 0;
        unsigned long long int *a2 = tags2[set2];
        unsigned long long int b2 = currentTag2;
        for(i = 0; i < n2; i++){
          if(a2[i] == b2){
    //        printf("Cache HIT!\n");
            refreshFIFO(tags2[set2], lastAccessed2[set2], currentTag2, n2);
            cacheHit2++;
            found2 = 1;
            break;
          }
        }
        if(found2 == 0){
          cacheMiss2++;
          memoryRead2++;
          if(isFull(tags2[set2], n2)){
            doReplace(tags2[set2], lastAccessed2[set2], n2, currentTag2);
          } else {
            storeInside(currentTag2, tags2[set2], accessed2[set2], n2);
          }
          refreshFIFO(tags2[set2], lastAccessed2[set2], currentTag2, n2);



          previousAddress2 += blockSize2;
          previousAddress2 = (previousAddress2 >> numOfBlocks2);
          mask2 = (1 << numOfIndexBits2) - 1;
          set2 = previousAddress2 & mask2;
          currentTag2 = previousAddress2 >> numOfIndexBits2;


          //PREFETCHING STATRS HERE

          int i;
          int found2 = 0;
          unsigned long long int *a2 = tags2[set2];
          unsigned long long int b2 = currentTag2;
          for(i = 0; i < n2; i++){
            if(a2[i] == b2){
              found2 = 1;
              break;
            }
          }
          if(found2 == 0){
            memoryRead2++;
  //          printf("memoryread2++\n");
            if(isFull(tags2[set2], n2)){
              doReplace(tags2[set2], lastAccessed2[set2], n2, currentTag2);
            }
            else {
              storeInside(currentTag2, tags2[set2], accessed2[set2], n2);

            }
            refreshFIFO(tags2[set2], lastAccessed2[set2], currentTag2, n2);
          }

          //PRETECH END HERE


        }
      }

      if(operation2 == 'W'){
  //      printf("OPERATION IS W PREFETCH\n");
        int i;
        int found2 = 0;
        unsigned long long int *a2 = tags2[set2];
        unsigned long long int b2 = currentTag2;
        for(i = 0; i < n2; i++){
          if(a2[i] == b2){
            refreshFIFO(tags2[set2], lastAccessed2[set2], currentTag2, n2); //cache hit!
            cacheHit2++;
            memoryWrite2++;
  //          printf("cachehit2++!\nmemorywrite2++!\n");
            found2 = 1;
            break;
          }
        }
        if(found2 == 0){
          cacheMiss2++;
          memoryRead2++;
          memoryWrite2++;
  //        printf("cachemiss2++!\nmemoryread2++\nmemorywrite2++\n");
          if(isFull(tags2[set2], n2)){  //cache miss!
            doReplace(tags2[set2], lastAccessed2[set2], n2, currentTag2);
          }
          else {
            storeInside(currentTag2, tags2[set2], accessed2[set2], n2);
          }
          refreshFIFO(tags2[set2], lastAccessed2[set2], currentTag2, n2);

          //PREFETCHING HERE

          previousAddress2 += blockSize2;
          previousAddress2 = (previousAddress2 >> numOfBlocks2);
          mask2 = (1 << numOfIndexBits2) - 1;
          set2 = previousAddress2 & mask2;
          currentTag2 = previousAddress2 >> numOfIndexBits2;

          int i;
          int found2 = 0;
          unsigned long long int *a2 = tags2[set2];
          unsigned long long int b2 = currentTag2;
          for(i = 0; i < n2; i++){
            if(a2[i] == b2){
              found2 = 1;
              break;
            }
          }
          if(found2 == 0){
            memoryRead2++;
            if(isFull(tags2[set2], n2)){
              doReplace(tags2[set2], lastAccessed2[set2], n2, currentTag2);
            } else {
              storeInside(currentTag2, tags2[set2], accessed2[set2], n2);
            }
          }
          refreshFIFO(tags2[set2], lastAccessed2[set2], currentTag2, n2);

          // PREFETCHING END

        }
      }
    }
    printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", memoryRead2, memoryWrite2, cacheHit2, cacheMiss2);
  }

return 0;


}
