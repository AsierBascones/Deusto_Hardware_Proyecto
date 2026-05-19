#include <iostream>
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
void Principal(SocketClient *cliente, MenuUsuario* menu){
	bool permanecer = true;
	char opcion;
	while(permanecer){
		opcion = menu->MenuPrincipal();
		if(opcion == '1'){
			string comando = "05";
			cout << "Cliente -> Servidor: Enviando peticion '" << comando << "'" << endl;

			cliente->enviarMensaje(comando);

			string respuesta = cliente->recibirMensaje();

			cout << "Servidor -> Cliente: \n" << respuesta << "\n" << endl;
		}else if(opcion == '2'){
			menu->AnyadirProductos(cliente);
		}else if(opcion == '3'){
			menu->ConfirmarCompra(cliente);
		}else if(opcion == '4'){
			menu->VerPedidos(cliente);
		}else if(opcion == '5'){
			permanecer = false;
			Inicio(cliente);
		}
	}
}
void Inicio(SocketClient *cliente){
	MenuUsuario menu = MenuUsuario();
	char opcion;
	bool permanecer = true;
	bool iniciadoSesion = false;
	while (permanecer){
		opcion = menu.MenuInicial();
		if(opcion == '1'){
			iniciadoSesion = menu.IniciarSesion(cliente);
			if(iniciadoSesion){
				permanecer = false;
				Principal(cliente, &menu);
			}
	    }else if(opcion == '2'){
	    	menu.RegistrarUsuario(cliente);
	    }else if(opcion == '3'){
	    	permanecer = false;
	    	cliente->desconectar();
	        cout << "[*] Desconectado del servidor." << endl;
	    }else{
	        cout << "No es válido" << endl;
	    }
	}
}
