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

/*     Parallellization of the external work calculation 	 */

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Include/CalculiX.h"

static ITG *nkapar = NULL, *nkbpar = NULL, *mt1;

static double *allwk1, *fnext1, *fnextini1, *v1, *vini1;

void worparll(double *allwk, double *fnext, ITG *mt, double *fnextini,
              double *v, double *vini, ITG *nk, ITG *num_cpus) {
  ITG i, idelta, isum, num_cpus_loc;

  /* variables for multithreading procedure */

  ITG *ithread = NULL;

  pthread_t tid[*num_cpus];

  /* check that num_cpus does not exceed nk */

  if (*num_cpus > *nk) {
    num_cpus_loc = *nk;
  } else {
    num_cpus_loc = *num_cpus;
  }

  /* determining the element bounds in each thread */

  NNEW(nkapar, ITG, num_cpus_loc);
  NNEW(nkbpar, ITG, num_cpus_loc);
  NNEW(allwk1, double, num_cpus_loc);

  /* dividing the element number range into num_cpus equal numbers of
     active entries.  */

  idelta = (ITG)floor(*nk / (double)(num_cpus_loc));
  isum = 0;
  for (i = 0; i < num_cpus_loc; i++) {
    nkapar[i] = isum;
    if (i != num_cpus_loc - 1) {
      isum += idelta;
    } else {
      isum = *nk;
    }
    nkbpar[i] = isum;
  }

  /* create threads and wait */

  fnext1 = fnext;
  mt1 = mt;
  fnextini1 = fnextini;
  v1 = v;
  vini1 = vini;

  NNEW(ithread, ITG, num_cpus_loc);

  for (i = 0; i < num_cpus_loc; i++) {
    ithread[i] = i;
    pthread_create(&tid[i], NULL, (void *)worparllmt, (void *)&ithread[i]);
  }
  for (i = 0; i < num_cpus_loc; i++) pthread_join(tid[i], NULL);

  for (i = 0; i < num_cpus_loc; i++) {
    (*allwk) += allwk1[i];
  }

  SFREE(ithread);
  SFREE(nkapar);
  SFREE(nkbpar);
  SFREE(allwk1);
}

/* subroutine for multithreading of worparll */

void *worparllmt(ITG *i) {
  ITG nka, nkb, k, j;

  nka = nkapar[*i];
  nkb = nkbpar[*i];

  for (k = nka; k < nkb; k++) {
    for (j = 1; j < 4; j++) {
      allwk1[*i] += (fnext1[k * *mt1 + j] + fnextini1[k * *mt1 + j]) *
                    (v1[k * *mt1 + j] - vini1[k * *mt1 + j]) / 2.;
    }
  }

  return NULL;
}
