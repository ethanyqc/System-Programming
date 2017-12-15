//THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 //A TUTOR OR CODE WRITTEN BY OTHER STUDENTS -YCHE498

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct _seg {/* definition of new type "seg" */
	int  bits[256];
	struct _seg  *next, *prev;
}seg;

//setbit and check bit Macro
#define   set( x, i )     ( x |= (1 << i) )
#define   check( x, i )     (( x & (1 << i)) != 0 )

//total bits per segment
#define BITSPERSEG  (8*256*sizeof(int))

seg *pt1,*pt2, *head;

//whichSeg methof
seg* whichSeg(int j){
    seg* s = head;
    int k;
		k=8192;//count
    while(k<j){
      s=s->next;//continue to the next node
      k+=8192;//increment of size of segment
    }
    return s;

}

int whichInt(int j){
    int indexInt;
    indexInt = ((j-1)%8192)/32;//return the index of int, over the size of 32
		return indexInt;
}

int whichBit(int j){
    int indexBit;
    indexBit=(j-1)%32;//return the bit, remainder of divison of 32, return index
    return indexBit;
}

//method to test if a bit is 1 or 0, inout a int
int testPrime(int j){
		seg* temp = whichSeg(j);
		if(check( temp->bits[whichInt(j)], whichBit(j))==1){
			return 1;
		}
		else{
			return 0;
		}
}

//mark the given int as non prime by set to 1
void markNonPrime(int j){
		seg* temp = whichSeg(j);
		set(temp->bits[whichInt(j)], whichBit(j));
}

//seive method mark all the nonprime to 1, returns the total number of primes in bitmap
int seive(int j){

		for (int i = 2; i<=(int)sqrt(j); i++) {

			if(testPrime(i)==0){
					for (int k=i*i;k<=j;k=k+i) {
								markNonPrime(k);
					}

			}
		}
		//Count primes
		int count=0;
		for(int x=2;x<=j;x++){
			if(testPrime(x)==0){
					count++;
			}
		}

		return count-1;
}

//Tester print for debug purpose
/*
void print(int m){
	int count=0;
	//int primes[markNonPrime(m)];
	int p;
	for(p=2;p<=m;p++){
		if(testPrime(p)==0){
				printf("%d ", p);
		}
	}
			//testPrime(4);
}*/

//short method, input the number's 3 coordinates, and return 1 or 0
int bit(seg* s, int i, int j){//i=int, j=bit
	if(check(s->bits[i], j)==1){
		return 1;
	}
	else{
		return 0;
	}
}

//goldbach method returns the number of solutions. ALso find the optimized solution
int goldbach(int k){
	int solCount=0;
	int minDiff=k;
	int sol1;
	int sol2;
	for(int i=3;i<k/2;i+=2){

		seg* startSeg = head;
		int startInt = 0;
		int startBit = 2;//since start from 3
		seg* endSeg = whichSeg(k-i);
		int endInt = whichInt(k-i);
		int endBit = whichBit(k-i);

		if(bit(startSeg, startInt, startBit)==0 && bit(endSeg, endInt, endBit)==0){ //both startbit and endbit are 0
			solCount++;
			if(abs(k-i-i)<minDiff){//check min difference
				minDiff=abs(k-i-i);
				sol1=i;
				sol2=k-i;
			}
			//printf("%d and %d\n", i,k-i);//print all solutions
		}
		//seg=8192
		//int=256
		//bit=32
		startBit++;
		if(startBit>=32){//if bit reach 32
				if(startInt<256){//if int is still less than 256
					startBit=0;//refresh startbit
					startInt++;//increment startint
				}
				else if(startInt>=256){//if exceed 256(>32>256=change seg)
					startInt=0;//clear startint
					startBit=0;//also clear bit
					startSeg=startSeg->next;//proceed to next segement
				}
		}
		endBit--;
		if(endBit<0){//if bit is less than 0
				if(endInt>=0){//and if endInt is still more than 0
					endBit=31;//than set bit to end of the prev int
					endInt--;//decrement int
				}
				else if(endInt<0){//if endint is less than 0(meaning reach the begining of bits array)
					endInt=255;//change endint refreash to end index 255
					endBit=31;//reset endbit index to 31
					endSeg=endSeg->prev;//go to prev seg
				}
		}


	}
		printf("Input %d = %d + %d\n", k,sol1,sol2);
		return solCount;

}


int main(int argc, char *argv[]) {
				int i;
        int N;
				int total;


        if (argc == 2) sscanf(argv[1],"%d",&N);
                else scanf("%d",&N);

      		total = (N +BITSPERSEG -1)/BITSPERSEG;


        head = (seg *) malloc(sizeof(seg));
        pt1=head;
        for (i=1;i<N;i++) {
                pt1->next = (seg *) malloc(sizeof (seg));//allocate new space
                pt1 = pt1->next;
                pt1->prev = pt2;
								pt2 = pt1;
							}
							printf("The number of odd primes less than or equal to %d is: %d\n", N, seive(N));
							printf("Enter even number above 5 for GoldBach result\n");
							int input;
							//print(500);
							while(feof(stdin)==0){
									scanf("%d", &input);
									if(input%2==1||input>N||input<=5){
										continue;
									}
										printf("Total number of solution is %d ",goldbach(input));
									}


							return 0;
}
