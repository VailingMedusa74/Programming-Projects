/* file : takuzu2.c*/
/* author : Shady Gmira (s.gmira@student.rug.nl) */
/* date : Friday October 21st 2022 */
/* version : 1.0 */

/* Description: A Takuzu solver */


#include <stdio.h>
#include <stdlib.h>

void *safeMalloc(int n) { //A safer version of malloc
  void *p = malloc(n);
  if (p == NULL) { 
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return p;
}
void readIn(int dim, char **takuzu) { //Read user input
  for (int r = 0; r < dim; r++) {
    for (int c = 0; c < dim; c++) {
      scanf("%c", &takuzu[r][c]);
    }
    getchar(); //Eat the \n
  }
}
void printTakuzu(int dim, char **takuzu) { //Print the board
  for (int r = 0; r < dim; r++) {
    for (int c = 0; c < dim; c++) {
      printf("%c", takuzu[r][c]);
    }
    printf("\n"); //Eat the \n
  }
}
int hasDifferentArrays(char **arr, int dim){
  /* checks if there are no identical rows or cols */
  for (int i = 0; i < dim; i++) {
    for(int j = i+1; j < dim; j++) {
      int count = 0;
      for(int k = 0; k < dim; k++)
        if(arr[i][k] == arr[j][k]) {
         count++; 
        }
      if (count == dim) {
        return 0;
      }
    }
  }
  return 1;   
}

int hasEvenDistribution(char **arr, int dim){
  /* checks if the rows or cols have an equal number of 0s and 1s */
  for (int i=0; i < dim; i++) {
    int zeros = 0; 
    for (int j=0; j < dim; j++) {
      if (arr[i][j] == '0') {
        zeros++;
      }
    }
    if (zeros != dim/2) {
      return 0;
    }
  }
  return 1; 
} 

int hasNoThreeAdjacentNumbers(char **arr, int dim){
  /* checks if no row or col contains more than two adjacent 0s or 1s */
  for (int i =0; i < dim - 1; i++) {
    for (int j = 0; j < dim - 2; j++) {
      if ((arr[i][j] == arr[i][j + 1]) && (arr[i][j] == arr[i][j + 2])) {
        return 0;
      }
    }
  } 
  return 1; 
}

int isSolved(int dim, char **takuzu) { //Checks if a full takuzu is a valid solution or not 
  char **takuzuTwo = safeMalloc(sizeof(char*)*dim);
  for (int i = 0; i < dim; i++) {
    takuzuTwo[i] = safeMalloc(sizeof(char)*dim);
  }
  for (int r = 0; r < dim; r++) {
    for (int c = 0; c < dim; c++) {
      takuzuTwo[r][c] = takuzu[c][r]; //Making a sideways flipped takuzu
    }
  }
  //Checks for both takuzus (normal direction and rotated one) if the logic is valid
  if ((hasDifferentArrays(takuzu, dim)) && (hasNoThreeAdjacentNumbers(takuzu, dim)) && (hasEvenDistribution(takuzu, dim)) && (hasDifferentArrays(takuzuTwo, dim)) && (hasNoThreeAdjacentNumbers(takuzuTwo, dim)) && (hasEvenDistribution(takuzuTwo, dim))) {
    for (int i = 0; i < dim; i++) {
      free(takuzuTwo[i]);
    }
    free(takuzuTwo); //Freeing the rotated copy of the takuzu
    return 1;
  }
  for (int i = 0; i < dim; i++) {
    free(takuzuTwo[i]);
  }
  free(takuzuTwo); //Freeing the rotated copy of the takuzu
  return 0;
}

int isValid(int dim, char **takuzu, int x, int y) { //Checks if a move I'm about to make is valid or not
  int zero = 0;
  int one = 0;
  int none = 0;
  for (int x = 0; x < dim - 2; x++) {
    if ((dim > 2) && (takuzu[y][x] == takuzu[y][x + 1]) && (takuzu[y][x] == takuzu[y][x + 2]) && (takuzu[y][x] != '.')) { //Checks if there are no more than 2 of the same numbers in a row
      return 0;
    }
  }
  
  for (int y = 0; y < dim - 2; y++) {
    if ((dim > 2) && (takuzu[y][x] == takuzu[y + 1][x]) && (takuzu[y][x] == takuzu[y + 2][x]) && (takuzu[y][x] != '.')) { //Checks if there are no more than 2 of the same number in a col
      return 0;
    }
  }
  for (int i = 0; i < dim; i++) {
    if (takuzu[y][i] == '0') {
      zero++;
    }
    if (takuzu[y][i] == '1') {
      one++;
    }
    if (takuzu[y][i] == '.') {
      none++;
    }
  }
  if ((none == 0) && (zero != one)) { //Checking if I have the same number of 1's amd 0's in a row
    return 0;
  }
  
  zero = 0;
  one = 0;
  none = 0;
  
   for (int i = 0; i < dim; i++) { //Checking if I have the same number of 1's amd 0's in a column
    if (takuzu[i][x] == '0') {
      zero++;
    }
    if (takuzu[i][x] == '1') {
      one++;
    }
    if (takuzu[i][x] == '.') {
      none++;
    }
  }
  if ((none == 0) && (zero != one)) {
    return 0;
  }
  return 1;
}


void solver(int dim, char **takuzu, int i) { //Driver code for the backtracking solution
  if (i == (dim*dim)) { //Base case where I've iterated through the entire 2D array
    if (isSolved(dim, takuzu)) { //2nd base case where I check if a takuzu board is a valid solution or not
      printTakuzu(dim, takuzu);
      exit(0); //Quit the program to stop having to search for other solutions (optimization)
    }
      return;  
  }
  
  int x = i%dim; //Columns num
  int y = i/dim; //Row num
  if (takuzu[y][x] == '.') { //Recurse until I find a '.' then which I try different values to see which fit
    takuzu[y][x] = '0';
    if (isValid(dim, takuzu, x, y)) { //First try with a 0 to see if it is a valid move
      solver(dim, takuzu, i + 1); //If it is a valid move I recurse and increase i(the number of elements found in the 2D array) by 1
    }
    takuzu[y][x] = '1';
    if (isValid(dim, takuzu, x, y)) { //Same as what I did for 0
      solver(dim, takuzu, i + 1);
    }
    takuzu[y][x] = '.'; //If nothing is found reset the value of my takuzu
    return;
  } else {
    solver(dim, takuzu, i + 1); //If I come accross a 1 or a 0 I just move onto the next element
  }
}

int main(int argc, char *argv[]) { 
  int dim; //Dimensions of the takuzu board
  scanf("%d", &dim);
  getchar(); //Eat the \n
  char **takuzu = safeMalloc(sizeof(char*)*dim);
  for (int i = 0; i < dim; i++) {
    takuzu[i] = safeMalloc(sizeof(char)*dim);
  }
  readIn(dim, takuzu); //Read user input
  solver(dim, takuzu, 0); //First recursive call
  
  for (int i = 0; i < dim; i++) { //Free memory
    free(takuzu[i]);
  }
  free(takuzu);
  
  
  return 0;
}
