#include <stdio.h>
#include <openssl/err.h>
#include "e_atalla_err.h"

#ifndef OPENSSL_NO_ERR

#define ERR_FUNC(func) ERR_PACK(0, func, 0)
#define ERR_REASON(reason) ERR_PACK(0, 0, reason)

static ERR_STRING_DATA ATALLA_str_functs[] = {
    {ERR_FUNC(ATALLA_F_ATALLA_CTRL), "ATALLA_CTRL"},
    {ERR_FUNC(ATALLA_F_ATALLA_FINISH), "ATALLA_FINISH"},
    {ERR_FUNC(ATALLA_F_ATALLA_INIT), "ATALLA_INIT"},
    {ERR_FUNC(ATALLA_F_ATALLA_MOD_EXP), "ATALLA_MOD_EXP"},
    {ERR_FUNC(ATALLA_F_ATALLA_RSA_MOD_EXP), "ATALLA_RSA_MOD_EXP"},
    {0, NULL}
};

static ERR_STRING_DATA ATALLA_str_reasons[] = {
    {ERR_REASON(ATALLA_R_ALREADY_LOADED), "already loaded"},
    {ERR_REASON(ATALLA_R_BN_CTX_FULL), "bn ctx full"},
    {ERR_REASON(ATALLA_R_BN_EXPAND_FAIL), "bn expand fail"},
    {ERR_REASON(ATALLA_R_CTRL_COMMAND_NOT_IMPLEMENTED), "ctrl command not implemented"},
    {ERR_REASON(ATALLA_R_MISSING_KEY_COMPONENTS), "missing key components"},
    {ERR_REASON(ATALLA_R_NOT_LOADED), "not loaded"},
    {ERR_REASON(ATALLA_R_REQUEST_FAILED), "request failed"},
    {ERR_REASON(ATALLA_R_UNIT_FAILURE), "unit failure"},
    {0, NULL}
};

#endif

#ifdef ATALLA_LIB_NAME
static ERR_STRING_DATA ATALLA_lib_name[] = {
    {0, ATALLA_LIB_NAME},
    {0, NULL}
};
#endif

static int ATALLA_lib_error_code = 0;
static int ATALLA_error_init = 1;

static void ERR_load_ATALLA_strings(void)
{
    if (ATALLA_lib_error_code == 0)
        ATALLA_lib_error_code = ERR_get_next_error_library();

    if (ATALLA_error_init) {
        ATALLA_error_init = 0;
#ifndef OPENSSL_NO_ERR
        ERR_load_strings(ATALLA_lib_error_code, ATALLA_str_functs);
        ERR_load_strings(ATALLA_lib_error_code, ATALLA_str_reasons);
#endif

#ifdef ATALLA_LIB_NAME
        ATALLA_lib_name->error = ERR_PACK(ATALLA_lib_error_code, 0, 0);
        ERR_load_strings(0, ATALLA_lib_name);
#endif
    }
}

static void ERR_unload_ATALLA_strings(void)
{
    if (ATALLA_error_init == 0) {
#ifndef OPENSSL_NO_ERR
        ERR_unload_strings(ATALLA_lib_error_code, ATALLA_str_functs);
        ERR_unload_strings(ATALLA_lib_error_code, ATALLA_str_reasons);
#endif

#ifdef ATALLA_LIB_NAME
        ERR_unload_strings(0, ATALLA_lib_name);
#endif
        ATALLA_error_init = 1;
    }
}

static void ERR_ATALLA_error(int function, int reason, char *file, int line)
{
    if (ATALLA_lib_error_code == 0)
        ATALLA_lib_error_code = ERR_get_next_error_library();
    ERR_PUT_error(ATALLA_lib_error_code, function, reason, file, line);
}
