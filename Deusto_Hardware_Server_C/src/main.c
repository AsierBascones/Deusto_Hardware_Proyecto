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
        printf("\nOpcion: ");
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
            sqlite3_close(db);
            exit(0);
        } else {
            printf("Opcion no valida\n");
        }
    }
}

int main(int argc, char **argv) {
    sqlite3 *db;

    if (cargar_configuracion("config/config.txt", &config) == 0) {
        printf("Configuración cargada correctamente.\n");
        mostrar_configuracion(&config);
    } else {
        printf("ADVERTENCIA: Usando configuración por defecto.\n");
    }

    printf("Arrancando el servidor de Deusto Hardware...\n");

    // Esto abre o crea el fichero de la BD
    printf("Usando base de datos: %s\n", config.bd_ruta);
    int resultado = sqlite3_open(config.bd_ruta, &db);

    if (resultado != SQLITE_OK) {
        printf("Error fatal al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return resultado;
    }

    inicializar_base_datos(db);
    rellenar_base_datos(db);

    printf("Bienvenido a Deusto Hardware\n");

    serverOAdmin(db);

    return 0;
}
