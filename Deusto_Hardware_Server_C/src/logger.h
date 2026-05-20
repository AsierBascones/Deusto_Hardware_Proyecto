#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

typedef enum {
    LOG_INFO  = 0,
    LOG_WARN  = 1,
    LOG_ERROR = 2
} LogLevel;

int  log_init(const char *ruta, LogLevel nivel_minimo, int tambien_consola);
void log_close(void);
void log_msg(LogLevel nivel, const char *fmt, ...);
LogLevel log_nivel_desde_texto(const char *texto);

#endif /* SRC_LOGGER_H_ */
