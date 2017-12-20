/*
 * Include the provided hashtable library
 */
#include "hashtable.h"

/*
 * Include the header file
 */
#include "philspel.h"

/*
 * Standard IO and file routines
 */
#include <stdio.h>

/*
 * General utility routines (including malloc())
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routine
 */
#include <string.h>

/*
 * this hashtable stores the dictionary
 */
HashTable *dictionary;

/*
 * the MAIN routine.  You can safely print debugging information
 * to standard error (stderr) and it will be ignored in the grading
 * process in the same way which this does.
 */
int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /* main in C should always return 0 as a way of telling
     whatever program invoked this that everything went OK
     */
  return 0;
}

/*
 * You need to define this function. void *s can be safely casted
 * to a char * (null terminated string) which is done for you here for
 * convenience.
 */
unsigned int stringHash(void *s){
  char *string = (char *) s;

/* used the djb2 hash function developed by Dan Bernstein: http://www.cse.yorku.ca/~oz/hash.html */
	unsigned int sum = 0;
	unsigned int addr = 0;
  	while ((addr = *string)) {
    	sum = (sum << 5) - sum + addr;
    	string++;
  }
  return sum;
}

/*
 * You need to define this function.  It should return a nonzero
 * value if the two strings are identical (case sensitive comparison)
 * and 0 otherwise.
 */
int stringEquals(void *s1, void *s2){
  char *string1 = (char *) s1;
  char *string2 = (char *) s2;

  if(strcmp(string1, string2) != 0)
  	return 1;
  else
  	return 0;
}

/*
 * this function should read in every word in the dictionary and
 * store it in the dictionary.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, exit.  You will need to allocate
 * (using malloc()) space for each word.  As described in the specs, you
 * can initially assume that no word is longer than 70 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You can NOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(0)
 * to cleanly exit the program. Since the format is one word at a time, with
 * returns in between, you can
 * safely use fscanf() to read in the strings.
 */
void readDictionary(char *filename){

  /* C syntax and I/O strategy taken from http://www.cprogramming.com/tutorial/cfileio.html */

	FILE* filePointer;
	int counter = 0;
	int limit = 70;
	char* characterPointer;
	char character;
	char* characterMemory = (char*) malloc(limit);

	/* open the file and see if the file name is valid */

	if ((filePointer = fopen(filename, "r+")) == NULL) {
		fprintf(stderr, "Load Failed\n");
		exit(0);
	}  

	/* realized that we need another variable to manipulate the string memory incase of unbounded strings */

	int resize_array = limit;

	/* while loop reads individual characters in file until the "EOF" */

	while((character = fgetc(filePointer)) != EOF) {
		if (character != '\n') {
		  characterMemory[counter] = character;
		  characterMemory[counter + 1] = '\0';
		  counter = counter + 1;
			
		  	/* Allows us to premptively resize the memory we had allotted by 4 memory units */
			if (counter == resize_array - 4) {
		    	resize_array = resize_array * 2;
		    	characterMemory = (char*) realloc(characterMemory, resize_array * sizeof(char));
		  	}
		} else {
			characterPointer = strdup(characterMemory);
			insertData(dictionary, characterPointer, characterPointer);
			resize_array = limit;
			counter = 0;
			characterMemory[0] = '\0';
		}
	}
	if (characterMemory[0] != '\0') {
	    characterPointer = strdup(characterMemory);
	    insertData(dictionary, characterPointer, characterPointer);
  	}
	free(characterMemory);
	fclose(filePointer);
}


/*
 * This should process standard input and copy it to standard output
 * as specified in specs.  EG, if a standard dictionary was used
 * and the string "this is a taest of  this-proGram" was given to
 * standard input, the output to standard output (stdout) should be
 * "this is a teast [sic] of  this-proGram".  All words should be checked
 * against the dictionary as they are input, again with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not being found, by appending " [sic]" after the
 * error.
 *
 * Since we care about preserving whitespace, and pass on all non alphabet
 * characters untouched, and with all non alphabet characters acting as
 * word breaks, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), so you will probably have
 * to get characters from standard input one at a time.
 *
 * As stated in the specs, you can initially assume that no word is longer than
 * 70 characters, but you may have strings of non-alphabetic characters (eg,
 * numbers, punctuation) which are longer than 70 characters. For the final 20%
 * of your grade, you can no longer assume words have a bounded length.
 */

int checkDict(struct HashTable * dictionary, void* string) {
		int length = strlen(string);
		char s1[length];  

		strcpy(s1, string);
		fprintf(stderr, "%s\n", s1);

		char s2[length];  

		strcpy(s2, string);
		char *p;
		for(p = s2; *p; ++p) {
			//insert code to make s2 lowercase
			*p = tolower(*p);
		}
 		fprintf(stderr, "%s\n", s2);
			
		char s3[length];  

		strcpy(s3, s2);
		s3[0] = s1[0];
		fprintf(stderr, "%s\n", s3);

		return (findData(dictionary, s1) == NULL) + (findData(dictionary, s2) == NULL) + (findData(dictionary, s3) == NULL);
	}

void processInput(){

	FILE* filePointer = fopen("testInput", "r+");

	int counter = 0;
	int limit = 70;
	char* characterPointer;
	char character;
	char* characterMemory = (char*) malloc(limit);

	int resize_array = limit;


	while((character = fgetc(stdin)) != EOF) {
	    if (character != '\n') {
	    	characterMemory[counter] = character;
	    	characterMemory[counter + 1] = '\0';
	    	counter = counter + 1;
	    	if (counter == resize_array - 3) {
	    		resize_array = resize_array * 2;
	    		characterMemory = (char*) realloc(characterMemory, resize_array * sizeof(char));
	    	}
	    } else if (characterMemory[0] == '\0') {
	    	printf("%c", character);
	    } else {
			if (checkDict(dictionary, characterMemory)) {
   				printf("%s", characterMemory);
  			} else {
    			printf("%s [sic]", characterMemory);
    		}
   	    	resize_array = limit;
	    	counter = 0;
	    	characterMemory[0] = '\0';
	    	printf("%c", character);
	    }
	}

	if (characterMemory[0] != '\0') {
		if (checkDict(dictionary, characterMemory)) {
   			printf("%s", characterMemory);
  		} else {
    		printf("%s [sic]", characterMemory);
  		}

	}

	fclose(filePointer);
	free(characterMemory);
}












