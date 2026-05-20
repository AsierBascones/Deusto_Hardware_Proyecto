#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>

class Producto {
private:
    int id;
    std::string nombre;
    std::string descripcion;
    double precio;
    int stock;
    std::string marca;
    std::string categoria;

public:
    Producto();
    Producto(int id, const std::string& nombre, const std::string& descripcion,
             double precio, int stock, const std::string& marca, const std::string& categoria);

    virtual ~Producto();

    int getId() const;
    std::string getNombre() const;
    std::string getDescripcion() const;
    double getPrecio() const;
    int getStock() const;
    std::string getMarca() const;
    std::string getCategoria() const;

    void mostrar() const;
};

#endif // PRODUCTO_H
