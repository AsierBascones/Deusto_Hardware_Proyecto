#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include "MenuUsuario.h"
#include "SocketClient.h"
#include "Logger.h"

using namespace std;

MenuUsuario::MenuUsuario() {
    this->idUsuarioLogueado = -1;
    this->catalogoDescargado = false;
}

MenuUsuario::~MenuUsuario() {
}

char MenuUsuario::MenuInicial() {
    string opcion;

    cout << " Bienvenido a Deusto Hardware" << endl;
    cout << " 1. Iniciar Sesion" << endl;
    cout << " 2. Registrar Usuario" << endl;
    cout << " 3. Salir" << endl;
    cout << " Selecciona una opcion: ";
    getline(cin, opcion);

    return opcion[0];
}

bool MenuUsuario::IniciarSesion(SocketClient *cliente) {
    string comando = "01";
    string email;
    string contrasenya;
    string respuesta;
    string resCorta;

    cout << " Introduce el email: ";
    getline(cin, email);
    cout << " Introduce la clave: ";
    getline(cin, contrasenya);

    cliente->enviarMensaje(comando + "|" + email + "|" + contrasenya);
    respuesta = cliente->recibirMensaje();

    size_t pos = respuesta.find('|');
    if (pos != string::npos) {
        resCorta = respuesta.substr(0, pos);
    }

    if (resCorta == "OK") {
        cout << "\n[+] Login correcto. Bienvenido de nuevo." << endl;

        size_t primerPalo = respuesta.find('|');
        size_t segundoPalo = respuesta.find('|', primerPalo + 1);
        size_t tercerPalo = respuesta.find('|', segundoPalo + 1);

        if (tercerPalo != string::npos) {
            string idStr = respuesta.substr(tercerPalo + 1);
            this->idUsuarioLogueado = stoi(idStr);
        }
        Logger::msg(Logger::INFO, "Login correcto (email=%s, id=%d)", email.c_str(), this->idUsuarioLogueado);
        return true;
    } else {
        cout << "\n[-] Error al iniciar sesion. Verifica tus datos." << endl;
        Logger::msg(Logger::WARN, "Login fallido (email=%s)", email.c_str());
        return false;
    }
}

bool MenuUsuario::RegistrarUsuario(SocketClient *cliente) {
    string nombre;
    string apellidos;
    string email;
    string contrasena;
    string telefono;
    string direccion;
    string ciudad;
    string comando = "03";
    string respuesta;
    string resCorta;

    cout << " Introduce tu nombre: ";
    getline(cin, nombre);
    cout << " Introduce tus apellidos: ";
    getline(cin, apellidos);
    cout << " Introduce tu correo: ";
    getline(cin, email);
    cout << " Introduce tu clave: ";
    getline(cin, contrasena);
    cout << " Introduce tu telefono: ";
    getline(cin, telefono);
    cout << " Introduce tu direccion: ";
    getline(cin, direccion);
    cout << " Introduce tu ciudad: ";
    getline(cin, ciudad);

    cliente->enviarMensaje(comando + "|" + nombre + "|" + apellidos + "|" + email + "|" + contrasena + "|" + telefono + "|" + direccion + "|" + ciudad);
    respuesta = cliente->recibirMensaje();

    size_t pos = respuesta.find('|');
    if (pos != string::npos) {
        resCorta = respuesta.substr(0, pos);
    }
    cout << "\nRespuesta del servidor: " << resCorta << endl;

    if (resCorta == "OK") {
        Logger::msg(Logger::INFO, "Registro de cliente OK (email=%s, ciudad=%s)", email.c_str(), ciudad.c_str());
        return true;
    } else {
        Logger::msg(Logger::WARN, "Registro de cliente fallido (email=%s)", email.c_str());
        return false;
    }
}

char MenuUsuario::MenuPrincipal() {
    string opcion;

    cout << " MENU PRINCIPAL" << endl;
    cout << " 1. Ver Catalogo" << endl;
    cout << " 2. Anadir productos al carrito" << endl;
    cout << " 3. Confirmar compra" << endl;
    cout << " 4. Ver mis pedidos" << endl;
    cout << " 5. Volver al menu inicial" << endl;
    cout << " Selecciona una opcion: ";
    getline(cin, opcion);

    return opcion[0];
}

void MenuUsuario::VerPedidos(SocketClient *cliente) {
    string comando = "09";
    string respuesta;

    Logger::msg(Logger::INFO, "Consultando historial de pedidos (idUsuario=%d)", this->idUsuarioLogueado);
    cout << "\n[*] Consultando el historial del usuario..." << endl;
    cliente->enviarMensaje(comando + "|" + to_string(this->idUsuarioLogueado));
    respuesta = cliente->recibirMensaje();

    cout << "\n=============================================" << endl;
    cout << "           HISTORIAL DE PEDIDOS              " << endl;
    cout << "=============================================" << endl;

    // Si el servidor devuelve un error o no tiene pedidos
    if (respuesta.substr(0, 3) == "ERR" || respuesta.empty()) {
        cout << "   No se encontraron pedidos en tu cuenta." << endl;
        cout << "=============================================" << endl;
        return;
    }

    // Quitamos el "OK|" del principio para quedarnos solo con los datos
    if (respuesta.substr(0, 3) == "OK|") {
        respuesta = respuesta.substr(3);
    }

    stringstream ss(respuesta);
    string fila;
    bool tienePedidos = false;

    // Troceamos por '#' si el usuario tuviera varios pedidos realizados
    while (getline(ss, fila, '#')) {
        if (fila.empty()) continue;

        stringstream ssFila(fila);
        string idPed, fecha, estado, totalStr;

        // Troceamos cada campo del pedido por ';'
        getline(ssFila, idPed, ';');
        getline(ssFila, fecha, ';');
        getline(ssFila, estado, ';');
        getline(ssFila, totalStr, ';');

        if (!idPed.empty()) {
            tienePedidos = true;
            cout << " Pedido ID:  #" << idPed << endl;
            cout << " Fecha:      " << fecha << endl;
            cout << " Estado:     " << estado << endl;
            cout << " Total:      " << totalStr << " EUR" << endl;
            cout << "---------------------------------------------" << endl;
        }
    }

    if (!tienePedidos) {
        cout << "No se encontraron pedidos en tu cuenta." << endl;
    }
    cout << "=============================================" << endl;
}

void MenuUsuario::ConfirmarCompra(SocketClient *cliente) {
    if (carritoLocal.empty()) {
        cout << "[-] El carrito esta vacio. Anade productos antes de comprar." << endl;
        Logger::msg(Logger::WARN, "Confirmar compra abortado: carrito vacio");
        return;
    }

    string respuesta;
    string comando = "06";
    string texto = comando + "|" + to_string(this->idUsuarioLogueado) + "|";

    for (size_t i = 0; i < carritoLocal.size(); i++) {
        if (i > 0) {
            texto += "#";
        }
        texto += carritoLocal[i].idProducto;
        texto += ",";
        texto += carritoLocal[i].cantidad;
    }

    cliente->enviarMensaje(texto);
    respuesta = cliente->recibirMensaje();

    if (respuesta.substr(0, 2) == "OK") {
        Logger::msg(Logger::INFO, "Compra confirmada por el servidor (idUsuario=%d, items=%zu)", this->idUsuarioLogueado, carritoLocal.size());
        carritoLocal.clear();
        cout << "[+] Compra realizada correctamente en el servidor." << endl;
        cout << "[*] Carrito local vaciado con exito." << endl;
    } else {
        Logger::msg(Logger::WARN, "Compra rechazada por el servidor: %s", respuesta.c_str());
        cout << "[-] Error al procesar la compra: " << respuesta << endl;
    }
}

void MenuUsuario::AnyadirProductos(SocketClient *cliente) {
    string idProd;
    string cantidadProd;
    bool valido = false;

    cout << " Introduce el ID del Producto: ";
    getline(cin, idProd);
    while (!valido) {
        if (all_of(idProd.begin(), idProd.end(), ::isdigit) && !idProd.empty()) {
            valido = true;
        } else {
            cout << " [!] Solo se permiten digitos. Introduce el ID: ";
            getline(cin, idProd);
        }
    }

    cout << " Introduce la cantidad: ";
    getline(cin, cantidadProd);
    valido = false;
    while (!valido) {
        if (all_of(cantidadProd.begin(), cantidadProd.end(), ::isdigit) && !cantidadProd.empty()) {
            valido = true;
        } else {
            cout << " [!] Solo se permiten digitos. Introduce la cantidad: ";
            getline(cin, cantidadProd);
        }
    }

    bool duplicado = false;
    for (auto &item : carritoLocal) {
        if (item.idProducto == idProd) {
            int total = stoi(item.cantidad) + stoi(cantidadProd);
            item.cantidad = to_string(total);
            duplicado = true;
            break;
        }
    }

    if (!duplicado) {
        ItemCarrito nuevoItem = {idProd, cantidadProd};
        carritoLocal.push_back(nuevoItem);
        Logger::msg(Logger::INFO, "Anadido al carrito local: producto=%s, cantidad=%s", idProd.c_str(), cantidadProd.c_str());
    } else {
        Logger::msg(Logger::INFO, "Cantidad actualizada en carrito local: producto=%s (+%s)", idProd.c_str(), cantidadProd.c_str());
    }

    cout << "\n[+] Producto guardado en el carrito de memoria local (Peticiones al servidor: 0)" << endl;
}

void MenuUsuario::MostrarCatalogo(SocketClient *cliente) {
    if (this->catalogoDescargado) {
        Logger::msg(Logger::INFO, "Mostrar catalogo: desde cache local (%zu productos)", catalogoLocal.size());
        cout << "\n=== CATALOGO DE PRODUCTOS (Desde Cache Local en RAM) ===" << endl;
        for (const auto& prod : catalogoLocal) {
            prod.mostrar();
        }
        return;
    }
    Logger::msg(Logger::INFO, "Mostrar catalogo: descargando del servidor...");

    string comando = "05";
    cliente->enviarMensaje(comando);
    string respuesta = cliente->recibirMensaje();

    if (respuesta.substr(0, 3) == "OK|") {
        respuesta = respuesta.substr(3);
    }

    stringstream ss(respuesta);
    string fila;

    while (getline(ss, fila, '#')) {
        if (fila.empty()) continue;

        stringstream ssFila(fila);
        string idStr, nombre, desc, precioStr, stockStr, marca, categoria;

        getline(ssFila, idStr, ';');
        getline(ssFila, nombre, ';');
        getline(ssFila, desc, ';');
        getline(ssFila, precioStr, ';');
        getline(ssFila, stockStr, ';');
        getline(ssFila, marca, ';');
        getline(ssFila, categoria, ';');

        if (!idStr.empty()) {
            Producto prod(
                stoi(idStr),
                nombre,
                desc,
                stod(precioStr),
                stoi(stockStr),
                marca,
                categoria
            );
            this->catalogoLocal.push_back(prod);
        }
    }

    this->catalogoDescargado = true;
    Logger::msg(Logger::INFO, "Catalogo descargado del servidor (%zu productos)", catalogoLocal.size());

    cout << "\n=== CATALOGO DE PRODUCTOS (Descargado del Servidor) ===" << endl;
    for (const auto& prod : catalogoLocal) {
        prod.mostrar();
    }
}
