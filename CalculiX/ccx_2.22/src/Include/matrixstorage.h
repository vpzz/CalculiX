/*     CALCULIX - A 3-dimensional finite element program                 */
/*              Copyright (C) 1998 Guido Dhondt                          */
/*     This program is free software; you can redistribute it and/or     */
/*     modify it under the terms of the GNU General Public License as    */
/*     published by the Free Software Foundation; either version 2 of    */
/*     the License, or (at your option) any later version.               */

/*     This program is distributed in the hope that it will be useful,   */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/*     GNU General Public License for more details.                      */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software       */
/*     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.         */
#include "CalculiX.h"
void matrixstorage(double *ad, double **aup, double *adb, double *aub,
                   double *sigma, ITG *icol, ITG **irowp, ITG *neq, ITG *nzs,
                   ITG *ntrans, ITG *inotr, double *trab, double *co, ITG *nk,
                   ITG *nactdof, char *jobnamec, ITG *mi, ITG *ipkon,
                   char *lakon, ITG *kon, ITG *ne, ITG *mei, ITG *nboun,
                   ITG *nmpc, double *cs, ITG *mcs, ITG *ithermal,
                   ITG *nmethod);
