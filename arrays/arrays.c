#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;


/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
  Array *array = malloc(sizeof(Array));

  // Set initial values for capacity and count
  array->count = 0;
  array->capacity = capacity;

  // Allocate memory for elements
  array->elements = malloc(capacity * sizeof(char *));
  for (int i = 0; i < array->capacity; i++) {
    array->elements[i] = NULL;
  }
  return array;
}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {
  printf("in destroy function\n");
  printf("capacity is %d\n", arr->capacity);

  // Free all elements
  /* for (int i = 0; i < arr->capacity; i++) { */
  /*   printf("freeing %d\n", i); */
  /*   /1* printf("%s\n", arr->elements[i]); *1/ */
  /*   free(arr->elements[i]); */
  /* } */

  /* I'm a bit confused as to why the above for loop causes segfaults. I thought I should have to free each pointer, but it will crash and the tests won't pass.  If I just free arr->elements, as below, it works fine. But why?  */

  free(arr->elements);

  // Free array
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {

  // Create a new element storage with double capacity
  char **new_elements = malloc(arr->capacity * 2 * sizeof(char **));

  // Copy elements into the new storage
  for (int i = 0; i < arr->capacity; i++) {
    new_elements[i] = arr->elements[i];
  }

  // Free the old elements array (but NOT the strings they point to)
  free(arr->elements);

  // Update the elements and capacity to new values
  arr->elements = new_elements;
  arr->capacity = arr->capacity * 2;
}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater or equal to than the current count
  if (index >= arr->count) {
    fprintf(stderr, "read: The index is greater than the array's count\n");
    return NULL;
  } else {
  // Otherwise, return the element at the given index
    return (arr->elements[index]);
  }
}


/*****
 * Insert an element to the array at the given index
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if (index > arr->count) {
    fprintf(stderr, "insert: The index is greater than the array's count\n");
    exit(1);
  }

  // Resize the array if the number of elements is over capacity
  if (arr->count >= arr->capacity) {
    resize_array(arr);
  }

  // Move every element after the insert index to the right one position
  for (int i = arr->count; i > index; i--) {
    arr->elements[i] = arr->elements[i - 1];
  }

  // Copy the element and add it to the array
  /* char str[strlen(element) + 1]; */
  /* strcpy(str, element); */
  arr->elements[index] = strdup(element);

  // Increment count by 1
  arr->count++;
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // Resize the array if the number of elements is over capacity
  // or throw an error if resize isn't implemented yet.
  if (arr->count >= arr->capacity) {
    resize_array(arr);
  }

  // Copy the element and add it to the end of the array
  /* char str[strlen(element) + 1]; */
  /* strcpy(str, element); */
  arr->elements[arr->count] = strdup(element);

  // Increment count by 1
  arr->count++;

}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  int index = -1;
  // Search for the first occurence of the element and remove it.
  for (int i = 0; i < arr->count; i++) {
    printf("element %d is %s\n", i, arr->elements[i]);
    if (strcmp(arr->elements[i], element) == 0) {
      printf("match found at index %d\n", i);
      index = i;
      break;
    }
  }
  if (index == -1) {
    fprintf(stderr, "Element not found\n");
  } else {
    // Don't forget to free its memory!
    /* why would we want to free its memory? we're not decreasing the capacity, right? we just want
       to remove the element. Shouldn't we just NULL it out? 
       Oh, I get it. We're not freeing the array, we're freeing the string it points to
       */
    // Shift over every element after the removed element to the left one position
    for (int i = index; i < arr->count - 1; i++) {
      arr->elements[i] = arr->elements[i + 1];
    }
    /* printf("removing %s", arr->elements[arr->count]); */
    free(arr->elements[arr->count - 1]);
    // Decrement count by 1
    arr->count--;
    printf("array shuffled: results:\n[");
    for (int i = 0; i < arr->count; i++) {
      printf("%s\n", arr->elements[i]);
    }
    printf("]\n");
  }
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0);
  printf("capacity: %d\n", arr->capacity);
  arr_append(arr, "STRING4");
  arr_print(arr);
  printf("capacity: %d\n", arr->capacity);
  arr_insert(arr, "STRING2", 0);
  arr_print(arr);
  printf("capacity: %d\n", arr->capacity);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  printf("capacity: %d\n", arr->capacity);
  arr_remove(arr, "STRING3");
  arr_print(arr);
  printf("capacity: %d\n", arr->capacity);

  destroy_array(arr);

  return 0;
}
#endif
