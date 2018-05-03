/**
   Skeleton of the Autocomplete assignment
   that allows for loading and writing
   a datafile
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// define a max query length
#define MAX_QUERY_LENGTH 255


/**
   DO NOT MODIFY
   Function to load data file consisting
   of words and their frequencies

   @param queries array of strings of max length 255
   @param weights array of unsigned longs
   @param fp pointer to a data file
 */
void load(char queries[][MAX_QUERY_LENGTH], unsigned long weights[],
	  FILE *fp) {

  char query[MAX_QUERY_LENGTH];  // buffer for queries
  unsigned long weight;  // used to read in weights
  int i = 0;  // index

  // keep scanning until we fail to scan a query
  // weight
  while (fscanf(fp, "%lu", &weight) == 1) {

    // scan the space
    fgetc(fp);
    // scan the query
    fscanf(fp, "%[^\n]\n", query);

    // put in the appropriate query, weight pair
    strcpy(queries[i], query);
    weights[i] = weight;
    i++;
  }
}


/**
   DO NOT MODIFY
   Function to write queries and weights into a data file

   @param queries array of strings of max length 255
   @param weights array of unsigned longs
   @param fp pointer to a data file
   @param length number of elements in each array
 */
void write(char queries[][MAX_QUERY_LENGTH], unsigned long weights[],
	   FILE *fp, int length) {

  for (int i = 0; i < length; i++) {
    fprintf(fp, "%s\t%lu\n", queries[i], weights[i]);
  }
}


void insertionsort(char queries[][MAX_QUERY_LENGTH], unsigned long weights[], int length) {
	for (int i = 1; i < length; i++)
	{
		char unsorted_query[MAX_QUERY_LENGTH];
		strcpy(unsorted_query, queries[i]); // copy the query at position i to the unsorted variable
		unsigned long unsorted_weight = weights[i];
		for (int j = i-1; j >= 0; j--)
		{
			char sorted_query[MAX_QUERY_LENGTH];
			strcpy(sorted_query, queries[j]);
			unsigned long sorted_weight = weights[j];
			if (strcmp(unsorted_query, sorted_query) < 0) // the unsorted query should move left
			{
				strcpy(queries[j+1], sorted_query); // shift the sorted query to the right
				weights[j+1] = sorted_weight;
			} else
			{
				strcpy(queries[j+1], unsorted_query); // put the unsorted stuff into the spot where it will be more than the things to its left
				weights[j+1] = unsorted_weight;
				break; // don't need to do any more shifting, so exit the loop through j
			}
			if (j == 0) {
				strcpy(queries[0],unsorted_query);
				weights[0] = unsorted_weight;
			}
		}
	}
}

int binarysearch(char queries[][MAX_QUERY_LENGTH], int start, int end, char input[MAX_QUERY_LENGTH]) {
	if (start == end) {
		// printf("returning from base case");
		return start; //base case, length of array is 1
	}
	int found = 0;
	int idx1 = start;
	int idx2 = end;
	int mid = (idx1 + idx2 -1)/2;
	while (idx1 <= idx2 && !found) {
		if ((idx1+idx2)%2 == 0) {
			mid = (idx1 + idx2)/2;
		} else {
			mid = (idx1 + idx2 -1)/2;
		}

		// printf("idx1: %i\n mid: %i\n idx2: %i\n", idx1,mid, idx2);
		if (strcmp(input, queries[mid]) == 0 || idx1 == idx2) {
			found = 1;
		} else if (strcmp(input, queries[mid]) > 0) {
			idx1 = mid + 1;
		} else {
			idx2 = mid - 1;
		}
	}
	if (mid < idx1) {
		mid = idx1;
	}
	// printf("found = %i\n", found);
	// printf("idx1 is %i mid is %i idx2 is %i\n", idx1,mid, idx2);
	// printf("%s\n",queries[mid]);
	return mid;
}

/*
*  Checks if str1 begins with str2
*/
bool beginsWith(char str1[MAX_QUERY_LENGTH], char str2[MAX_QUERY_LENGTH]) {
	int str1_sz = strlen(str1);
	int str2_sz = strlen(str2);
	if (str2_sz > str1_sz) { // The first string can't begin with the second because the second is longer
		return false;
	}
	for (int i = 0; i < str2_sz; i++) { // Make sure each character is the same
		if (str1[i] != str2[i]) {
			return false;
		}
	}
	return true;
}

int getTrueStart(char queries[][MAX_QUERY_LENGTH], int possible_loc, char input[MAX_QUERY_LENGTH]) {
	if (beginsWith(queries[possible_loc], input)) {
		return possible_loc;
	} else if (possible_loc != 0 && beginsWith(queries[possible_loc-1], input)) { // Check if the item before is the right one
		return possible_loc-1;
	} else if (possible_loc != sizeof(queries)/sizeof(queries[0]-1) && beginsWith(queries[possible_loc+1], input)) {
		return possible_loc+1;
	}
	return -1; // This should never happen
}

/**
   Main function
 */
int main() {

  // YOU MAY ADD CODE INBETWEEN BUT
  // DO NOT MODIFY THE FOLLOWING LINES OF CODE

  // name of the data files
  char *data_file = "data/wiktionary.txt";
  char *sorted_file = "data/sorted_wiktionary.txt";
  // open a file pointer
  FILE *fp = fopen(data_file, "r");

  // scan number of queries in the vocabulary file
  int num_queries;
  fscanf(fp, "%d", &num_queries);

  // declare the parallel arrays
  char queries[num_queries][MAX_QUERY_LENGTH];
  unsigned long weights[num_queries];

  // read the data into the arrays
  load(queries, weights, fp);

  // always remember to close file pointers!
  fclose(fp);


	// Sort the arrays of queries and weights together, based on alphabetical of queries
	insertionsort(queries, weights, num_queries);

  // write the sorted arrays to a file
  fp = fopen(sorted_file, "w");
  write(queries, weights, fp, num_queries);
  fclose(fp);
  // END OF DO NOT MODIFY

	char input[MAX_QUERY_LENGTH];
	char input_line[MAX_QUERY_LENGTH];
	int start_loc = 0;
	//int end_loc = 0;

	while (fgets(input_line, MAX_QUERY_LENGTH, stdin) != NULL) { //until EOF is found

		sscanf(input_line, "%s", input);

		printf("Input is:   %s\n", input);

		int possible_loc = binarysearch(queries, 0, num_queries, input);
		printf("Found word:   %s\n", queries[possible_loc]);

		start_loc = getTrueStart(queries, possible_loc, input);
		if (start_loc < 0) {
			printf("Error: Start was found to be less than 0");
		} else {
			printf("The real word should be:  %s\n", queries[start_loc]);
		}
		// getAutoCompleteOptions(queries, )

	}

	// while loop to take in user input for query desired, until they don't want to any more
	// binary search through the sorted array
	// find the subarray of matching possible num_queries
	// sort by weight
	// output the top 5 operations
	// continue while loop by asking for input again

  return 0;
}
