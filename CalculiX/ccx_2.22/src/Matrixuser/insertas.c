/*     CalculiX - A 3-dimensional finite element program                 */
/*              Copyright (C) 1998-2024 Guido Dhondt                     */

/*     This program is free software; you can redistribute it and/or     */
/*     modify it under the terms of the GNU General Public License as    */
/*     published by the Free Software Foundation(version 2);    */
/*                                                                       */

/*     This program is distributed in the hope that it will be useful,   */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/*     GNU General Public License for more details.                      */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software       */
/*     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.         */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Include/CalculiX.h"
#include "../Include/mortar.h"

/*  inserts a new nonzero matrix position into the sparse matrix data structure
 */

void insertas(ITG **irowp, ITG **mast1p, ITG *i1, ITG *i2, ITG *ifree,
              ITG *nzs_, double *contribution, double **bdp) {
  /*   inserts a new nonzero matrix position into the data structure
       the structure is not assumed to be symmetric
       i1: row number (FORTRAN convention)
       i2: column number (FORTRAN convention) */

  ITG idof1, idof2, *irow = NULL, *mast1 = NULL, nzs_old, i;
  double *bd = NULL;

  irow = *irowp;
  mast1 = *mast1p;
  bd = *bdp;

  idof1 = *i1;
  idof2 = *i2;

  if (*ifree > *nzs_) {
    nzs_old = *nzs_;
    *nzs_ = (ITG)(1.5 * *nzs_ + 1);
    RENEW(irow, ITG, *nzs_);
    for (i = nzs_old; i < *nzs_; i++) {
      irow[i] = 0;
    }
    RENEW(mast1, ITG, *nzs_);
    for (i = nzs_old; i < *nzs_; i++) {
      mast1[i] = 0;
    }
    RENEW(bd, double, *nzs_);
    for (i = nzs_old; i < *nzs_; i++) {
      bd[i] = 0.;
    }
  }
  mast1[*ifree - 1] = idof2;
  irow[*ifree - 1] = idof1;
  bd[*ifree - 1] = *contribution;
  ++*ifree;

  *irowp = irow;
  *mast1p = mast1;
  *bdp = bd;

  return;
}
