dnl Autoconf script for tcsh
dnl To rebuild the `configure' script from this, execute the command
dnl 	autoconf
dnl in the directory containing this script.
dnl
dnl You'll also need a version of config.guess from a gnu package
dnl
dnl Written by Kaveh Ghazi (ghazi@caip.rutgers.edu) 5/11/96.
dnl
dnl $tcsh$

AC_PREREQ([2.59])dnl Minimum Autoconf version required.
AC_INIT([tcsh], [6.20.00], [http://bugs.gw.com/])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([tc.vers.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_TESTDIR([.], [.])

AC_PROG_INSTALL
AC_CANONICAL_HOST

AC_MSG_CHECKING([cached host tuple])
if { test x"${ac_cv_host_system_type+set}" = x"set" &&
     test x"$ac_cv_host_system_type" != x"$host"; }; then
  AC_MSG_RESULT([different])
  AC_MSG_ERROR([remove config.cache and re-run configure])
else
  AC_MSG_RESULT(ok)
fi
ac_cv_host_system_type="$host"


changequote(, )dnl

case "${host}" in

  ## Alpha (DEC) machines.
  alpha*-dec-osf* )
    tcsh_config_file=decosf1
  ;;

  ## Ultrix
  *-dec-ultrix* )
    tcsh_config_file=ultrix
  ;;

  ## DGUX
  *-dg-dguxR4* )
    tcsh_config_file=dgux5.4
  ;;
  m88k-dg-dgux5.4R* )
    tcsh_config_file=dgux5.4
  ;;

  ## HP/UX
  *-hp-hpux7* )
    tcsh_config_file=hpux7
  ;;
  *-hp-hpux[89]* )
    tcsh_config_file=hpux8
  ;;
  *-hp-hpux1[0-9]* )
    tcsh_config_file=hpux11
  ;;

  ## IBM AIX systems
  *-ibm-aix*)
    tcsh_config_file=aix
  ;;

  ## SX-4
  sx4-nec-*)
    CC='cc -h0,ansi,novector,float0'
    LDFLAGS='-Gsmall'
    tcsh_config_file=superux8
  ;;

  ## IBM OS/390 systems
  *-ibm-os390*)
    CC='c89'
    tcsh_config_file=os390
  ;;

  ## Android
  *-*-android*)
    tcsh_config_file=android
  ;;

  ## Linux
  *-*-linux* | *-*-gnu* | *-*-k*bsd*-gnu)
    tcsh_config_file=linux
  ;;

  ## Motorola systems
  m68k-motorola-sysv* )
    tcsh_config_file=sysV68
  ;;
  m88k-motorola-sysv3* )
    tcsh_config_file=sysV88
  ;;

  ## Minix systems
  *-*-minix*)
    tcsh_config_file=minix
  ;;

  ## NetBSD systems
  *-*-netbsd*)
    tcsh_config_file=bsd4.4
  ;;

  ## FreeBSD systems
  *-*-freebsd*)
    tcsh_config_file=bsd4.4
  ;;

  ## MidnightBSD systems
  *-*-midnightbsd*)
    tcsh_config_file=bsd4.4
  ;;
  
  ## DragonFlyBSD systems
  *-*-dragonfly*)
    tcsh_config_file=bsd4.4
  ;;

  ## MirBSD systems
  *-*-mirbsd*)
    tcsh_config_file=bsd4.4
  ;;

  ## OpenBSD systems
  *-*-openbsd*)
    tcsh_config_file=bsd4.4
  ;;

  ## BSDI systems
  *-*-bsdi*)
    tcsh_config_file=bsd4.4
  ;;

  ## Mac OS X Server
  *-*-rhapsody* )
    tcsh_config_file=bsd4.4
  ;;

  ## Mac OS X Server
  *-*-darwin* )
    tcsh_config_file=bsd4.4
  ;;

  ## DragonFly systems
  *-*-dragonfly*)
    tcsh_config_file=bsd4.4
  ;;

  ## Silicon Graphics machines
  *-sgi-iri* )
    tcsh_config_file=irix
    case "${host_os}" in
      irix[34]*) # Irix-3.x - Irix 4.x
        NON_GNU_DFLAGS='-D__STDC__'
        LIBS='-lsun -lbsd -lc_s'
      ;;
      irix5* | irix6.[01]*) # Irix 5.x, Irix 6.0 - 6.1
        LIBS='-lbsd'
      ;;
      irix6.[2-9]*) # Irix 6.2 and later
	tcsh_config_file=irix62
      ;;
    esac
  ;;

  ## Suns
  *-sun-sunos3* )
    tcsh_config_file=sunos35
  ;;
  *-sun-sunos4.0* )
    tcsh_config_file=sunos40
  ;;
  *-sun-sunos4.1.[12]* )
    tcsh_config_file=sunos41
  ;;
  *-sun-sunos4.1.[3456]* )
    tcsh_config_file=sunos413
  ;;
  *-*-solaris2.[01] ) # Should handle sparc or x86
    tcsh_config_file=sol2
  ;;
  *-sun-solaris2.2 ) # Sparc only release
    tcsh_config_file=sol22
  ;;
  *-sun-solaris2.3 ) # Sparc only release
    tcsh_config_file=sol23
  ;;
  *-*-solaris2.[45] ) # Should handle sparc, x86 and powerpc
    tcsh_config_file=sol24
  ;;
  *-*-solaris2.[678] ) # Should handle sparc, x86 and powerpc
    tcsh_config_file=sol26
  ;;
  *-*-solaris2.* ) # Should handle sparc, x86 and powerpc
    tcsh_config_file=sol29
  ;;

  ## Dell SVR4
  *-dell-sysv4* )
    tcsh_config_file=sysv4
    DFLAGS="$DFLAGS -DDELL"
  ;;

  ## SVR4 (Generic and otherwise)
  *-*-sysv4* )
    tcsh_config_file=sysv4
  ;;

  ## Tektronix systems
  m68k-tektronix-bsd* )
    tcsh_config_file=bsd
    NON_GNU_DFLAGS='-DBSD -DUTek'
  ;;
  m88k-tektronix-sysv3* )
    tcsh_config_file=tekXD88
  ;;

  ## SCO
  *-sco3.2v5*) 
    tcsh_config_file=sco32v5
  ;;
  *-sco3.2v4*)
    tcsh_config_file=sco32v4
  ;;
  *-sco3.2v2*)
    tcsh_config_file=sco32v2
  ;;

  ## Siemens BS2000 Mainframe
  bs2000-siemens-*)
    tcsh_config_file=bs2000
  ;;

  ## Convex
  *-convex*)
    tcsh_config_file=convex
  ;;

  ## Cray
  *-cray-unicos*)
    tcsh_config_file=cray
  ;;

  ## Red Hat Cygwin
  *-cygwin)
    tcsh_config_file=cygwin
  ;;

  ## mips-compaq-nonstopux
  *-*-nonstopux)
    tcsh_config_file=sysv4
  ;;

  ## Apollo Domain/OS
  *-apollo-*)
    tcsh_config_file=apollo
  ;;

  ## QNX6
  *-qnx6*)
    tcsh_config_file=qnx6
  ;;

  * )
  changequote([, ])dnl
  AC_MSG_ERROR([Tcsh can't guess the configuration file name
for `${host}' systems.
Check tcsh's `Ported' file for manual configuration instructions.])
  changequote(, )dnl
  ;;

esac

echo "Tcsh will use configuration file \`$tcsh_config_file'."
changequote([, ])dnl
cp ${srcdir}/config/${tcsh_config_file} config_p.h
AH_BOTTOM(
[#include "config_p.h"
#include "config_f.h"])

dnl Checks for programs
AC_PROG_CC
AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL

AC_PATH_PROG([GENCAT], [gencat]) 

dnl Require build CC to create gethost helper when cross building
if test "x${cross_compiling}" = xyes ; then
  CC_FOR_GETHOST="cc"
else
  CC_FOR_GETHOST="\$(CC)"
fi
AC_SUBST(CC_FOR_GETHOST)

if test "x$GCC" != xyes ; then
  DFLAGS="$DFLAGS $NON_GNU_DFLAGS"
  CFLAGS="$CFLAGS $NON_GNU_CFLAGS"
fi

dnl More recent Android requires PIEs
case "${host}" in
  *-*-android*)
    CFLAGS="${CFLAGS} -fPIE"
    LDFLAGS="${LDFLAGS} -pie"
    ;;
esac

dnl Checks for libraries
AC_SEARCH_LIBS(crypt, crypt)
AC_SEARCH_LIBS(getspnam, sec)
AC_SEARCH_LIBS([tgetent], [termlib termcap curses ncurses], [], [
   AC_MSG_ERROR([unable to find the tgetent() function])
])
AC_SEARCH_LIBS(gethostbyname, nsl)
AC_SEARCH_LIBS(connect, socket)
AC_SEARCH_LIBS(catgets, catgets)
AM_ICONV

dnl Checks for header files
AC_CHECK_HEADERS([auth.h crypt.h features.h inttypes.h paths.h] dnl
		 [shadow.h stdint.h utmp.h utmpx.h])
AC_CHECK_HEADERS([wchar.h],
	[AC_CHECK_SIZEOF([wchar_t], [], [dnl
#include <stdio.h>
#include <wchar.h>])
	AC_CHECK_HEADERS([wctype.h])])
AC_HEADER_DIRENT
AC_HEADER_STAT
AC_HEADER_STDC

dnl Checks for types
AC_CHECK_TYPES([long long])
AC_TYPE_GETGROUPS
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT32_T

AC_DEFUN([AC_TYPE_SSIZE_T], [
AC_CHECK_TYPE(ssize_t,,
AC_DEFINE(ssize_t, int, [Define to `int' not defined in <sys/types.h>.]),
[
#include <sys/types.h>
])
])
AC_TYPE_SSIZE_T

AC_DEFUN([AC_TYPE_SOCKLEN_T], [
AC_CHECK_TYPE(socklen_t,,
AC_DEFINE(socklen_t, int, [Define to `int' if neither <sys/types.h> nor <sys/socket.h> define.]),
[
#include <sys/types.h>
#include <sys/socket.h>
])
])
AC_TYPE_SOCKLEN_T


dnl Checks for structures
AC_CHECK_MEMBERS([struct dirent.d_ino], , ,
[#ifdef HAVE_DIRENT_H
# include <dirent.h>
#else
# ifdef HAVE_NDIR_H
#  include <ndir.h>
# else
#  include <sys/dir.h>
# endif
# define dirent direct
#endif])
AC_CHECK_MEMBERS([struct utmp.ut_host, struct utmp.ut_user, struct utmp.ut_tv,
		  struct utmp.ut_xtime, struct utmpx.ut_host,
		  struct utmpx.ut_user, struct utmpx.ut_tv,
		  struct utmpx.ut_xtime], , ,
[#include <sys/types.h>
#ifdef HAVE_UTMPX_H
#include <utmpx.h>
#define utmp utmpx
#elif defined HAVE_UTMP_H
#include <utmp.h>
#endif])
AC_CHECK_MEMBERS([struct sockaddr_storage.ss_family], , ,
[#include <sys/types.h>
#include <sys/socket.h>])

dnl Checks for compiler characteristics
AC_C_CONST
AC_C_VOLATILE

dnl checks for library functions
AC_CHECK_DECLS([crypt, environ, gethostname, getpgrp], , ,
[#include "config_p.h"
AC_INCLUDES_DEFAULT([])
#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif
])
AC_CHECK_FUNC([setlocale], [have_setlocale=yes], [have_setlocale=no])
AC_CHECK_FUNC([catgets], [have_catgets=yes], [have_catgets=no])
AC_CHECK_FUNCS([dup2 getauthid getcwd gethostname getpwent] dnl
	[getutent getutxent mallinfo mblen memmove memset mkstemp nice] dnl
	[nl_langinfo sbrk setpgid setpriority strerror strstr sysconf wcwidth])
AC_FUNC_GETPGRP
AC_FUNC_MBRTOWC
if test "x${cross_compiling}" != xyes ; then
  AC_FUNC_SETPGRP
else
  ac_cv_func_setpgrp_void=yes
fi
AC_FUNC_STRCOLL

dnl This is not good enough; we need sockaddr_storage too.
dnl See whether we can use IPv6 related functions
dnl AC_DEFUN([IPv6_CHECK_FUNC], [
dnl changequote(, )dnl
dnl ac_tr_lib=HAVE_`echo $1 | sed -e 's/[^a-zA-Z0-9_]/_/g' \
dnl   -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
dnl changequote([, ])dnl
dnl AC_CHECK_FUNC($1, [dnl
dnl   AC_DEFINE_UNQUOTED($ac_tr_lib)
dnl   ac_cv_lib_inet6_$1=no], [dnl
dnl   AC_MSG_CHECKING([whether your system has IPv6 directory])
dnl   AC_CACHE_VAL(ipv6_cv_dir, [dnl
dnl     for ipv6_cv_dir in /usr/local/v6 /usr/inet6 no; do
dnl       if test $ipv6_cv_dir = no -o -d $ipv6_cv_dir; then
dnl 	break
dnl       fi
dnl     done])dnl
dnl   AC_MSG_RESULT($ipv6_cv_dir)
dnl   if test $ipv6_cv_dir = no; then
dnl     ac_cv_lib_inet6_$1=no
dnl   else
dnl     if test x$ipv6_libinet6 = x; then
dnl       ipv6_libinet6=no
dnl       SAVELDFLAGS="$LDFLAGS"
dnl       LDFLAGS="-L$ipv6_cv_dir/lib"
dnl     fi
dnl     AC_CHECK_LIB(inet6, $1, [dnl
dnl       AC_DEFINE_UNQUOTED($ac_tr_lib)
dnl       if test $ipv6_libinet6 = no; then
dnl 	ipv6_libinet6=yes
dnl 	LIBS="$LIBS -linet6"
dnl       fi],)dnl
dnl     if test $ipv6_libinet6 = no; then
dnl       LDFLAGS="$SAVELDFLAGS"
dnl     fi
dnl   fi])dnl
dnl if test $ac_cv_func_$1 = yes -o $ac_cv_lib_inet6_$1 = yes
dnl then
dnl   ipv6_cv_$1=yes
dnl   ifelse([$2], , :, [$2])
dnl else
dnl   ipv6_cv_$1=no
dnl   ifelse([$3], , :, [$3])
dnl fi])
dnl IPv6_CHECK_FUNC(getnameinfo, DFLAGS="$DFLAGS -DINET6")

AC_SUBST(DFLAGS)

dnl Checks for system services
if test "$have_setlocale" != no; then
  AC_ARG_ENABLE([nls], AS_HELP_STRING([--disable-nls], [Disable NLS support]),
	        [], [enable_nls=yes])
  if test "x$enable_nls" != xno; then
    AC_DEFINE([NLS], [1], [Support NLS.])
  fi
fi

if test "x$enable_nls" != xno -a "$have_catgets" != no -a -n "$GENCAT" ; then
  AC_ARG_ENABLE([nls-catalogs], AS_HELP_STRING([--disable-nls-catalogs], [Disable NLS catalog support]),
	        [], [enable_nls_catalogs=yes])
  if test "x$enable_nls_catalogs" != xno; then
    BUILD_CATALOGS="yes"
    AC_DEFINE([NLS_CATALOGS], [1], [Support NLS catalogs.])
  fi
fi

AC_ARG_WITH(hesiod,
  [  --with-hesiod=PREFIX    Use Hesiod lookup for ~ expansion],
  [hesiod="$withval"], [hesiod=no])
if test "$hesiod" != no; then
  HESLIB="-lhesiod"
  AC_CHECK_FUNC(res_send, :,
    AC_CHECK_LIB(resolv, res_send, HESLIB="$HESLIB -lresolv"))
  HESDEF=-DHESIOD
  if test "$hesiod" != yes; then
    HESDEF="$HESDEF -I$hesiod/include"
    HESLIB="-L$hesiod/lib $HESLIB"
  fi
fi
AC_SUBST(HESDEF)
AC_SUBST(HESLIB)
AC_SUBST(BUILD_CATALOGS)

AC_CONFIG_FILES([Makefile nls/Makefile])
AC_OUTPUT
