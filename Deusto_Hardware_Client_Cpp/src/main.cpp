#include <iostream>
#include <string>
#include "SocketClient.h"
#include "MenuUsuario.h"
#include "main.h"
#include "Logger.h"

using namespace std;

int main() {
    Logger::init("logs/cliente.log", Logger::INFO, true);
    Logger::msg(Logger::INFO, "Cliente arrancado");

    cout << "=== CLIENTE DE PRUEBA DEUSTO HARDWARE ===" << endl;

    SocketClient cliente("127.0.0.1", 8080);

    cout << "[*] Intentando conectar con el servidor..." << endl;
    Logger::msg(Logger::INFO, "Intentando conectar a 127.0.0.1:8080");

    if (cliente.conectar()) {
        cout << "[+] Conexion establecida con exito.\n" << endl;
        Logger::msg(Logger::INFO, "Conexion con el servidor establecida");
        Inicio(&cliente);
    } else {
        cout << "[-] Fallo al conectar. ¿Seguro que el servidor esta encendido?" << endl;
    }

    cout << "\nPresiona ENTER para salir...";
    cin.get();

    Logger::msg(Logger::INFO, "Cliente terminado");
    Logger::close();
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
            Logger::msg(Logger::INFO, "Menu principal: 1 - Ver catalogo");
            menu->MostrarCatalogo(cliente);
        } else if (opcion == '2') {
            Logger::msg(Logger::INFO, "Menu principal: 2 - Anadir productos al carrito");
            cout << "\n[ ANADIR PRODUCTO AL CARRITO ]" << endl;
            menu->AnyadirProductos(cliente);
        } else if (opcion == '3') {
            Logger::msg(Logger::INFO, "Menu principal: 3 - Confirmar compra");
            cout << "\n[ PROCESANDO COMPRA ]" << endl;
            menu->ConfirmarCompra(cliente);
        } else if (opcion == '4') {
            Logger::msg(Logger::INFO, "Menu principal: 4 - Ver mis pedidos");
            menu->VerPedidos(cliente);
        } else if (opcion == '5') {
            Logger::msg(Logger::INFO, "Menu principal: 5 - Cerrar sesion de usuario");
            permanecer = false;
            cout << "\n[*] Cerrando sesion de usuario..." << endl;
            Inicio(cliente);
        } else {
            Logger::msg(Logger::WARN, "Menu principal: opcion invalida '%c'", opcion);
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
            Logger::msg(Logger::INFO, "Menu inicial: 1 - Iniciar Sesion");
            cout << "\n[ INICIO DE SESION ]" << endl;
            iniciadoSesion = menu.IniciarSesion(cliente);
            if (iniciadoSesion) {
                permanecer = false;
                Principal(cliente, &menu);
            }
        } else if (opcion == '2') {
            Logger::msg(Logger::INFO, "Menu inicial: 2 - Registrar Usuario");
            cout << "\n[ REGISTRO DE NUEVO USUARIO ]" << endl;
            menu.RegistrarUsuario(cliente);
        } else if (opcion == '3') {
            Logger::msg(Logger::INFO, "Menu inicial: 3 - Salir");
            permanecer = false;
            cliente->desconectar();
            cout << "\n[*] Desconectado del servidor. Gracias por usar Deusto Hardware." << endl;
        } else {
            Logger::msg(Logger::WARN, "Menu inicial: opcion invalida '%c'", opcion);
            cout << "\n[-] Opcion no valida. Intentalo de nuevo." << endl;
        }
    }
}
