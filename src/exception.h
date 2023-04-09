#ifndef EXCEPTION_H
#define EXCEPTION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>

/* par1="Exception"=_excep_e;
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

/* These exceptions would become a super class which is so-called
   "Abstract Class". */
static __inline__ const char *
__excep_etos(excep_t e)
{
  switch(e)
    {
      /* Once malloc returns NULL, this exception could stop the programme
      for good. */
      case InstanceFailureException:
        return "InstanceFailureException";
      /* When in "_var.h" and "_array.h" ..., it is common to have problems
         related to bounding restrictions. Therefor, to protect a programme
         by throwing this exception would be a better solution than just
         having a "Segmentation Fault (Core dumped)" in the end of the day. */
      case IllegalMemoryAccessException:
        return "IllegalMemoryAccessException";
      /* When passing through a function with given parameters, it is critical
         to check whether they are qualified for the function to take in.
         For example, if function "void f(int a)" requires parameter "a"
         NOT to be negative, then we need to throw this exception for safety.
         This exception is called on first once InvalidNullPointerException
         involves.
      */
      case InvalidArgumentException:
        return "InvalidArgumentException";
      /* When in an array, a pointer must NOT go out of the scheduled
         bound that limits the actual physical sizes on the memory.
         This exception is kind of similarity of IllegalMemoryAccessException.
         However, when using "_var.h" and "_array.h", there would be more
         complex situations that puts variables into a conception of
         generalisation for different size for one single variable.
         This requires non-system-builtin but systematic check mechanics to
         kick in. */
      case OutOfBoundException:
        return "OutOfBoundException";
      /* In some particular cases, you would meet on some scenarios
         that restricts the target to NOT be nulled. */
      case InvalidNullPointerException:
        return "InvalidNullPointerException";
      /* When allocating with allocating functions, usually those who returns
      NULL means failure of allocations. Out of memory specifically is the major
      caution. And we use this exception to identify them. */
      case OutOfMemoryException:
        return "OutOfMemoryException";
      /* To be able to throw this exception, you must have typo in
         the name of targeting exception, or simply you just passed an
         unknown exception into this function. Therefor, it is no longer
         the responsibility for this function to "guess" which exception
         you were meant to throw. And UnknownException it is to be thrown. */
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
