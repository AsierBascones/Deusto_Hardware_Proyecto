#include <iostream>
#include "MenuUsuario.h"
#include "SocketClient.h"
#include <string>
#include <iostream>
using namespace std;
MenuUsuario::MenuUsuario(){
}
MenuUsuario::~MenuUsuario(){

}
char MenuUsuario::MenuInicial(){
	char opcion;

	cout << "Bienvenido a Deusto Hardware" << endl;
	cout << "1. Iniciar Sesion" << endl;
	cout << "2. Registrar Usuario" << endl;
	cout << "3. Salir" << endl;
	cin >> opcion;
	return opcion;
}
bool MenuUsuario::IniciarSesion(SocketClient *cliente){
	string comando = "01";
	string email;
	string contrasenya;
	string respuesta;
	string resCorta;
	cout << "Introducte el email" << endl;
	cin >> email;
	cout << "Introducte la contraseña" << endl;
	cin >> contrasenya;
	cliente->enviarMensaje(comando + "|" + email + "|" + contrasenya);
	respuesta = cliente->recibirMensaje();
	size_t pos = respuesta.find('|');
	if (pos != string::npos) {
		resCorta = respuesta.substr(0, pos);
	}
	cout << resCorta << endl;
	if(strcmp(resCorta.c_str(), "OK") == 0){
		return true;
	}else{
		return false;
	}
}
bool MenuUsuario::RegistrarUsuario(SocketClient *cliente){
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
	cin >> nombre;
	cout << "Introduce tus apellidos" << endl;
	cin >> apellidos;
	cout << "Introduce tu correo" << endl;
	cin >> email;
	cout << "Introduce tu contraseña" << endl;
	cin >> contrasena;
	cout << "Introduce tu telefono" << endl;
	cin >> telefono;
	cout << "Introduce tu direccion" << endl;
	cin >> direccion;
	cout << "Introduce tu ciudad" << endl;
	cin >> ciudad;
	cliente->enviarMensaje(comando + "|" + nombre + "|" + apellidos + "|" + email + "|" + contrasena + "|" + telefono + "|" + direccion + "|" + ciudad);
	respuesta = cliente->recibirMensaje();
	size_t pos = respuesta.find('|');
	if (pos != string::npos) {
		resCorta = respuesta.substr(0, pos);
	}
	cout << resCorta << endl;
	if(strcmp(resCorta.c_str(), "OK") == 0){
		return true;
	}else{
		return false;
	}
}
char MenuUsuario::MenuPrincipal(){
	char opcion;
	cout << "Bienvenido" << endl;
	cout << "1. Ver Catalogo" << endl;
	cout << "2. Anyadir productos al carrito" << endl;
	cout << "3. Confirmar compra" << endl;
	cout << "4. Ver mis pedidos" << endl;
	cout << "5. Volver" << endl;
	cin >> opcion;
	return opcion;
}
void MenuUsuario::VerPedidos(SocketClient *cliente){
	string comando = "09";
	string id;
	string respuesta;
	cout << "Introduce el id" << endl;
	cin >> id;
	cliente->enviarMensaje(comando + "|" + id);
	respuesta = cliente->recibirMensaje();
	cout << respuesta << endl;
}
void MenuUsuario::ConfirmarCompra(SocketClient *cliente){
	string respuesta;
	string comando = "06";
	int idUsuario;
	int cantidad;
	int idProd;
	string cantidadProd;
	cout << "Introductr el id" << endl;
	cin >> idUsuario;
	cout << "Cuantos Productos?" << endl;
	cin >> cantidad;
	string texto = comando + "|" + to_string(idUsuario) + "|";
	for (int i = 0; i < cantidad; i++){
		cout << "Introducir el id del Producto" << endl;
		cin >> idProd;
		cout << "Introducir la cantidad" << endl;
		cin >> cantidadProd;
		if(i > 0){
			texto += "#";
		}
		texto += to_string(idProd);
		texto += ",";
		texto += cantidadProd;
	}
	cliente->enviarMensaje(texto);
	respuesta = cliente->recibirMensaje();
	cout << respuesta << endl;
}
void MenuUsuario::AnyadirProductos(SocketClient *cliente){
	string comando = "10";
	string nombre;
	string desc;
	double precio;
	int stock;
	string marca;
	int idCategoria;
	string respuesta;
	cout << "Introduce el nombre del producto" << endl;
	cin >> nombre;
	cout << "Introduce la descripcion" << endl;
	cin >> desc;
	cout << "Introduce el precio" << endl;
	cin >> precio;
	cout << "Introduce el stock" << endl;
	cin >> stock;
	cout << "Introduce la marca" << endl;
	cin >> marca;
	cout << "Introduce el id de la Categoria" << endl;
	cin >> idCategoria;
	cliente->enviarMensaje(comando + "|" + nombre + "|" + desc + "|" + to_string(precio) + "|" + to_string(stock) + "|" + marca + "|" + to_string(idCategoria));
	respuesta = cliente->recibirMensaje();
	cout << respuesta << endl;
}
