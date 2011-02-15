#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#define DEBUG 0

#if DEBUG
#define SAMPLE_SIZE 1000000
char ms[] = "mississippi";
#endif

unsigned long start;
unsigned long end;
char* bestStart;
int bestSize = 0;

int compare(char* myLeft, char* myRight) {
	int ret = 0;
	int sizeOfPrefix;

	// strcmp basically
	char* src = myLeft;
	char* dst = myRight;

	while(!(ret = *src - *dst) && *dst)
		++src, ++dst;

	sizeOfPrefix = src - myLeft;

    if(sizeOfPrefix > bestSize) {
		bestSize = sizeOfPrefix;
		bestStart = myLeft;
	}

	return ret;
}

void mySort(char* arr[], int beg, int end) {
	char* t;

	if (end > beg + 1) {
		char* piv = arr[beg];
		int l = beg + 1, r = end;
		while (l < r) {
			if (compare(arr[l], piv) < 0)
				l++;
			else {
				// swap(&arr[l], &arr[--r]);
				r--;
				t = arr[l];
				arr[l] = arr[r];
				arr[r] = t;
			}

		}
		// swap(&arr[--l], &arr[beg]);
		t = arr[beg];
		arr[beg] = arr[--l];
		arr[l] = t;
		mySort(arr, beg, l);
		mySort(arr, r, end);
	}
}

char* getInput() {
	char* inputStr = NULL;
	int size = 4000;
	int len = 0;

	do {
		inputStr = (char *) realloc(inputStr, sizeof(char) * size);

		len += fread(inputStr + len, sizeof(char), size - len - 1, stdin);
		inputStr[len] = '\0';

		len = size - 1;
		size <<= 1;
	} while (!feof(stdin));

	return inputStr;
}

#if DEBUG
	#define PRINT_STATUS(a) fprintf(stderr, a)
	#define PRINT_TIME printTime()
	#define START_TIMER start = currentTimeMillis()

	char* getRandomString(int size) {
		char* randomStr = (char *) malloc(sizeof(char) * size);
		randomStr[--size] = '\0';
		while(size-- > 0)
			randomStr[size] = rand() % 255 + 1;

		return randomStr;
	}


	unsigned long currentTimeMillis() {
		struct timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_usec + tv.tv_sec * 1000000;
	}

	void printTime() {
		end = currentTimeMillis();
		fprintf(stderr, "took %ld microseconds (it is now %ld)\n", end - start, end);
		start = end;
	}
#else
	#define PRINT_STATUS(a) 0
	#define START_TIMER 0
	#define PRINT_TIME 0
#endif


int main(int argc, char* argv[]) {
	int i;
	int sizeOfPrefix;
	char* myString;

	START_TIMER;

	PRINT_STATUS("Grabbing input... ");
	#if DEBUG == 2
		myString = ms;//getInput();//getRandomString(2000);
	#else
		myString = getInput();
	#endif
	bestStart = myString;
	PRINT_TIME;

	PRINT_STATUS("Producing the array of pointers... ");
	int numElements = strlen(myString);
	char** myArr = (char**) malloc(sizeof(char*) * numElements);
	for(i = 0; i < numElements; i++) {
		myArr[i] = myString + i;
	}
	PRINT_TIME;

	PRINT_STATUS("Sorting the suffixes... ");
	mySort(myArr, 0, numElements);
	PRINT_TIME;

	#if DEBUG == 2
		PRINT_STATUS("Finding the longest shared prefix... ");
		for(i = 0; i < numElements; i++) {
			printf("%s\n", myArr[i]);
		}
	#endif
	PRINT_TIME;

	*(bestStart + bestSize) = '\0';
	fprintf(stderr, "Found string of %d length: ", bestSize);
	fprintf(stdout, "%s", bestStart);
	return 0;
}
