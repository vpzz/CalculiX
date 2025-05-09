
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Include/CalculiX.h"

/*---------------------------------------------------------------------*/
/* Strings vergleichen (bis zu welchem character sind sie gleich?)     */
/*---------------------------------------------------------------------*/

int compare(char *str1, char *str2, int length) {
  int i;

  i = 0;
  while ((str1[i] == str2[i]) && (i < length)) i++;

  return i; /* Return how far we got before a difference occurred, or the
               variable      */
  /* length, whichever is the smaller                                         */
}
