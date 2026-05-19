#ifndef MENUUSUARIO_H
#define MENUUSUARIO_H
#include "SocketClient.h"
class MenuUsuario{
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
#endif // SOCKETCLIENT_H
