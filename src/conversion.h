/**
 * \file conversion.h
 *
 * \brief Conversion between mzed_t and mzd_slice_t
 *
 * \author Martin Albrecht <martinralbrecht@googlemail.com>
 */

#ifndef M4RIE_CONVERSION_H
#define M4RIE_CONVERSION_H

/******************************************************************************
*
*            M4RIE: Linear Algebra over GF(2^e)
*
*    Copyright (C) 2011 Martin Albrecht <martinralbrecht@googlemail.com>
*
*  Distributed under the terms of the GNU General Public License (GEL)
*  version 2 or higher.
*
*    This code is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*  The full text of the GPL is available at:
*
*                  http://www.gnu.org/licenses/
******************************************************************************/

#include <m4ri/m4ri.h>
#include <m4rie/mzed.h>
#include <m4rie/mzd_slice.h>

/**
 * \brief Pack a bitslice matrix into a packed represenation.
 *
 * \param A Matrix over GF(2^k) or NULL
 * \param Z Bitslice matrix over GF(2^k)
 *
 * \ingroup Constructions
 */

mzed_t *mzed_cling(mzed_t *A, const mzd_slice_t *Z);

/**
 * \brief Unpack the matrix Z into bitslice representation.
 *
 * \param A Bitslice matrix or NULL
 * \param Z Input matrix
 *
 * \ingroup Constructions
 */

mzd_slice_t *mzed_slice(mzd_slice_t *A, const mzed_t *Z);

/**
 * \brief Unpack the matrix Z over GF(2^2) into bitslice representation.
 *
 * Elements in GF(2^2) can be represented as x*a + y where a is a root
 * of x^2 + x + 1. A0 contains the coefficients for x while A1
 * contains the coefficients for y.
 *
 * \param A Zero bitslice matrix over GF(2^2)
 * \param Z Matrix over GF(2^2)
 */

mzd_slice_t *_mzed_slice2(mzd_slice_t *A, const mzed_t *Z);

/**
 * \brief Unpack the matrix Z over GF(2^e) into bitslice representation.
 *
 * \param A Zero bitslice matrix over GF(2^e)
 * \param Z Matrix over GF(2^e)
 */

mzd_slice_t *_mzed_slice4(mzd_slice_t *A, const mzed_t *Z);

/**
 * \brief Unpack the matrix Z over GF(2^e) into bitslice representation.
 *
 * \param A Zero bitslice matrix over GF(2^e)
 * \param Z Matrix over GF(2^e)
 */

mzd_slice_t *_mzed_slice8(mzd_slice_t *A, const mzed_t *Z);

/**
 * \brief Pack a bitslice matrix into a classical represenation over GF(2^2).
 *
 * Elements in GF(2^2) can be represented as c_1*a + c_0 where a is a
 * root of x^2 + x + 1. A1 contains the coefficients for c_1 while A0
 * contains the coefficients for c_0.
 *
 * \param A Matrix over GF(2^2), must be zero
 * \param Z Bitslice matrix over GF(2^2)
 */

mzed_t *_mzed_cling2(mzed_t *A, const mzd_slice_t *Z);


/**
 * \brief Pack a bitslice matrix into a classical represenation over GF(2^e) for e in {3,4}.
 *
 * \param A Matrix over GF(2^e), must be zero
 * \param Z Bitslice matrix over GF(2^e)
 */

mzed_t *_mzed_cling4(mzed_t *A, const mzd_slice_t *Z);

/**
 * \brief Pack a bitslice matrix into a classical represenation over GF(2^e) for e in {5,6,7,8}.
 *
 * \param A Matrix over GF(2^e), must be zero
 * \param Z Bitslice matrix over GF(2^e)
 */

mzed_t *_mzed_cling8(mzed_t *A, const mzd_slice_t *Z);

/**
 * \brief Compute C += A*B using Karatsuba multiplication of polynomials over GF(2).
 *
 * \param C Preallocated return matrix, may be NULL for automatic creation.
 * \param A Input matrix A.
 * \param B Input matrix B.
 *
 * \sa _mzd_slice_mul_karatsuba
 */

static inline mzed_t *_mzed_mul_karatsuba(mzed_t *C, const mzed_t *A, const mzed_t *B) {
  mzd_slice_t *As,*Bs,*Cs;
  if(C)
    Cs = mzed_slice(NULL,C);
  else
    Cs = NULL;
  As = mzed_slice(NULL,A);
  Bs = mzed_slice(NULL,B);

  Cs = _mzd_slice_mul_karatsuba(Cs, As, Bs);

  C = mzed_cling(C, Cs);

  mzd_slice_free(As);
  mzd_slice_free(Bs);
  mzd_slice_free(Cs);
  return C;
}

/**
 * \brief Compute C = A*B.
 *
 * \param C Preallocated return matrix, may be NULL for automatic creation.
 * \param A Input matrix A.
 * \param B Input matrix B.
 *
 * \sa _mzd_slice_mul_karatsuba
 */

static inline mzed_t *mzed_mul_karatsuba(mzed_t *C, const mzed_t *A, const mzed_t *B) {
  if (A->ncols != B->nrows || A->finite_field != B->finite_field) 
    m4ri_die("mzed_mul_karatsuba: rows, columns and fields must match.\n");
  if (C != NULL) {
    if (C->finite_field != A->finite_field || C->nrows != A->nrows || C->ncols != B->ncols) 
      m4ri_die("mzed_mul_karatsuba: rows and columns of returned matrix must match.\n");
    mzed_set_ui(C,0);
  }
  return _mzed_mul_karatsuba(C, A, B);
}

/**
 * \brief Compute C += A*B.
 *
 * \param C Preallocated return matrix.
 * \param A Input matrix A.
 * \param B Input matrix B.
 */

static inline mzed_t *mzed_addmul_karatsuba(mzed_t *C, const mzed_t *A, const mzed_t *B) {
  assert(C != NULL);
  if (A->ncols != B->nrows || A->finite_field != B->finite_field) 
    m4ri_die("mzed_addmul_karatsuba: rows, columns and fields must match.\n");
  if (C->finite_field != A->finite_field || C->nrows != A->nrows || C->ncols != B->ncols) 
    m4ri_die("mzed_addmul_karatsuba: rows and columns of returned matrix must match.\n");
  return _mzed_mul_karatsuba(C, A, B);
}

/**
 * \brief Recale the row r in A by X starting c.
 *
 * \param A Matrix
 * \param r Row index.
 * \param c Column index.
 * \param X Multiplier
 *
 * \ingroup RowOperations
 */

static inline void mzd_slice_rescale_row(mzd_slice_t *A, rci_t r, rci_t c, word x) {
  mzd_slice_t *A_w = mzd_slice_init_window(A, r, 0, r+1, A->ncols);
  mzed_t *A_we = mzed_cling(NULL, A_w);

  mzed_rescale_row(A_we, r, c, x);

  mzed_slice(A_w, A_we);
  mzed_free(A_we);
  mzd_slice_free_window(A_w);
}

#endif //M4RIE_CONVERSION_H
