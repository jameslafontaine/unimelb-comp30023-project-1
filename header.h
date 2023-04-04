/**
 * Include function prototypes and type definitions for the correspoding .c file here

 * Document how to use the function in the header file, or more accurately close to the declaration.

 * Document how the function works (if it's not obvious from the code) in the source file, or more accurately, close to the definition.

 * For the birds-eye thing in the header, you don't necessarily need the documentation that close - you can document groups of declarations at once.

 * Broadly speaking, the caller should be interested in errors and exceptions (if only so they can be translated as they propogate through the layers of abstraction) so these should be documented close to the relevant declarations. * 

*/

/* Example of qsort using your own cmpfunc that appropriately compares the data type being used

int values[] = { 88, 56, 100, 2, 25 };

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}


qsort(values, 5, sizeof(int), cmpfunc);
*/