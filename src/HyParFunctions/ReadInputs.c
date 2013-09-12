#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <basic.h>
#include <mpivars.h>
#include <hypar.h>

int ReadInputs(void *s,void *m)
{
  HyPar         *solver = (HyPar*) s;
  MPIVariables  *mpi    = (MPIVariables*) m;
  int           ierr    = 0;

  /* Only root process reads in the input files */
  if (!mpi->rank) {
    /* reading solver inputs */
    FILE *in;
    printf("Reading solver inputs from file \"solver.inp\".\n");
    in = fopen("solver.inp","r");
    if (!in) {
      fprintf(stderr,"Error: File \"solver.inp\" not found.\n");
      return(1);
    } else {
	    char word[_MAX_STRING_SIZE_];
      ierr = fscanf(in,"%s",word); if (ierr != 1) return(1);

      if (!strcmp(word, "begin")){
		    while (strcmp(word, "end")){
			    ierr = fscanf(in,"%s",word); if (ierr != 1) return(1);
          if (!strcmp(word, "ndims")) {
            ierr = fscanf(in,"%d",&solver->ndims); if (ierr != 1) return(1);
            solver->dim_global = (int*) calloc (solver->ndims,sizeof(int));
            mpi->iproc         = (int*) calloc (solver->ndims,sizeof(int));
          }	else if (!strcmp(word, "nvars"))			      ierr = fscanf(in,"%d",&solver->nvars);
    			else if   (!strcmp(word, "size")) {
            int i;
            if (!solver->dim_global) {
              fprintf(stderr,"Error in ReadInputs(): dim_global not allocated.\n");
              fprintf(stderr,"Please specify ndims before dimensions.\n"         );
              return(1);
            } else {
              for (i=0; i<solver->ndims; i++) ierr = fscanf(in,"%d",&solver->dim_global[i]);
              if (ierr != 1) return(1);
            }
          } else if (!strcmp(word, "iproc")) {
            int i;
            if (!mpi->iproc) {
              fprintf(stderr,"Error in ReadInputs(): iproc not allocated.\n");
              fprintf(stderr,"Please specify ndims before iproc.\n"         );
              return(1);
            } else {
              for (i=0; i<solver->ndims; i++) ierr = fscanf(in,"%d",&mpi->iproc[i]);
              if (ierr != 1) return(1);
            }
    			} else if (!strcmp(word, "ghost"))		        ierr = fscanf(in,"%d",&solver->ghosts);
		    	else if   (!strcmp(word, "n_iter"))		        ierr = fscanf(in,"%d",&solver->n_iter);
		    	else if   (!strcmp(word, "hyp_space_order"))	ierr = fscanf(in,"%d",&solver->hyp_space_scheme);
		    	else if   (!strcmp(word, "par_space_order"))	ierr = fscanf(in,"%d",&solver->par_space_scheme);
    			else if   (!strcmp(word, "time_order" ))	    ierr = fscanf(in,"%d",&solver->time_scheme);
    			else if   (!strcmp(word, "dt"))			          ierr = fscanf(in,"%lf",&solver->dt);
    			else if   (!strcmp(word, "screen_op_iter"))   ierr = fscanf(in,"%d",&solver->screen_op_iter);
    			else if   (!strcmp(word, "file_op_iter"))		  ierr = fscanf(in,"%d",&solver->file_op_iter);
    			else if   (!strcmp(word, "op_file_format"))   ierr = fscanf(in,"%s",solver->op_file_format);
    			else if   (!strcmp(word, "op_overwrite"))     ierr = fscanf(in,"%s",solver->op_overwrite);
          else if   ( strcmp(word, "end")) {
            char useless[_MAX_STRING_SIZE_];
            ierr = fscanf(in,"%s",useless);
            printf("Warning: keyword %s in file \"solver.inp\" with value %s not recognized or extraneous. Ignoring.\n",
                        word,useless);
          }
          if (ierr != 1) return(1);
        }
	    } else {
    		fprintf(stderr,"Error: Illegal format in file \"solver.inp\".\n");
        return(1);
	    }
      /* Print to screen the inputs read */
      int i;
	    printf("\tNo. of dimensions                          : %d\n",solver->ndims);
	    printf("\tNo. of variables                           : %d\n",solver->nvars);
	    printf("\tDomain size                                : ");
      for (i=0; i<solver->ndims; i++) printf ("%d ",solver->dim_global[i]);
      printf("\n");
#ifndef serial
	    printf("\tProcesses along each dimension             : ");
      for (i=0; i<solver->ndims; i++) printf ("%d ",mpi->iproc[i]);
      printf("\n");
#endif
	    printf("\tNo. of ghosts pts                          : %d\n",solver->ghosts);
	    printf("\tNo. of iter.                               : %d\n",solver->n_iter);
      printf("\tSpatial reconstruction scheme (hyperbolic) : %d\n",solver->hyp_space_scheme);
      printf("\tSpatial reconstruction scheme (parabolic ) : %d\n",solver->par_space_scheme);
      printf("\tTime integration scheme                    : %d\n",solver->time_scheme);
    	printf("\tTime Step                                  : %E\n",solver->dt);
      printf("\tScreen output iterations                   : %d\n",solver->screen_op_iter);
      printf("\tFile output iterations                     : %d\n",solver->file_op_iter);
      printf("\tSolution file format                       : %s\n",solver->op_file_format);
      printf("\tOverwrite solution file                    : %s\n",solver->op_overwrite);
    }
    fclose(in);
  }
  return(0);
}
