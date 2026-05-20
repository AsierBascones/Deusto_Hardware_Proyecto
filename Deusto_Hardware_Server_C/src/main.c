#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <io.h>
#include <windows.h>
#include "main_server.h"
#include "main_admin.h"
#include "main.h"
#include "config.h"
#include "db_manager.h"
#include "sqlite3.h"
#include "logger.h"

#define MaxLine 50
#define SQLITE_CHECKPOINT_TRUNCATE 3

Configuracion config;

void borrarBase(sqlite3* db) {
    sqlite3_wal_checkpoint_v2(db, NULL, SQLITE_CHECKPOINT_TRUNCATE, NULL, NULL);

    // Cerrar todos los statements pendientes si los hay
    sqlite3_stmt* stmt;
    while ((stmt = sqlite3_next_stmt(db, NULL)) != NULL) {
        sqlite3_finalize(stmt);
    }

    sqlite3_close_v2(db);
    db = NULL;

    Sleep(200);

    chmod(config.bd_ruta, 0666);

    if (remove(config.bd_ruta) != 0) {
        perror("Error eliminando BD");
        log_msg(LOG_ERROR, "No se pudo eliminar el fichero de BD '%s'", config.bd_ruta);
    } else {
    	log_msg(LOG_WARN, "Base de datos eliminada por peticion del admin (%s)", config.bd_ruta);
    }
}

void serverOAdmin(sqlite3 *db) {
    bool permanecer = true;
    char opcion;
    char str[MaxLine];

    while (permanecer) {
        printf("\n1. Servidor\n");
        printf("2. Admin\n");
        printf("3. Salir\n");
        printf("Opcion: ");
        fflush(stdout);

        fgets(str, 50, stdin);
        clearLines(str, MaxLine);
        sscanf(str, "%c", &opcion);
        printf("%c\n", opcion);

        if (opcion == '1') {
            server(db);
        } else if (opcion == '2') {
            inicio(db);
        } else if (opcion == '3') {
        	log_msg(LOG_INFO, "Servidor cerrado por el operador desde el menu principal");
            sqlite3_close(db);
            log_close();
            exit(0);
        } else {
            printf("Opcion no valida\n");
        }
    }
}

int main(int argc, char **argv) {
    sqlite3 *db;
    int config_ok;

    config_ok = cargar_configuracion("config/config.txt", &config);

    if (log_init(config.log_ruta,
                 log_nivel_desde_texto(config.log_nivel),
                 config.log_consola) != 0) {
        fprintf(stderr, "AVISO: El log se escribira solo por consola (no se pudo abrir %s)\n",
                config.log_ruta);
    }

    if (config_ok == 0) {
        log_msg(LOG_INFO, "Configuracion cargada desde config/config.txt");
        printf("Configuracion cargada correctamente.\n");
        mostrar_configuracion(&config);
    } else {
        log_msg(LOG_WARN, "No se pudo cargar config/config.txt, se usan valores por defecto");
        printf("ADVERTENCIA: Usando configuracion por defecto.\n");
    }

    log_msg(LOG_INFO, "Arrancando el servidor de Deusto Hardware");
    printf("Arrancando el servidor de Deusto Hardware...\n");

    printf("Usando base de datos: %s\n", config.bd_ruta);
    int resultado = sqlite3_open(config.bd_ruta, &db);

    if (resultado != SQLITE_OK) {
        log_msg(LOG_ERROR, "Error fatal al abrir la BD '%s': %s",
                config.bd_ruta, sqlite3_errmsg(db));
        printf("Error fatal al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        log_close();
        return resultado;
    }

    inicializar_base_datos(db);
    rellenar_base_datos(db);

    printf("\nBienvenido a Deusto Hardware");

    serverOAdmin(db);

    log_close();
    return 0;
}
