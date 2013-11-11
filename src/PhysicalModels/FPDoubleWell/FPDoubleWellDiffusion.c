#include <stdlib.h>
#include <basic.h>
#include <arrayfunctions.h>
#include <physicalmodels/fpdoublewell.h>
#include <hypar.h>

int FPDoubleWellDiffusion(double *f,double *u,int dir,void *s,double t)
{
  HyPar         *solver = (HyPar*)        s;
  FPDoubleWell  *param  = (FPDoubleWell*) solver->physics;
  int           ierr    = 0, i, v;

  int *dim    = solver->dim_local;
  int ghosts  = solver->ghosts;
  int ndims   = solver->ndims;
  int nvars   = solver->nvars;
  int index[ndims], bounds[ndims], offset[ndims];

  /* set bounds for array index to include ghost points */
  ierr = ArrayCopy1D_int(dim,bounds,ndims); CHECKERR(ierr);
  for (i=0; i<ndims; i++) bounds[i] += 2*ghosts;

  /* set offset such that index is compatible with ghost point arrangement */
  ierr = ArraySetValue_int(offset,ndims,-ghosts); CHECKERR(ierr);

  int done = 0; ierr = ArraySetValue_int(index,ndims,0); CHECKERR(ierr);
  while (!done) {
    int p = ArrayIndex1D(ndims,dim,index,offset,ghosts);
    for (v = 0; v < nvars; v++) f[nvars*p+v] = 0.5*param->q * u[nvars*p+v];
    done = ArrayIncrementIndex(ndims,bounds,index);
  }

  return(0);
}
