#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

static FILE *log_fp = NULL;
static LogLevel log_nivel_min = LOG_INFO;
static int log_a_consola = 1;

static const char *nivel_a_texto(LogLevel n) {
    switch (n) {
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        default:        return "INFO";
    }
}

LogLevel log_nivel_desde_texto(const char *texto) {
    if (texto == NULL) {
        return LOG_INFO;
    }
    if (strcmp(texto, "ERROR") == 0) {
        return LOG_ERROR;
    }
    if (strcmp(texto, "WARN") == 0 || strcmp(texto, "WARNING") == 0) {
        return LOG_WARN;
    }
    return LOG_INFO;
}

int log_init(const char *ruta, LogLevel nivel_minimo, int tambien_consola) {
    log_nivel_min = nivel_minimo;
    log_a_consola = tambien_consola;

    if (ruta == NULL || ruta[0] == '\0') {
        log_fp = NULL;
        return -1;
    }

    log_fp = fopen(ruta, "a");
    if (log_fp == NULL) {
        fprintf(stderr, "[LOGGER] No se pudo abrir el fichero de log: %s\n", ruta);
        return -1;
    }
    log_msg(LOG_INFO, "===== Inicio de sesion de log =====");
    return 0;
}

void log_close(void) {
    if (log_fp != NULL) {
        log_msg(LOG_INFO, "===== Fin de sesion de log =====");
        fclose(log_fp);
        log_fp = NULL;
    }
}

void log_msg(LogLevel nivel, const char *fmt, ...) {
    if (nivel < log_nivel_min) {
        return;
    }

    time_t ahora = time(NULL);
    struct tm *tm_info = localtime(&ahora);
    char ts[32];
    if (tm_info != NULL) {
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm_info);
    } else {
        strcpy(ts, "0000-00-00 00:00:00");
    }

    char mensaje[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(mensaje, sizeof(mensaje), fmt, args);
    va_end(args);

    if (log_fp != NULL) {
        fprintf(log_fp, "[%s] [%s] %s\n", ts, nivel_a_texto(nivel), mensaje);
        fflush(log_fp);
    }

    if (log_a_consola) {
        FILE *salida = (nivel == LOG_ERROR) ? stderr : stdout;
        fprintf(salida, "[%s] [%s] %s\n", ts, nivel_a_texto(nivel), mensaje);
        fflush(salida);
    }
}

