/*     CalculiX - A 3-dimensional finite element program                 */
/*              Copyright (C) 1998-2024 Guido Dhondt                          */

/*     This program is free software; you can redistribute it and/or     */
/*     modify it under the terms of the GNU General Public License as    */
/*     published by the Free Software Foundation(version 2);    */
/*                    */

/*     This program is distributed in the hope that it will be useful,   */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/*     GNU General Public License for more details.                      */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software       */
/*     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.         */

/*     Parallellization of subroutine of calcresidual.c          	 */

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Include/CalculiX.h"

static ITG *neapar = NULL, *nebpar = NULL;

static double *cv1, *alpham1, *adb1, *aux21, *b1, *scal11, *alpha1, *cvini1;

void res4parll(double *cv, double *alpham, double *adb, double *aux2, double *b,
               double *scal1, double *alpha, double *cvini, ITG *neq0,
               ITG *num_cpus) {
  ITG i, idelta, isum, num_cpus_loc;

  /* variables for multithreading procedure */

  ITG *ithread = NULL;

  pthread_t tid[*num_cpus];

  /* check that num_cpus does not exceed neq0*/

  if (*num_cpus > *neq0) {
    num_cpus_loc = *neq0;
  } else {
    num_cpus_loc = *num_cpus;
  }

  /* determining the element bounds in each thread */

  NNEW(neapar, ITG, num_cpus_loc);
  NNEW(nebpar, ITG, num_cpus_loc);

  /* dividing the element number range into num_cpus equal numbers of
     active entries.  */

  idelta = (ITG)floor(*neq0 / (double)(num_cpus_loc));
  isum = 0;
  for (i = 0; i < num_cpus_loc; i++) {
    neapar[i] = isum;
    if (i != num_cpus_loc - 1) {
      isum += idelta;
    } else {
      isum = *neq0;
    }
    nebpar[i] = isum;
  }

  /* create threads and wait */

  cv1 = cv;
  alpham1 = alpham;
  adb1 = adb;
  aux21 = aux2;
  b1 = b;
  scal11 = scal1;
  alpha1 = alpha;
  cvini1 = cvini;

  NNEW(ithread, ITG, num_cpus_loc);

  for (i = 0; i < num_cpus_loc; i++) {
    ithread[i] = i;
    pthread_create(&tid[i], NULL, (void *)res4parllmt, (void *)&ithread[i]);
  }
  for (i = 0; i < num_cpus_loc; i++) pthread_join(tid[i], NULL);

  SFREE(ithread);
  SFREE(neapar);
  SFREE(nebpar);
}

/* subroutine for multithreading of res4parll */

void *res4parllmt(ITG *i) {
  ITG nea, neb, k;

  nea = neapar[*i];
  neb = nebpar[*i];

  for (k = nea; k < neb; ++k) {
    cv1[k] = *alpham1 * adb1[k] * aux21[k];
    b1[k] -= *scal11 * cv1[k] - alpha1[0] * cvini1[k];
  }

  return NULL;
}
