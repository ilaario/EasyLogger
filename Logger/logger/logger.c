#include "../lib/cache.h"
#include "../lib/logger.h"
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>

FILE* fp;

int write_on_system_log(const char* buffer, char* program_name) {
    int rc = 0;
    if(program_name == NULL) {
        return EINVAL;
    }

#ifdef LINUX_
    openlog(program_name, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    // syslog(LOG_MAKEPRI(LOG_LOCAL1, LOG_NOTICE), "Program started by User %d", getuid());
    syslog(LOG_INFO, buffer);
    closelog();

    return rc;
#endif

    return rc;
}

/***
 * Return the log level
 * @param level log_level: The requested log_level in the enum
 * @return The requested log_level as a string
 */
char *get_log_level(enum log_level level) {
    switch (level) {
        case DEBUG:
            return "DEBUG";
        case TRACE:
            return "TRACE";
        case APPERR:
            return "APPER";
        case SYSERR:
            return "SYSER";
        case SYSINFO:
            return "SYSIN";
        case APPINFO:
            return "APPIN";
        default:
            fprintf(stderr, "!!! INVALID LOG LEVEL !!!\n");
            return NULL;
    }
}

/***
 * Write a buffer inside a specific FILE
 * @param buffer char[256]: The requested buffer
 * @param level log_level: The log_level requested
 * @return Return 0 if OK, else if error
 */
int write_buffer(const char* buffer, log_level level) {
    char *funct_name = "write_buffer";

    if (fp == NULL) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: NULL File pointer with errno: %d\n", __FILE__, __LINE__, funct_name, errno);
        return errno;
    }

    int rc = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL); // Ottieni il tempo corrente con precisione fino ai microsecondi

    time_t time_now = tv.tv_sec;
    struct tm *tm_info = localtime(&time_now);

    char time_stamp[64];
    strftime(time_stamp, sizeof(time_stamp), "%d/%m/%Y - %H:%M.%S", tm_info); // Formatta il tempo

    //generate the string with the full current time 
    snprintf(time_stamp + strlen(time_stamp), sizeof(time_stamp) - strlen(time_stamp), ".%03ld", tv.tv_usec / 1000); // Aggiungi i millisecondi

    rc = fprintf(fp, "[%s]\t- [PID: %d]\t- [%s]\t--> [%s] \n", get_log_level(level), getpid(), time_stamp, buffer);
    if(rc <= 0) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed writing on fp with rc: %d\n", __FILE__, __LINE__, funct_name, rc);
        return rc;
    }

    rc = write_on_system_log(buffer, "logger");
    if (rc != 0)
    {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed writing on system log with rc: %d..\n", __FILE__, __LINE__, funct_name, rc);
        return rc;
    }

    fflush(fp);
    return rc;
}

/***
 * Initialise the log, opening the file
 * @param filePath const char*: Path to the file we want to write on
 * @return Return 0 if OK, else if error
 */
int log_init(const char* filePath){
    char *funct_name = "log_init";
    if(filePath == NULL) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: File Path can't be NULL\n", __FILE__, __LINE__, funct_name);
        return EINVAL;
    }

    fp = fopen(filePath, "a");
    if (fp == NULL) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed opening FP\n", __FILE__, __LINE__, funct_name);
        return EBADF;
    }

    write_buffer("LOG STARTED", TRACE);

    return 0;
}

/***
 * Interrupt the log, closing the file pointer
 * @return Return 0 if OK, else if error
 */
int log_stop(){
    write_buffer("EXITING", TRACE);
    return fclose(fp);
}

/***
 * Log general function
 * @param log log_level: The requested log level
 * @param buffer char[256]: Buffer
 * @return Return 0 if OK, else if error
 */
int logx(log_level log, const char* buffer){
    char *funct_name = "logx";
    int rc = write_buffer(buffer, log);
    if(rc != 0) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed writing log with rc: %d\n", __FILE__, __LINE__, funct_name, rc);
        return rc;
    }
    return rc;
}

/***
 * Log in SYSERROR level
 * @param buffer char[256]: Buffer
 * @return Return 0 if OK, else if error
 */
int syserr_log(const char* buffer){
    char *funct_name = "syserr_log";
    int rc = write_buffer(buffer, SYSERR);
    if(rc != 0) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed writing log with rc: %d\n", __FILE__, __LINE__, funct_name, rc);
        return rc;
    }
    return rc;
}

/***
 * Log in SYSINFO level
 * @param buffer char[256]: Buffer
 * @return Return 0 if OK, else if error
 */
int sysinfo_log(const char* buffer){
    char *funct_name = "sysinfo_log";
    int rc = write_buffer(buffer, SYSINFO);
    if(rc != 0) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed writing log with rc: %d\n", __FILE__, __LINE__, funct_name, rc);
        return rc;
    }
    return rc;
}

/***
 * Log in APPERROR level
 * @param buffer char[256]: Buffer
 * @return Return 0 if OK, else if error
 */
int apperr_log(const char* buffer){
    char *funct_name = "apperr_log";
    int rc = write_buffer(buffer, APPERR);
    if(rc != 0) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed writing log with rc: %d\n", __FILE__, __LINE__, funct_name, rc);
        return rc;
    }
    return rc;
}

/***
 * Log in APPINFO level
 * @param buffer char[256]: Buffer
 * @return Return 0 if OK, else if error
 */
int appinfo_log(const char* buffer){
    char *funct_name = "appinfo_log";
    int rc = write_buffer(buffer, APPINFO);
    if(rc != 0) {
        fprintf(stderr, "%s %d - FUNCT: %s, Error: Failed writing log with rc: %d\n", __FILE__, __LINE__, funct_name, rc);
        return rc;
    }
    return rc;
}

int main(int argc, char const *argv[]) {

    /* Check the correct number of arguments passed
    if (argc < 2)
    {
        perror("!!! INVALID ARGUMENT, MISSING <fp> !!!");
        return -1;
    }

    fp = fopen(argv[1], "a");

    if (fp == NULL)
    {
        perror("!!! INVALID FILE POINTER !!!");
        return -1;
    }

    char buffer[256];
    int debug_value = 0;
    while (1)
    {
        printf("> ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) 
        {
            fprintf(stderr, "!!! INVALID INPUT !!!\n");
            fflush(stdin);
            strcpy(buffer, "!!! INVALID INPUT !!!");

            write_buffer(buffer, APPERROR);

            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, ".exit") == 0)
        {
            printf("--- EXITING ---\n");
            strcpy(buffer, "--- EXITING ---");
            write_buffer(buffer, TRACE);

            break;
        }

        write_buffer(buffer, atoi(argv[2]));
    }

    fclose(fp); */

    if (argc < 1)
    {
        perror("!!! INVALID ARGUMENT, MISSING <fp> !!!");
        return -1;
    }

    int rc = 0;

    rc = log_init(argv[1]);
    if(rc != 0) {
        perror("!!! ERROR INIT LOG !!!");
        return -1;
    }

    rc = logx(DEBUG, "Questa è una prova di LOGX");
    if(rc != 0) {
        perror("!!! ERROR LOGX !!!");
        return -1;
    }

    rc = syserr_log("Questa è una prova di SYSERR_LOG");
    if(rc != 0) {
        perror("!!! ERROR SYSERR_LOG !!!");
        return -1;
    }

    rc = sysinfo_log("Questa è una prova di SYSINFO_LOG");
    if(rc != 0) {
        perror("!!! ERROR SYSINFO_LOG !!!");
        return -1;
    }

    rc = apperr_log("Questa è una prova di APPERROR_LOG");
    if(rc != 0) {
        perror("!!! ERROR APPERROR_LOG !!!");
        return -1;
    }

    rc = appinfo_log("Questa è una prova di APPINFO_LOG");
    if(rc != 0) {
        perror("!!! ERROR APPINFO_LOG !!!");
        return -1;
    }

    rc = log_stop();
    if(rc != 0){
        perror("!!! ERROR LOG_STOP !!!");
        return -1;
    }

    return 0;
}
