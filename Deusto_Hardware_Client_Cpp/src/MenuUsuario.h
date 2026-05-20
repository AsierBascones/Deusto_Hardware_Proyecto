#ifndef MENUUSUARIO_H
#define MENUUSUARIO_H

#include "SocketClient.h"
#include <string>
#include <vector>

struct ItemCarrito {
    std::string idProducto;
    std::string cantidad;
};

class MenuUsuario {
private:
    int idUsuarioLogueado;
    std::vector<ItemCarrito> carritoLocal;

public:
    char MenuInicial();
    bool IniciarSesion(SocketClient *cliente);
    bool RegistrarUsuario(SocketClient *cliente);
    char MenuPrincipal();
    void VerPedidos(SocketClient *client);
    void AnyadirProductos(SocketClient *client);
    void ConfirmarCompra(SocketClient *client);

    MenuUsuario();
    virtual ~MenuUsuario();
};

#endif
