#include <iostream>
#include "SocketClient.h"

using namespace std;

int main() {
    cout << "=== CLIENTE DE PRUEBA DEUSTO HARDWARE ===" << endl;

    SocketClient cliente("127.0.0.1", 8080);

    cout << "[*] Intentando conectar con el servidor..." << endl;

    if (cliente.conectar()) {
        cout << "[+] Conexion establecida con exito.\n" << endl;

        string comando = "05";
        cout << "Cliente -> Servidor: Enviando peticion '" << comando << "'" << endl;

        cliente.enviarMensaje(comando);

        string respuesta = cliente.recibirMensaje();

        cout << "Servidor -> Cliente: \n" << respuesta << "\n" << endl;

        cliente.desconectar();
        cout << "[*] Desconectado del servidor." << endl;

    } else {
        cout << "[-] Fallo al conectar. ¿Seguro que el servidor esta encendido?" << endl;
    }

    cout << "\nPresiona ENTER para salir...";
    cin.get();

    return 0;
}
