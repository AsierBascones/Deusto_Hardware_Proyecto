#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <string>
#include <winsock2.h>

class SocketClient {
private:
	std::string ip;
	int puerto;
	SOCKET sock;
	bool conectado;

public:
	SocketClient(const std::string &ipServidor, int puertoServidor);
	~SocketClient();

	bool conectar();
	void desconectar();
	bool enviarMensaje(const std::string &mensaje);
	std::string recibirMensaje();

	bool isConectado() const;
};

#endif // SOCKETCLIENT_H
