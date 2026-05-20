#include "Logger.h"

#include <cstdarg>
#include <cstring>
#include <ctime>
#include <cstdio>

FILE *Logger::fp = NULL;
Logger::Level Logger::nivelMin = Logger::INFO;
bool Logger::consola = true;

const char *Logger::nivelTexto(Level n) {
    switch (n) {
        case INFO:  return "INFO";
        case WARN:  return "WARN";
        case ERROR: return "ERROR";
        default:    return "INFO";
    }
}

int Logger::init(const std::string &ruta, Level nivelMinimo, bool tambienConsola) {
    nivelMin = nivelMinimo;
    consola = tambienConsola;

    if (ruta.empty()) {
        fp = NULL;
        return -1;
    }

    fp = std::fopen(ruta.c_str(), "a");
    if (fp == NULL) {
        std::fprintf(stderr, "[LOGGER] No se pudo abrir el fichero de log: %s\n", ruta.c_str());
        return -1;
    }

    msg(INFO, "===== Inicio de sesion de log (cliente) =====");
    return 0;
}

void Logger::close() {
    if (fp != NULL) {
        msg(INFO, "===== Fin de sesion de log (cliente) =====");
        std::fclose(fp);
        fp = NULL;
    }
}

void Logger::msg(Level nivel, const char *fmt, ...) {
    if (nivel < nivelMin) {
        return;
    }

    std::time_t ahora = std::time(NULL);
    std::tm *tm_info = std::localtime(&ahora);
    char ts[32];
    if (tm_info != NULL) {
        std::strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm_info);
    } else {
        std::strcpy(ts, "0000-00-00 00:00:00");
    }

    char mensaje[1024];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(mensaje, sizeof(mensaje), fmt, args);
    va_end(args);

    if (fp != NULL) {
        std::fprintf(fp, "[%s] [%s] %s\n", ts, nivelTexto(nivel), mensaje);
        std::fflush(fp);
    }

    if (consola) {
        FILE *salida = (nivel == ERROR) ? stderr : stdout;
        std::fprintf(salida, "[%s] [%s] %s\n", ts, nivelTexto(nivel), mensaje);
        std::fflush(salida);
    }
}
