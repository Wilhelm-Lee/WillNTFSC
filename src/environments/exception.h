/*
 *     This file is part of project <https://github.com/WilliamPascal/JackNTFS>
 *     Copyright (C) 2023  William Pascal
 *
 *     This program is frexcep_enum software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Frexcep_enum Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  Sexcep_enum the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, sexcep_enum <https://www.gnu.org/licenses/>.
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>

#ifdef __cplusplus
__BEGIN_DECLS
#endif /* __cplusplus */

/* par1="Exception"=excep_e;
   par2="File"=__FILE__;
   par3="Line"=__LINE__;
   par4="Function"=__FUNCTION__ */
static const char *__restrict__ _EXCEP_FMT
    = "Threw the %s:\n\tat %s:%ld, func %s\n";
static const char *__restrict__ _DEF_EXCEP_FMT
    = "Threw the %s\n";

typedef enum E_excep
{
  /* JackNTFS Exceptions */
  PermissionDeniedException        = 0b00000000L,
  ReadOperationFailedException     = 0b00000001L,
  WriteOperationFailedException    = 0b00000010L,
  /* 0b0000_0011 */
  ExecuteOperationFailedException  = 0b00000100L,
  /* 0b0000_0101
     0b0000_0110 */
  OperationTimedOutException       = 0b00000111L,

  /* COOL_MinGW Exceptions */
  InstanceFailureException         = 0b10000000L,
  IllegalMemoryAccessException,
  InvalidArgumentException,
  OutOfBoundException,
  InvalidNullPointerException,
  OutOfMemoryException,

  /* Shared Exceptions */
  NoSuchElementException,
  UnknownException                 = 0b11111111L /* END OF ENUMERATION */
} excep_e;

static __inline__ const char *
__excep_etos(excep_e e)
{
  switch(e)
    {
      /* JackNTFS Exceptions */
      case PermissionDeniedException:
        return "PermissionDeniedException";
      case ReadOperationFailedException:
        return "ReadOperationFailedException";
      case WriteOperationFailedException:
        return "WriteOperationFailedException";
      case ExecuteOperationFailedException:
        return "ExecuteOperationFailedException";
      case OperationTimedOutException:
        return "OperationTimedOutException";
      /* COOL_MinGW Exceptions */
      case InstanceFailureException:
        return "InstanceFailureException";
      case IllegalMemoryAccessException:
        return "IllegalMemoryAccessException";
      case InvalidArgumentException:
        return "InvalidArgumentException";
      case OutOfBoundException:
        return "OutOfBoundException";
      case InvalidNullPointerException:
        return "InvalidNullPointerException";
      case OutOfMemoryException:
        return "OutOfMemoryException";
      /* Shared Exceptions */
      case NoSuchElementException:
        return "NoSuchElementException";
      /* Default Exception */
      default:
        return "UnknownException";
    }
}

typedef struct S_exception
{
  /* "excep_enum" stands for "exception enumeration (excep_e)" */
  excep_e _excep_enum;
  char *__restrict__ __file__;
  long int __line__;
  char *__restrict__ __function__;

  /* Recursive logic;
     Used for doing recursive logging output */
  S_exception *_prev;
  S_exception *_this;
} exception_t;

static __inline__ void
init_S_exception(exception_t *excep)
{
  excep = (S_exception *)malloc(sizeof(S_exception));
  excep->_prev         = NULL;
  excep->_this         = NULL;
  excep->__file__      = {0};
  excep->__line__      = -1;
  excep->__function__  = {0};
}

static __inline__ void
continue_S_exception(exception_t *_this, exception_t *_prev)
{
  _this->_prev = _prev;
  _prev->_prev = ((_prev->_prev == NULL) ? NULL : /* YOU LEFT HERE */);
}

static __inline__ void
free_S_exception(exception_t *excep)
{
  free(excep);
}

static __inline__ void
_excep_output_excep_enum(excep_e excep_enum, const char *__restrict__ __file__,
                         long int __line__,
                         const char *__restrict__ __function__)
{
  fprintf(stderr, ((__file__ == NULL && __line__ == -1 && __function__ == NULL)
                   ? _DEF_EXCEP_FMT
                   /* Ignore _FMT when outputting the exception title,
                      use _EXCEP_FMT instead. */
                   : _EXCEP_FMT), __excep_etos(excep_enum),
                   __file__, __line__, __function__);
}

static __inline__ void
_excep_output_excep();

/* Example:
      Required Macros:                ___M1___  ___M2___  _____M3_____

      THROW(InstanceFailureException, __FILE__, __LINE__, __FUNCTION__,
                                      ~~~^^~~~  ~~~^^~~~  ~~~~^^^^~~~~
           "Errored when instancing %s.\nGiven options is illegal:\n\
           %d, %lf", opt1, opt2); */
static __inline__ void
THROW(excep_e excep_enum, const char *__restrict__ __file__, long int __line__,
      const char *__restrict__ __function__, const char *__restrict__ FMT, ...)
{
  /* Once FMT is not specified, we use _DEF_EXCEP_FMT */
  if (FMT == NULL)
    {
      fprintf(stderr, _DEF_EXCEP_FMT, __excep_etos(excep_enum));
      exit(EXIT_FAILURE);
    }

  /* Output secondary description about the thrown exception. */
  va_list _vlist;
  va_start(_vlist, FMT);
  _excep_output_excep_enum(excep_enum, __file__, __line__, __function__);
  vfprintf(stderr, FMT, _vlist);
  va_end(_vlist);

  exit(EXIT_FAILURE);
}

/* Supports recursive logging output through out every called method. :) */
static __inline__ void
THROWR(exception_t *excep, char *__restrict FMT, ...)
{
  /* Terminating condition */
  if (excep->_prev == excep->_this && excep->_this == NULL) {
    /* It probably needs memctl to release all allocated memory spaces. */
    _excep_output_excep_enum(excep->_excep_enum, excep->__file__, excep->__line__,
                             excep->__function__);
    exit(EXIT_FAILURE);
  }
  _excep_output_excep_enum(excep->_prev->_excep_enum,
                           excep->_prev->__file__,
                           excep->_prev->__line__,
                           excep->_prev->__function__);
}

#ifdef __cplusplus
__END_DECLS
#endif /* __cplusplus */

#endif /* NO EXCEPTION_H */
