/**
   Skeleton of the Autocomplete assignment
   that allows for loading and writing
   a datafile
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	//check for length of 1
	if (end >= 1) {
		int mid = start + (end - 1)/2;
		if (strcmp(queries[mid], input) == 0) { // base case if perfect
			return mid;
		}
		if (strcmp(queries[mid],input) > 0 ) { //input is on the right side of the middle query
			return binarysearch(queries, mid, end, input);
		}
		return binarysearch(queries, start, mid, input); //input is on the left side of the middle query
	}
	return -1; //if didn't find it

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

	int getanother = 1;
	while (getanother) {
		char input[MAX_QUERY_LENGTH];
		printf("Input a word to return the autocomplete options (enter 00 to stop): ");
		fgets(input, MAX_QUERY_LENGTH, stdin);
		printf("%s\n", input);
		if (input[0] == '0' && input[1] == '0') {
			getanother = 0;
		}
		int location = binarysearch(queries, 0, num_queries, input);
		char the[] = {'t','h','e'};
		printf("%i\n", strcmp(input,the));
		printf("%lu\n", sizeof(input));
		printf("%i\n", location);



	}

	// while loop to take in user input for query desired, until they don't want to any more
	// binary search through the sorted array
	// find the subarray of matching possible num_queries
	// sort by weight
	// output the top 5 operations
	// continue while loop by asking for input again

  return 0;
}
