void
matmul (int i_matdim,
        const double* pd_A,
        const double* pd_B,
        double* pd_C)
{
   int i, j, k;

   for (i = 0; i < i_matdim; i++)
     for (j = 0; j < i_matdim; j++) 
       for (k = 0; k < i_matdim; k++)
         {
           pd_C [i*i_matdim + j] += pd_A [i*i_matdim + k] * pd_B [k*i_matdim + 
j];
         }
}

