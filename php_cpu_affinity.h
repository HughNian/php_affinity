/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: niansong                                                     |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_CPU_AFFINITY_H
#define PHP_CPU_AFFINITY_H

extern zend_module_entry cpu_affinity_module_entry;
#define phpext_cpu_affinity_ptr &cpu_affinity_module_entry

#define PHP_CPU_AFFINITY_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_CPU_AFFINITY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_CPU_AFFINITY_API __attribute__ ((visibility("default")))
#else
#	define PHP_CPU_AFFINITY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif
#include <stdint.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <stdlib.h>

PHP_MINIT_FUNCTION(cpu_affinity);
PHP_MSHUTDOWN_FUNCTION(cpu_affinity);
PHP_RINIT_FUNCTION(cpu_affinity);
PHP_RSHUTDOWN_FUNCTION(cpu_affinity);
PHP_MINFO_FUNCTION(cpu_affinity);

PHP_FUNCTION(get_cpu_mask);
PHP_FUNCTION(set_cpu_mask);

/*
ZEND_BEGIN_MODULE_GLOBALS(cpu_affinity)
    //long  global_value;
    cpu_set_t cpu_mask;
ZEND_END_MODULE_GLOBALS(cpu_affinity)
*/

/* In every utility function you add that needs to use variables 
   in php_cpu_affinity_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as CPU_AFFINITY_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define CPU_AFFINITY_G(v) TSRMG(cpu_affinity_globals_id, zend_cpu_affinity_globals *, v)
#else
#define CPU_AFFINITY_G(v) (cpu_affinity_globals.v)
#endif

#endif	/* PHP_CPU_AFFINITY_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
