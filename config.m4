dnl $Id$
dnl config.m4 for extension cpu_affinity

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(cpu_affinity, for cpu_affinity support,
Make sure that the comment is aligned:
[  --with-cpu_affinity             Include cpu_affinity support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(cpu_affinity, whether to enable cpu_affinity support,
Make sure that the comment is aligned:
[  --enable-cpu_affinity           Enable cpu_affinity support])

if test "$PHP_CPU_AFFINITY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-cpu_affinity -> check with-path
 SEARCH_PATH="/usr/local /usr"     # you might want to change this
 dnl SEARCH_FOR="/include/cpu_affinity.h"  # you most likely want to change this
 if test -r $PHP_CPU_AFFINITY/$SEARCH_FOR; then # path given as parameter
    CPU_AFFINITY_DIR=$PHP_CPU_AFFINITY
 else # search default path list
    AC_MSG_CHECKING([for cpu_affinity files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        CPU_AFFINITY_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
 
  if test -z "$CPU_AFFINITY_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the cpu_affinity distribution])
  fi

  dnl # --with-cpu_affinity -> add include path
  PHP_ADD_INCLUDE($CPU_AFFINITY_DIR/include)

  dnl # --with-cpu_affinity -> check for lib and symbol presence
  dnl LIBNAME=cpu_affinity # you may want to change this
  dnl LIBSYMBOL=cpu_affinity # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CPU_AFFINITY_DIR/$PHP_LIBDIR, CPU_AFFINITY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CPU_AFFINITYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong cpu_affinity lib version or lib not found])
  dnl ],[
  dnl   -L$CPU_AFFINITY_DIR/$PHP_LIBDIR -lm
  dnl ])
  
  PHP_SUBST(CPU_AFFINITY_SHARED_LIBADD)
  PHP_NEW_EXTENSION(cpu_affinity, cpu_affinity.c, $ext_shared)
fi
