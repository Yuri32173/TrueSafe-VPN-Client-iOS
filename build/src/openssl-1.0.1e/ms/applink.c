#include <stdio.h>
#include <io.h>
#include <fcntl.h>

void *app_stdin(void) { return stdin; }
void *app_stdout(void) { return stdout; }
void *app_stderr(void) { return stderr; }
int app_feof(FILE *fp) { return feof(fp); }
int app_ferror(FILE *fp) { return ferror(fp); }
void app_clearerr(FILE *fp) { clearerr(fp); }
int app_fileno(FILE *fp) { return _fileno(fp); }
int app_fsetmod(FILE *fp, char mod) { return _setmode(_fileno(fp), mod == 'b' ? _O_BINARY : _O_TEXT); }

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport)
void **OPENSSL_Applink(void)
{
    static int once = 1;
    static void *OPENSSL_ApplinkTable[APPLINK_MAX + 1] = { (void *)APPLINK_MAX };

    if (once)
    {
        OPENSSL_ApplinkTable[APPLINK_STDIN] = app_stdin;
        OPENSSL_ApplinkTable[APPLINK_STDOUT] = app_stdout;
        OPENSSL_ApplinkTable[APPLINK_STDERR] = app_stderr;
        OPENSSL_ApplinkTable[APPLINK_FPRINTF] = fprintf;
        OPENSSL_ApplinkTable[APPLINK_FGETS] = fgets;
        OPENSSL_ApplinkTable[APPLINK_FREAD] = fread;
        OPENSSL_ApplinkTable[APPLINK_FWRITE] = fwrite;
        OPENSSL_ApplinkTable[APPLINK_FSETMOD] = app_fsetmod;
        OPENSSL_ApplinkTable[APPLINK_FEOF] = app_feof;
        OPENSSL_ApplinkTable[APPLINK_FCLOSE] = fclose;

        OPENSSL_ApplinkTable[APPLINK_FOPEN] = fopen;
        OPENSSL_ApplinkTable[APPLINK_FSEEK] = fseek;
        OPENSSL_ApplinkTable[APPLINK_FTELL] = ftell;
        OPENSSL_ApplinkTable[APPLINK_FFLUSH] = fflush;
        OPENSSL_ApplinkTable[APPLINK_FERROR] = app_ferror;
        OPENSSL_ApplinkTable[APPLINK_CLEARERR] = app_clearerr;
        OPENSSL_ApplinkTable[APPLINK_FILENO] = app_fileno;

        OPENSSL_ApplinkTable[APPLINK_OPEN] = _open;
        OPENSSL_ApplinkTable[APPLINK_READ] = _read;
        OPENSSL_ApplinkTable[APPLINK_WRITE] = _write;
        OPENSSL_ApplinkTable[APPLINK_LSEEK] = _lseek;
        OPENSSL_ApplinkTable[APPLINK_CLOSE] = _close;

        once = 0;
    }

    return OPENSSL_ApplinkTable;
}

#ifdef __cplusplus
}
#endif
