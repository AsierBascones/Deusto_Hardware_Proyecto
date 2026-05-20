#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "MenuUsuario.h"
#include "SocketClient.h"

using namespace std;

MenuUsuario::MenuUsuario() {
    // Iniciamos el menú con un valor no válido por defecto
    this->idUsuarioLogueado = -1;
}

MenuUsuario::~MenuUsuario() {
}

char MenuUsuario::MenuInicial() {
    string opcion;

    cout << "Bienvenido a Deusto Hardware" << endl;
    cout << "1. Iniciar Sesion" << endl;
    cout << "2. Registrar Usuario" << endl;
    cout << "3. Salir" << endl;
    getline(cin, opcion);

    return opcion[0];
}

bool MenuUsuario::IniciarSesion(SocketClient *cliente) {
    string comando = "01";
    string email;
    string contrasenya;
    string respuesta;
    string resCorta;

    cout << "Introducte el email" << endl;
    getline(cin, email);
    cout << "Introducte la contraseña" << endl;
    getline(cin, contrasenya);

    cliente->enviarMensaje(comando + "|" + email + "|" + contrasenya);
    respuesta = cliente->recibirMensaje();

    size_t pos = respuesta.find('|');
    if (pos != string::npos) {
        resCorta = respuesta.substr(0, pos);
    }
    cout << resCorta << endl;

    if (resCorta == "OK") {
        // Aquí extraemos el ID del usuario
        size_t primerPalo = respuesta.find('|');
        size_t segundoPalo = respuesta.find('|', primerPalo + 1);
        size_t tercerPalo = respuesta.find('|', segundoPalo + 1);

        if (tercerPalo != string::npos) {
            string idStr = respuesta.substr(tercerPalo + 1);
            this->idUsuarioLogueado = stoi(idStr); // Lo guardamos
        }
        return true;
    } else {
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

    cout << "Registro de Usuario" << endl;
    cout << "Introduce tu nombre" << endl;
    getline(cin, nombre);
    cout << "Introduce tus apellidos" << endl;
    getline(cin, apellidos);
    cout << "Introduce tu correo" << endl;
    getline(cin, email);
    cout << "Introduce tu contraseña" << endl;
    getline(cin, contrasena);
    cout << "Introduce tu telefono" << endl;
    getline(cin, telefono);
    cout << "Introduce tu direccion" << endl;
    getline(cin, direccion);
    cout << "Introduce tu ciudad" << endl;
    getline(cin, ciudad);

    cliente->enviarMensaje(comando + "|" + nombre + "|" + apellidos + "|" + email + "|" + contrasena + "|" + telefono + "|" + direccion + "|" + ciudad);
    respuesta = cliente->recibirMensaje();

    size_t pos = respuesta.find('|');
    if (pos != string::npos) {
        resCorta = respuesta.substr(0, pos);
    }
    cout << resCorta << endl;

    if (resCorta == "OK") {
        return true;
    } else {
        return false;
    }
}

char MenuUsuario::MenuPrincipal() {
    string opcion;

    cout << "Bienvenido" << endl;
    cout << "1. Ver Catalogo" << endl;
    cout << "2. Anyadir productos al carrito" << endl;
    cout << "3. Confirmar compra" << endl;
    cout << "4. Ver mis pedidos" << endl;
    cout << "5. Volver" << endl;
    getline(cin, opcion);

    return opcion[0];
}

void MenuUsuario::VerPedidos(SocketClient *cliente) {
    string comando = "09";
    string respuesta;

    // Aquí ya no pedimos el ID al usuario, lo usamos directamente
    cout << "[*] Consultando el historial del usuario..." << endl;
    cliente->enviarMensaje(comando + "|" + to_string(this->idUsuarioLogueado));
    respuesta = cliente->recibirMensaje();
    cout << respuesta << endl;
}

void MenuUsuario::ConfirmarCompra(SocketClient *cliente) {
    // Si la cesta local está vacía, no hacemos ninguna llamada al server
    if (carritoLocal.empty()) {
        cout << "El carrito esta vacio. Anyade productos antes de comprar." << endl;
        return;
    }

    string respuesta;
    string comando = "06";

    // Aquí también usamos el ID del usuario
    string texto = comando + "|" + to_string(this->idUsuarioLogueado) + "|";

    // Recorremos todo nuestro carrito local en RAM para empaquetarlo (id,cant#id,cant)
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
    cout << respuesta << endl;

    // Si el servidor devuelve OK, vaciamos el carrito local
    if (respuesta.substr(0, 2) == "OK") {
        carritoLocal.clear();
        cout << "[*] Compra finalizada. Carrito local vaciado." << endl;
    }
}

void MenuUsuario::AnyadirProductos(SocketClient *cliente) {
    // Esta función ya no usa el comando 10 ni envía datos por red.
    // Guarda la información en la estructura 'carritoLocal' dentro de la memoria RAM del cliente.
    string idProd;
    string cantidadProd;
    bool valido = false;

    cout << "Introducir el id del Producto" << endl;
    getline(cin, idProd);
    while (!valido) {
        if (all_of(idProd.begin(), idProd.end(), ::isdigit)) {
            valido = true;
        } else {
            cout << "Solo se permiten dígitos" << endl;
            cout << "Introducte el id del Producto" << endl;
            getline(cin, idProd);
        }
    }

    cout << "Introducir la cantidad" << endl;
    getline(cin, cantidadProd);
    valido = false;
    while (!valido) {
        if (all_of(cantidadProd.begin(), cantidadProd.end(), ::isdigit)) {
            valido = true;
        } else {
            cout << "Solo se permiten dígitos" << endl;
            cout << "Introducir la cantidad" << endl;
            getline(cin, cantidadProd);
        }
    }

    // Si el producto ya estaba en la cesta local, sumamos la cantidad para evitar repetidos
    bool duplicado = false;
    for (auto &item : carritoLocal) {
        if (item.idProducto == idProd) {
            int total = stoi(item.cantidad) + stoi(cantidadProd);
            item.cantidad = to_string(total);
            duplicado = true;
            break;
        }
    }

    // Si no estaba repetido, metemos el struct nuevo en el vector
    if (!duplicado) {
        ItemCarrito nuevoItem = {idProd, cantidadProd};
        carritoLocal.push_back(nuevoItem);
    }

    cout << "[+] Producto guardado en el carrito de memoria local (Peticiones al servidor: 0)" << endl;
}
