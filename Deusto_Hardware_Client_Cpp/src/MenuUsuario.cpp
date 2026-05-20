#include <iostream>
#include "MenuUsuario.h"
#include "SocketClient.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;
MenuUsuario::MenuUsuario(){
}
MenuUsuario::~MenuUsuario(){

}
char MenuUsuario::MenuInicial(){
	string opcion;

	cout << "Bienvenido a Deusto Hardware" << endl;
	cout << "1. Iniciar Sesion" << endl;
	cout << "2. Registrar Usuario" << endl;
	cout << "3. Salir" << endl;
	getline(cin, opcion);
	return opcion[0];
}
bool MenuUsuario::IniciarSesion(SocketClient *cliente){
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
	if(resCorta == "OK"){
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
	if(resCorta == "OK"){
		return true;
	}else{
		return false;
	}
}
char MenuUsuario::MenuPrincipal(){
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
void MenuUsuario::VerPedidos(SocketClient *cliente){
	string comando = "09";
	string id;
	string respuesta;
	cout << "Introduce el id" << endl;
	getline(cin, id);
	cliente->enviarMensaje(comando + "|" + id);
	respuesta = cliente->recibirMensaje();
	cout << respuesta << endl;
}
void MenuUsuario::ConfirmarCompra(SocketClient *cliente){
	string respuesta;
	string comando = "06";
	string idUsuario;
	string cantidad;
	string idProd;
	string cantidadProd;
	int cantidadInt;
	bool valido = false;
	cout << "Introducte el id" << endl;
	getline(cin, idUsuario);
	while(!valido){
		if (all_of(idUsuario.begin(), idUsuario.end(), ::isdigit)) {
			valido = true;
		} else {
			cout << "Solo se permiten dígitos" << endl;
			cout << "Introducte el id" << endl;
			getline(cin, idUsuario);
		}
	}
	cout << "Cuantos Productos?" << endl;
	getline(cin, cantidad);

	string texto = comando + "|" + idUsuario + "|";
	cantidadInt = stoi(cantidad);
	for (int i = 0; i < cantidadInt; i++){
		valido = false;
		cout << "Introducir el id del Producto" << endl;
		getline(cin, idProd);
		while(!valido){
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
		if(i > 0){
			texto += "#";
		}
		texto += idProd;
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
	string precio;
	string stock;
	string marca;
	string idCategoria;
	string respuesta;
	cout << "Introduce el nombre del producto" << endl;
	getline(cin, nombre);
	cout << "Introduce la descripcion" << endl;
	getline(cin, desc);
	cout << "Introduce el precio" << endl;
	getline(cin, precio);
	cout << "Introduce el stock" << endl;
	getline(cin, stock);
	cout << "Introduce la marca" << endl;
	getline(cin, marca);
	cout << "Introduce el id de la Categoria" << endl;
	getline(cin, idCategoria);
	cliente->enviarMensaje(comando + "|" + nombre + "|" + desc + "|" + precio + "|" + stock + "|" + marca + "|" + idCategoria);
	respuesta = cliente->recibirMensaje();
	cout << respuesta << endl;
}
