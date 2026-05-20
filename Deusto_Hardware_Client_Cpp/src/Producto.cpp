#include "Producto.h"
#include <iostream>
#include <iomanip>

using namespace std;

Producto::Producto() {
    this->id = 0;
    this->precio = 0.0;
    this->stock = 0;
}

Producto::Producto(int id, const string& nombre, const string& descripcion,
                   double precio, int stock, const string& marca, const string& categoria) {
    this->id = id;
    this->nombre = nombre;
    this->descripcion = descripcion;
    this->precio = precio;
    this->stock = stock;
    this->marca = marca;
    this->categoria = categoria;
}

Producto::~Producto() {
}

int Producto::getId() const {
    return id;
}

string Producto::getNombre() const {
    return nombre;
}

string Producto::getDescripcion() const {
    return descripcion;
}

double Producto::getPrecio() const {
    return precio;
}

int Producto::getStock() const {
    return stock;
}

string Producto::getMarca() const {
    return marca;
}

string Producto::getCategoria() const {
    return categoria;
}

void Producto::mostrar() const {
    cout << "---------------------------------------------" << endl;
    cout << "[" << id << "] " << nombre << endl;
    cout << "    Marca:       " << marca << endl;
    cout << "    Categoria:   " << categoria << endl;
    cout << "    Descripcion: " << descripcion << endl;
    cout << "    Precio:      " << fixed << setprecision(2) << precio << " EUR" << endl;
    cout << "    Stock:       " << stock << " unidades disponibles" << endl;
}
