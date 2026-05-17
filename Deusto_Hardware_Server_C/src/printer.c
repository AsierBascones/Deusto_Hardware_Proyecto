#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

#define MaxLine 50

void printProducto(int idProd, char nomProd[MaxLine], char marca[MaxLine], double precio, int stock, char nomCat[MaxLine]) {
    printf("Producto: Id:%i, Nombre: %s, Marca: %s, Precio Unidad: %.2f, Stock: %i, Categoria: %s\n",
           idProd, nomProd, marca, precio, stock, nomCat);
}

void printProductoCompleto(int idProd, char nomProd[MaxLine], char descripcion[MaxLine], char marca[MaxLine], double precio, int stock, char nomCat[MaxLine], char nomProv[MaxLine]) {
    printf("Producto: Id:%i, Nombre: %s, Descripcion: %s, Marca: %s, Precio Unidad: %.2f, Stock: %i, Categoria: %s, Nombre Proveedor: %s\n",
           idProd, nomProd, descripcion, marca, precio, stock, nomCat, nomProv);
}

void printPedido(int idPed, char username[MaxLine], char fechaCreacion[MaxLine], char estado[MaxLine], double total) {
    printf("Pedido: Id: %i, Usuario comprador: %s, Fecha de Compra: %s, Estado del Pedido: %s, Total: %.2f\n",
           idPed, username, fechaCreacion, estado, total);
}

void printProveedor(int idProv, char nombre[MaxLine], char telefono[MaxLine], char email[MaxLine], char direccion[MaxLine], int idCid) {
    printf("Proveedor: Id: %i, Nombre: %s, Telefono: %s, Email: %s, Direccion: %s, ID Ciudad: %i\n",
           idProv, nombre, telefono, email, direccion, idCid);
}

void printError(sqlite3 *db, int tipoError, char tabla[MaxLine]) {
    if (strcmp(sqlite3_errmsg(db), "not an error") != 0) {
        printf("%s\n", sqlite3_errmsg(db));
    }

    switch (tipoError) {
        case 0:
            printf("Error 0: %s no insertado\n", tabla);
            break;
        case 1:
            printf("Error 1: %s no encontrado\n", tabla);
            break;
        case 2:
            printf("Error 2: %s no fue eliminado\n", tabla);
            break;
        case 3:
            printf("Error 3: %s no se pudo actualizar\n", tabla);
            break;
        case 4:
            printf("Error 4: El usuario o la contraseña está mal escrito\n");
            break;
        case 5:
            printf("Error 5: El numero de telefono tiene caracteres incompatibles o es demasiado grande o pequeño\n");
            break;
        case 6:
            printf("Error 6: La opción %s no es válida\n", tabla);
            break;
        default:
            break;
    }
}

void printSucces(int tipoExito, char tabla[MaxLine]) {
    switch (tipoExito) {
        case 0:
            printf("%s eliminado\n", tabla);
            break;
        case 1:
            printf("%s actualizado\n", tabla);
            break;
        case 2:
            printf("%s creado exitosamente\n", tabla);
            break;
        default:
            break;
    }
}
