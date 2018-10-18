/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_T_category_get_categories */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_T_category_get_categories = PMPI_T_category_get_categories
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_T_category_get_categories  MPI_T_category_get_categories
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_T_category_get_categories as PMPI_T_category_get_categories
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_T_category_get_categories(int cat_index, int len, int indices[])
    __attribute__ ((weak, alias("PMPI_T_category_get_categories")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_T_category_get_categories
#define MPI_T_category_get_categories PMPI_T_category_get_categories

/* any non-MPI functions go here, especially non-static ones */

int MPIR_T_category_get_categories_impl(int cat_index, int len, int indices[])
{
    int mpi_errno = MPI_SUCCESS;
    cat_table_entry_t *cat;
    int i, num_subcats, count;

    cat = (cat_table_entry_t *) utarray_eltptr(cat_table, cat_index);
    num_subcats = utarray_len(cat->subcat_indices);
    count = len < num_subcats ? len : num_subcats;

    for (i = 0; i < count; i++) {
        indices[i] = *(int *) utarray_eltptr(cat->subcat_indices, i);
    }

    return mpi_errno;
}

#endif /* MPICH_MPI_FROM_PMPI */

/*@
MPI_T_category_get_categories - Get sub-categories in a category

Input Parameters:
+ cat_index - index of the category to be queried, in the range [0,N-1] (integer)
- len - the length of the indices array (integer)

Output Parameters:
. indices - an integer array of size len, indicating category variable indices (array of integers)

.N ThreadSafe

.N Errors
.N MPI_SUCCESS
.N MPI_T_ERR_NOT_INITIALIZED
.N MPI_T_ERR_INVALID_INDEX
@*/
int MPI_T_category_get_categories(int cat_index, int len, int indices[])
{
    int mpi_errno = MPI_SUCCESS;

    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_T_CATEGORY_GET_CATEGORIES);
    MPIT_ERRTEST_MPIT_INITIALIZED();
    MPIR_T_THREAD_CS_ENTER();
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_T_CATEGORY_GET_CATEGORIES);

    /* Validate parameters */
    MPIT_ERRTEST_CAT_INDEX(cat_index);
    if (len != 0)
        MPIT_ERRTEST_ARGNULL(indices);

    /* ... body of routine ...  */

    if (len == 0)
        goto fn_exit;

    mpi_errno = MPIR_T_category_get_categories_impl(cat_index, len, indices);
    MPIR_ERR_CHECK(mpi_errno);

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_T_CATEGORY_GET_CATEGORIES);
    MPIR_T_THREAD_CS_EXIT();
    return mpi_errno;

  fn_fail:
    goto fn_exit;
}
