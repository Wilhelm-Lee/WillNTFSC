/*
*     This file is part of project <https://github.com/WilliamPascal/JackNTFS>
*     Copyright (C) 2023  William Pascal
*
*     This program is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     This program is distributed in the hope that it will be useful,
*     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef EXCEPTION_H
#define EXCEPTION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>

/* par1="Exception"=excep_t;
   par2="File"=__FILE__;
   par3="Line"=__LINE__;
   par4="Function"=__FUNCTION__ */
static const char *__restrict__ _EXCEP_FMT =
"Threw the %s:\n\tat %s:%ld, func %s\n";
static const char *__restrict__ _DEF_EXCEP_FMT = "Threw the %s\n";

typedef enum E_excep
{
  /* JackNTFS Exceptions */
  PermissionDeniedException        = 0b00000000,
  ReadOperationFailedException     = 0b00000001,
  WriteOperationFailedException    = 0b00000010,
  /* 0b0000 0011 */
  ExecuteOperationFailedException  = 0b00000100,
  /* 0b0000 0101
     0b0000 0110 */
  OperationTimedOutException       = 0b00000111,

  /* COOL_MinGW Exceptions */
  InstanceFailureException         = 0b10000000,
  IllegalMemoryAccessException,
  InvalidArgumentException,
  OutOfBoundException,
  InvalidNullPointerException,
  OutOfMemoryException,

  /* Shared Exceptions */
  NoSuchElementException,
} excep_t;

static __inline__ const char *
__excep_etos(excep_t e)
{
  switch(e)
    {
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
      default:
        return "UnknownException";
    }
}

/* Example:
      Required Macros:                ___M1___  ___M2___  _____M3_____

      THROW(InstanceFailureException, __FILE__, __LINE__, __FUNCTION__,
                                      ~~~^^~~~  ~~~^^~~~  ~~~~^^^^~~~~
           "Errored when instancing %s.\nGiven options is illegal:\n\
           %d, %lf", opt1, opt2); */
static __inline__ void
THROW(excep_t e, const char *__restrict__ __file__, long int __line__,
      const char *__restrict__ __function__, const char *__restrict__ _FMT, ...)
{
  if (_FMT == NULL)
    {
      fprintf(stderr, _DEF_EXCEP_FMT, __excep_etos(e));
      exit(EXIT_FAILURE);
    }

  /* Output secondary description about the thrown exception. */
  va_list _vlist;
  va_start(_vlist, _FMT);
  fprintf(stderr, ((__file__ == NULL && __line__ == -1 && __function__ == NULL)
                   ? _DEF_EXCEP_FMT
                   /* Ignore _FMT when outputting the exception title,
                      use _EXCEP_FMT instead. */
                   : _EXCEP_FMT), __excep_etos(e),
                   __file__, __line__, __function__);
  vfprintf(stderr, _FMT, _vlist);
  va_end(_vlist);

  exit(EXIT_FAILURE);
}

#endif /* NO EXCEPTION_H */
