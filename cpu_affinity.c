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
  | Author:niansong                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_cpu_affinity.h"

//ZEND_DECLARE_MODULE_GLOBALS(cpu_affinity)

/* True global resources - no need for thread safety here */
static int le_cpu_affinity;

/* {{{ cpu_affinity_functions[]
 *
 * Every user visible function must have an entry in cpu_affinity_functions[].
 */
const zend_function_entry cpu_affinity_functions[] = {
	PHP_FE(get_cpu_mask, NULL)
	PHP_FE(set_cpu_mask, NULL)
	PHP_FE_END
};
/* }}} */

/* {{{ cpu_affinity_module_entry
 */
zend_module_entry cpu_affinity_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"cpu_affinity",
	cpu_affinity_functions,
	PHP_MINIT(cpu_affinity),
	PHP_MSHUTDOWN(cpu_affinity),
	PHP_RINIT(cpu_affinity),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(cpu_affinity),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(cpu_affinity),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_CPU_AFFINITY_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CPU_AFFINITY
ZEND_GET_MODULE(cpu_affinity)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("cpu_affinity.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_cpu_affinity_globals, cpu_affinity_globals)
    STD_PHP_INI_ENTRY("cpu_affinity.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_cpu_affinity_globals, cpu_affinity_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_cpu_affinity_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_cpu_affinity_init_globals(zend_cpu_affinity_globals *cpu_affinity_globals)
{
	cpu_affinity_globals->global_value = 0;
	cpu_affinity_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(cpu_affinity)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(cpu_affinity)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(cpu_affinity)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(cpu_affinity)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(cpu_affinity)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "cpu_affinity support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/*this is my function*/
static inline int print_cpu_mask(cpu_set_t cpu_mask)
{
    unsigned char flag = 0;
    unsigned int i = 0;
    for(; i < sizeof(cpu_set_t); i++){
        if(CPU_ISSET(i, &cpu_mask)){
	    if(flag == 0){
	        flag = 1;
		return i;
	    } else {
		return i;
	    }
	}
    }
}

static inline void get_cpu_mask(pid_t pid, cpu_set_t *mask)
{
    if(sched_getaffinity(pid, sizeof(cpu_set_t), mask) == -1){
	//abort();
	return;
    }
}

static inline void set_cpu_mask(pid_t pid, cpu_set_t *mask)
{
    if(sched_setaffinity(pid, sizeof(cpu_set_t), mask) == -1){
        //abort();
	return;
    }
}

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_cpu_affinity_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(get_cpu_mask)
{
    cpu_set_t cpu_mask;
    int ret;

    get_cpu_mask(0, &cpu_mask);
    ret =  print_cpu_mask(cpu_mask);
    RETURN_LONG(ret);
}

PHP_FUNCTION(set_cpu_mask)
{
    int argc = ZEND_NUM_ARGS();
    unsigned int active_cpu;
    int arg_len;
    cpu_set_t cpu_mask;
    int cpu_num;    

    if(zend_parse_parameters(argc TSRMLS_CC, "l", &active_cpu, &arg_len) == FAILURE){
        return;
    }
    
    cpu_num = sysconf(_SC_NPROCESSORS_CONF);

    if(active_cpu > cpu_num){
        RETURN_FALSE;
    }

    CPU_ZERO(&cpu_mask);
    CPU_SET(active_cpu, &cpu_mask);
    set_cpu_mask(0, &cpu_mask);
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
