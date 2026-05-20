#include "SocketClient.h"
#include "Logger.h"
#include <iostream>

using namespace std;

SocketClient::SocketClient(const string &ipServidor, int puertoServidor) {
	this->ip = ipServidor;
	this->puerto = puertoServidor;
	this->sock = INVALID_SOCKET;
	this->conectado = false;
}

SocketClient::~SocketClient() {
	desconectar();
}

bool SocketClient::conectar() {
	WSADATA wsaData;

	// Inicializar Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "Error: No se pudo inicializar Winsock.\n";
		return false;
	}

	// Crear el socket TCP
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Error: No se puede crear el socket.\n";
		WSACleanup();
		return false;
	}

	// Configurar la dirección del servidor
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(puerto);
	serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	// Intentar conectar
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		cerr << "Error: No se pudo conectar al servidor en " << ip << ":" << puerto << "\n";
		closesocket(sock);
		WSACleanup();
		return false;
	}

	conectado = true;
	Logger::msg(Logger::INFO, "Socket conectado a %s:%d", ip.c_str(), puerto);
	return true;
}

void SocketClient::desconectar() {
	if (conectado) {
		enviarMensaje("00");

		closesocket(sock);
		WSACleanup();
		conectado = false;
		Logger::msg(Logger::INFO, "Socket desconectado");
	}
}

bool SocketClient::enviarMensaje(const string& mensaje) {
	if (!conectado) return false;

	// Enviamos el mensaje más un salto de línea (para que el servidor sepa que terminó)
	string paquete = mensaje + "\n";

	int enviado = send(sock, paquete.c_str(), paquete.length(), 0);
	if (enviado == SOCKET_ERROR) {
		cerr << "Error al enviar datos.\n";
		return false;
	}
	Logger::msg(Logger::INFO, "-> servidor: %s", mensaje.c_str());
	return true;
}

string SocketClient::recibirMensaje() {
	if (!conectado) return "";

	char buffer[4096];
	memset(buffer, 0, 4096);

	int recibidos = recv(sock, buffer, 4096 - 1, 0);
	if (recibidos > 0) {
		return string(buffer);
	} else if (recibidos == 0) {
		cout << "El servidor ha cerrado la conexión.\n";
		Logger::msg(Logger::WARN, "El servidor ha cerrado la conexion");
		desconectar();
	} else {
		cerr << "Error al recibir datos.\n";
	}
	return "";
}

bool SocketClient::isConectado() const {
	return conectado;
}
