#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include <string>
#include <cstdio>

#ifdef ERROR
#undef ERROR
#endif

class Logger {
public:
    enum Level { INFO = 0, WARN = 1, ERROR = 2 };

    static int  init(const std::string &ruta, Level nivelMinimo = INFO, bool tambienConsola = true);
    static void close();
    static void msg(Level nivel, const char *fmt, ...);

private:
    static FILE *fp;
    static Level nivelMin;
    static bool consola;
    static const char *nivelTexto(Level n);
};

#endif /* SRC_LOGGER_H_ */
