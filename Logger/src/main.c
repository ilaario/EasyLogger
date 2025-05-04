#include <logger.h>
// #include "../lib/include/logger.h"

int main(int argc, char const *argv[]) {
    if (argc < 1) {
        perror("!!! INVALID ARGUMENT, MISSING <fp> !!!");
        return -1;
    }

    int rc = 0;

    rc = log_init(argv[1]);
    if(rc != 0) {
        perror("!!! ERROR INIT LOG !!!");
        return -1;
    }

    int num = 7;

    rc = logx(DEBUG, "Questa è una prova di LOGX, ecco un numero %d", num);
    if(rc != 0) {
        perror("!!! ERROR LOGX !!!");
        return -1;
    }

    rc = syserr_log("Questa è una prova di SYSERR_LOG, ecco un numero %d", num);
    if(rc != 0) {
        perror("!!! ERROR SYSERR_LOG !!!");
        return -1;
    }

    rc = sysinfo_log("Questa è una prova di SYSINFO_LOG, ecco un numero %d", num);
    if(rc != 0) {
        perror("!!! ERROR SYSINFO_LOG !!!");
        return -1;
    }

    rc = apperr_log("Questa è una prova di APPERROR_LOG, ecco un numero %d", num);
    if(rc != 0) {
        perror("!!! ERROR APPERROR_LOG !!!");
        return -1;
    }

    rc = appinfo_log("Questa è una prova di APPINFO_LOG, ecco un numero %d", num);
    if(rc != 0) {
        perror("!!! ERROR APPINFO_LOG !!!");
        return -1;
    }

    rc = logx(DEBUG, "Riga 1: %d\n"
                     "Riga 2: %d\n"
                     "Riga 3: %d",
                      num, num, num);
    if(rc != 0) {
        perror("!!! ERROR LOGX !!!");
        return -1;
    }

    rc = log_stop();
    if(rc != 0){
        perror("!!! ERROR LOG_STOP !!!");
        return -1;
    }

    return 0;
}
