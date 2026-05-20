#include <iostream>
#include <string>
#include "SocketClient.h"
#include "MenuUsuario.h"
#include "main.h"

using namespace std;

int main() {
    cout << "=== CLIENTE DE PRUEBA DEUSTO HARDWARE ===" << endl;

    SocketClient cliente("127.0.0.1", 8080);

    cout << "[*] Intentando conectar con el servidor..." << endl;

    if (cliente.conectar()) {
        cout << "[+] Conexion establecida con exito.\n" << endl;
        Inicio(&cliente);
    } else {
        cout << "[-] Fallo al conectar. ¿Seguro que el servidor esta encendido?" << endl;
    }

    cout << "\nPresiona ENTER para salir...";
    cin.get();

    return 0;
}

void Principal(SocketClient *cliente, MenuUsuario* menu) {
    bool permanecer = true;
    char opcion;

    while (permanecer) {
        cout << "\n=============================================" << endl;
        opcion = menu->MenuPrincipal();
        cout << "=============================================" << endl;

        if (opcion == '1') {
            menu->MostrarCatalogo(cliente);
        } else if (opcion == '2') {
            cout << "\n[ ANADIR PRODUCTO AL CARRITO ]" << endl;
            menu->AnyadirProductos(cliente);
        } else if (opcion == '3') {
            cout << "\n[ PROCESANDO COMPRA ]" << endl;
            menu->ConfirmarCompra(cliente);
        } else if (opcion == '4') {
            menu->VerPedidos(cliente);
        } else if (opcion == '5') {
            permanecer = false;
            cout << "\n[*] Cerrando sesion de usuario..." << endl;
            Inicio(cliente);
        }
    }
}

void Inicio(SocketClient *cliente) {
    MenuUsuario menu = MenuUsuario();
    char opcion;
    bool permanecer = true;
    bool iniciadoSesion = false;

    while (permanecer) {
        cout << "\n=============================================" << endl;
        opcion = menu.MenuInicial();
        cout << "=============================================" << endl;

        if (opcion == '1') {
            cout << "\n[ INICIO DE SESION ]" << endl;
            iniciadoSesion = menu.IniciarSesion(cliente);
            if (iniciadoSesion) {
                permanecer = false;
                Principal(cliente, &menu);
            }
        } else if (opcion == '2') {
            cout << "\n[ REGISTRO DE NUEVO USUARIO ]" << endl;
            menu.RegistrarUsuario(cliente);
        } else if (opcion == '3') {
            permanecer = false;
            cliente->desconectar();
            cout << "\n[*] Desconectado del servidor. Gracias por usar Deusto Hardware." << endl;
        } else {
            cout << "\n[-] Opcion no valida. Intentalo de nuevo." << endl;
        }
    }
}
