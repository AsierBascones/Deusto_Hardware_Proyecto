#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cargar_configuracion(const char *ruta_archivo, Configuracion *cfg) {
    FILE *f = fopen(ruta_archivo, "r");
    if (!f) {
        printf("ERROR: No se pudo abrir %s\n", ruta_archivo);
        printf("Usando valores por defecto...\n");

        strcpy(cfg->admin_usuario, "admin");
        strcpy(cfg->admin_contrasena, "admin123");
        strcpy(cfg->bd_ruta, "data/deusto_hardware.sqlite");
        strcpy(cfg->ruta_importacion, "data/importar/");
        strcpy(cfg->ruta_exportacion, "data/exportar/");
        strcpy(cfg->log_ruta, "logs/servidor.log");
        strcpy(cfg->log_nivel, "INFO");
        cfg->log_consola = 1;

        return -1;
    }

    strcpy(cfg->admin_usuario, "admin");
    strcpy(cfg->admin_contrasena, "admin123");
    strcpy(cfg->bd_ruta, "data/deusto_hardware.sqlite");
    strcpy(cfg->ruta_importacion, "data/importar/");
    strcpy(cfg->ruta_exportacion, "data/exportar/");
    strcpy(cfg->log_ruta, "logs/servidor.log");
    strcpy(cfg->log_nivel, "INFO");
    cfg->log_consola = 1;

    char linea[256];
    char clave[100], valor[200];

    while (fgets(linea, sizeof(linea), f)) {
        if (linea[0] == '#' || linea[0] == '\n') {
            continue;
        }

        linea[strcspn(linea, "\n")] = 0;

        char *igual = strchr(linea, '=');
        if (igual == NULL) {
            continue;
        }

        *igual = '\0';
        strcpy(clave, linea);
        strcpy(valor, igual + 1);

        char *p = clave;
        while (*p == ' ') p++;
        char *end = p + strlen(p) - 1;
        while (end > p && *end == ' ') end--;
        *(end + 1) = '\0';
        char clave_limpia[100];
        strcpy(clave_limpia, p);

        p = valor;
        while (*p == ' ') p++;
        end = p + strlen(p) - 1;
        while (end > p && *end == ' ') end--;
        *(end + 1) = '\0';

        if (strcmp(clave_limpia, "ADMIN_USUARIO") == 0) {
            strcpy(cfg->admin_usuario, p);
        } else if (strcmp(clave_limpia, "ADMIN_CONTRASENA") == 0) {
            strcpy(cfg->admin_contrasena, p);
        } else if (strcmp(clave_limpia, "BD_RUTA") == 0) {
            strcpy(cfg->bd_ruta, p);
        } else if (strcmp(clave_limpia, "RUTA_IMPORTACION") == 0) {
            strcpy(cfg->ruta_importacion, p);
        } else if (strcmp(clave_limpia, "RUTA_EXPORTACION") == 0) {
            strcpy(cfg->ruta_exportacion, p);
        } else if (strcmp(clave_limpia, "LOG_RUTA") == 0) {
            strcpy(cfg->log_ruta, p);
        } else if (strcmp(clave_limpia, "LOG_NIVEL") == 0) {
            strcpy(cfg->log_nivel, p);
        } else if (strcmp(clave_limpia, "LOG_CONSOLA") == 0) {
            cfg->log_consola = atoi(p);
        }
    }

    fclose(f);
    return 0;
}

void mostrar_configuracion(const Configuracion *cfg) {
    printf("\n=== CONFIGURACIÓN CARGADA ===\n");
    printf("Admin usuario:      %s\n", cfg->admin_usuario);
    printf("Admin contraseña:   %s\n", cfg->admin_contrasena);
    printf("Ruta base de datos: %s\n", cfg->bd_ruta);
    printf("Ruta importación:   %s\n", cfg->ruta_importacion);
    printf("Ruta exportación:   %s\n", cfg->ruta_exportacion);
    printf("Ruta logs:          %s\n", cfg->log_ruta);
    printf("Nivel log:          %s\n", cfg->log_nivel);
    printf("Log a consola:      %d\n", cfg->log_consola);
    printf("==============================\n\n");
}
