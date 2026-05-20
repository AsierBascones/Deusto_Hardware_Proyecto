#include <stdio.h>
#include "db_manager.h"
#include "sqlite3.h"

void inicializar_base_datos(sqlite3 *db) {
    char *mensaje_error = 0;
    const char *sql_tablas =
        "CREATE TABLE IF NOT EXISTS PAIS ("
        "  id_pais INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  nombre TEXT NOT NULL UNIQUE"
        ");"

        "CREATE TABLE IF NOT EXISTS PROVINCIA ("
        "  id_provincia INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  nombre TEXT NOT NULL, "
        "  id_pais INTEGER,"
        "  FOREIGN KEY (id_pais) REFERENCES PAIS (id_pais),"
        "  UNIQUE(nombre, id_pais)"
        ");"

        "CREATE TABLE IF NOT EXISTS CIUDAD ( "
        "  id_ciudad INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  nombre TEXT NOT NULL, "
        "  codigo_postal TEXT, "
        "  id_provincia INTEGER,"
        "  FOREIGN KEY (id_provincia) REFERENCES PROVINCIA (id_provincia),"
        "  UNIQUE(nombre, id_provincia)"
        ");"

        "CREATE TABLE IF NOT EXISTS CATEGORIA( "
        "  id_categoria INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  nombre TEXT NOT NULL UNIQUE, "
        "  descripcion TEXT "
        ");"

        "CREATE TABLE IF NOT EXISTS PROVEEDOR( "
        "  id_proveedor INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  nombre TEXT NOT NULL, "
        "  telefono TEXT, "
        "  email TEXT UNIQUE, "
        "  direccion TEXT, "
        "  id_ciudad INTEGER,"
        "  FOREIGN KEY (id_ciudad) REFERENCES CIUDAD (id_ciudad)"
        ");"

        "CREATE TABLE IF NOT EXISTS USUARIO ("
        "  id_usuario INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  nombre TEXT NOT NULL, "
        "  apellidos TEXT, "
        "  email TEXT UNIQUE, "
        "  contrasena TEXT NOT NULL, "
        "  rol TEXT, "
        "  id_ciudad INTEGER,"
        "  FOREIGN KEY (id_ciudad) REFERENCES CIUDAD (id_ciudad)"
        ");"

        "CREATE TABLE IF NOT EXISTS PRODUCTO ("
        "  id_producto INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  nombre TEXT NOT NULL UNIQUE, "
        "  descripcion TEXT, "
        "  precio REAL, "
        "  stock INTEGER, "
        "  marca TEXT, "
        "  id_categoria INTEGER, "
        "  id_proveedor INTEGER,"
        "  FOREIGN KEY (id_categoria) REFERENCES CATEGORIA(id_categoria),"
        "  FOREIGN KEY (id_proveedor) REFERENCES PROVEEDOR(id_proveedor)"
        ");"

        "CREATE TABLE IF NOT EXISTS CARRITO ("
        "  id_carrito INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  fecha_creacion DATE NOT NULL,"
        "  estado_compra TEXT,"
        "  id_usuario INTEGER,"
        "  FOREIGN KEY (id_usuario) REFERENCES USUARIO(id_usuario)"
        ");"

        "CREATE TABLE IF NOT EXISTS LINEA_CARRITO ("
        "  id_carrito INTEGER,"
        "  id_producto INTEGER,"
        "  cantidad INTEGER,"
        "  precio_unitario REAL,"
        "  PRIMARY KEY (id_carrito, id_producto),"
        "  FOREIGN KEY (id_carrito) REFERENCES CARRITO(id_carrito),"
        "  FOREIGN KEY (id_producto) REFERENCES PRODUCTO(id_producto)"
        ");"

        "CREATE TABLE IF NOT EXISTS PEDIDO ("
        "  id_pedido INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  fecha_envio DATE,"
        "  total REAL,"
        "  id_carrito INTEGER,"
        "  FOREIGN KEY (id_carrito) REFERENCES CARRITO(id_carrito)"
        ");"

        "CREATE TABLE IF NOT EXISTS RESENA ("
        "  id_resena INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  id_usuario INTEGER,"
        "  puntuacion REAL,"
        "  comentario TEXT,"
        "  fecha DATE,"
        "  FOREIGN KEY (id_usuario) REFERENCES USUARIO(id_usuario)"
        ");";

    int resultado = sqlite3_exec(db, sql_tablas, 0, 0, &mensaje_error);

    if (resultado != SQLITE_OK) {
        printf("ERROR al inicializar las tablas: %s\n", mensaje_error);
        sqlite3_free(mensaje_error);
    } else {
        printf("OK: Base de datos estructurada y lista para operar.\n");
    }
}

void rellenar_base_datos(sqlite3 *db) {
    char *mensaje = 0;

    const char *inserts =
            // PAISES
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(1, 'ESPAÑA');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(2, 'FRANCIA');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(3, 'PORTUGAL');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(4, 'ALEMANIA');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(5, 'ITALIA');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(6, 'REINO UNIDO');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(7, 'JAPON');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(8, 'ESTADOS UNIDOS');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(9, 'CHINA');"
            "INSERT OR IGNORE INTO PAIS (id_pais, nombre) VALUES(10, 'COREA DEL SUR');"

            // PROVINCIAS
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(1, 'ILE DE FRANCE', 2);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(2, 'OPORTO', 3);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(3, 'GALICIA', 1);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(4, 'MADRID', 1);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(5, 'BARCELONA', 1);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(6, 'BAVIERA', 4);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(7, 'LOMBARDIA', 5);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(8, 'TOKIO', 7);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(9, 'CALIFORNIA', 8);"
            "INSERT OR IGNORE INTO PROVINCIA (id_provincia, nombre, id_pais) VALUES(10, 'SEUL', 10);"

            // CIUDADES
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(1, 'PARIS', '37812', 1);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(2, 'OPORTO', '72732', 2);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(3, 'LUGO', '89274', 3);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(4, 'MADRID', '28001', 4);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(5, 'BARCELONA', '08001', 5);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(6, 'MUNICH', '80331', 6);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(7, 'MILAN', '20121', 7);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(8, 'SHINJUKU', '1600000', 8);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(9, 'SAN JOSE', '95101', 9);"
            "INSERT OR IGNORE INTO CIUDAD (id_ciudad, nombre, codigo_postal, id_provincia) VALUES(10, 'GANGNAM', '06000', 10);"

            // CATEGORIAS
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(1, 'PERIFERICOS', 'Teclados, ratones, cascos');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(2, 'CONSOLAS', 'Hardware de entretenimiento');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(3, 'PC', 'Sobremesas y portatiles');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(4, 'COMPONENTES', 'CPU, GPU, Placas base');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(5, 'MONITORES', 'Pantallas gaming y oficina');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(6, 'ALMACENAMIENTO', 'SSD, HDD, NVMe');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(7, 'MEMORIA RAM', 'Módulos DDR4 y DDR5');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(8, 'REFRIGERACION', 'Ventiladores y liquidas');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(9, 'SILLAS GAMING', 'Mobiliario ergonomico');"
            "INSERT OR IGNORE INTO CATEGORIA (id_categoria, nombre, descripcion) VALUES(10, 'REDES', 'Routers, switches, cables');"

            // PROVEEDORES
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(1, 'MANOLO', '974967799', 'manolo.uber@hardware.es', 'Av conchinchina', 1);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(2, 'JUAN', '974967779', 'juan.uber@hardware.es', 'Av conchinchina', 2);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(3, 'PEPE', '974967789', 'pepe.uber@hardware.es', 'Av conchinchina', 3);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(4, 'ASUS IBERICA', '911222333', 'ventas@asus.es', 'Calle Informática 5', 4);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(5, 'CORSAIR EU', '888444555', 'b2b@corsair.com', 'Munich Tech Park', 6);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(6, 'RAZER ASIA', '555123999', 'supply@razer.jp', 'Shinjuku Tower', 8);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(7, 'SAMSUNG DIST', '900888777', 'ssd@samsung.kr', 'Gangnam Blvd 12', 10);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(8, 'INTEL GLOBAL', '100200300', 'chips@intel.us', 'Silicon Valley 1', 9);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(9, 'LOGITECH EUROPA', '666333222', 'dist@logitech.it', 'Via Roma 10', 7);"
            "INSERT OR IGNORE INTO PROVEEDOR (id_proveedor, nombre, telefono, email, direccion, id_ciudad) VALUES(10, 'MSI SPAIN', '933444555', 'bcn@msi.es', 'Ronda Litoral', 5);"

            // USUARIOS
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(1, 'MANOLO', 'GOMEZ PEREZ', 'manolo123@gmail.com', 'megust4nloscocos', 'CLIENTE', 1);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(2, 'JORGE', 'RAMIREZ GARCIA', 'jaimito324@opendeusto.es', 'FOR3VERDEUSTO!', 'CLIENTE', 2);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(3, 'ANTUAN', 'FERNANDEZ BERMEJO', 'antuan@gmail.com', 'olala', 'CLIENTE', 3);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(4, 'SARA', 'LOPEZ', 'sara.l@gmail.com', '1234', 'CLIENTE', 4);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(5, 'ADMIN', 'SISTEMAS', 'admin@hardware.es', 'admin123', 'ADMIN', 4);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(6, 'CARLOS', 'RUIZ', 'cruiz@yahoo.es', 'password', 'CLIENTE', 5);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(7, 'ELENA', 'MARTIN', 'elena99@hotmail.com', 'ele123', 'CLIENTE', 6);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(8, 'DAVID', 'GARCIA', 'dgarcia@gmail.com', 'qwerty', 'CLIENTE', 7);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(9, 'LUCIA', 'SANCHEZ', 'lucia.s@gmail.com', 'zxcvbn', 'CLIENTE', 8);"
            "INSERT OR IGNORE INTO USUARIO (id_usuario, nombre, apellidos, email, contrasena, rol, id_ciudad) VALUES(10, 'PABLO', 'DIAZ', 'pdiaz@empresa.com', 'segura99', 'CLIENTE', 9);"

            // PRODUCTOS
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(1, 'RATON GAMER', 'Sensor 16K DPI', 49.99, 13, 'LOGITECH', 1, 1);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(2, 'NINTENDO WII', 'Consola retro', 499.99, 15, 'NINTENDO', 2, 2);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(3, 'PC CON LEDS', 'Torre gaming RGB', 750.00, 3, 'AMD', 3, 3);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(4, 'RTX 4090', 'GPU 24GB GDDR6X', 1899.99, 5, 'ASUS', 4, 4);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(5, 'TECLADO K70', 'Mecanico Cherry MX', 129.50, 20, 'CORSAIR', 1, 5);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(6, 'MONITOR 144Hz', 'Panel IPS 27 pulgadas', 249.00, 10, 'MSI', 5, 10);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(7, 'SSD 2TB 990 PRO', 'NVMe PCIe 4.0', 180.99, 45, 'SAMSUNG', 6, 7);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(8, 'KRAKEN Z73', 'Liquida 360mm con LCD', 210.00, 8, 'NZXT', 8, 8);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(9, 'SECRETLA TITAN', 'Silla gaming tela', 450.00, 12, 'SECRETLA', 9, 6);"
            "INSERT OR IGNORE INTO PRODUCTO (id_producto, nombre, descripcion, precio, stock, marca, id_categoria, id_proveedor) VALUES(10, 'DDR5 32GB', 'RAM 6000MHz CL30', 145.00, 30, 'CORSAIR', 7, 5);"

            // CARRITOS
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(1, '2026-05-20', 'CONFIRMADO', 1);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(2, '2026-05-18', 'CONFIRMADO', 2);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(3, '2026-05-19', 'CONFIRMADO', 3);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(4, '2026-05-20', 'EN_PROCESO', 4);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(5, '2026-05-10', 'CONFIRMADO', 6);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(6, '2026-05-12', 'ABANDONADO', 7);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(7, '2026-05-15', 'CONFIRMADO', 8);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(8, '2026-05-16', 'CONFIRMADO', 9);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(9, '2026-05-20', 'EN_PROCESO', 10);"
            "INSERT OR IGNORE INTO CARRITO (id_carrito, fecha_creacion, estado_compra, id_usuario) VALUES(10, '2026-05-21', 'CONFIRMADO', 1);"

            // LINEAS DE CARRITO
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(1, 1, 2, 49.99);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(1, 3, 1, 750.00);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(2, 2, 1, 499.99);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(3, 4, 1, 1899.99);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(4, 5, 1, 129.50);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(5, 6, 2, 249.00);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(6, 7, 1, 180.99);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(7, 8, 1, 210.00);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(8, 9, 1, 450.00);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(9, 10, 2, 145.00);"
            "INSERT OR IGNORE INTO LINEA_CARRITO (id_carrito, id_producto, cantidad, precio_unitario) VALUES(10, 1, 1, 49.99);"

            // PEDIDOS
            "INSERT OR IGNORE INTO PEDIDO (id_pedido, fecha_envio, total, id_carrito) VALUES(1, '2026-05-21', 849.98, 1);"
            "INSERT OR IGNORE INTO PEDIDO (id_pedido, fecha_envio, total, id_carrito) VALUES(2, '2026-05-19', 499.99, 2);"
            "INSERT OR IGNORE INTO PEDIDO (id_pedido, fecha_envio, total, id_carrito) VALUES(3, '2026-05-20', 1899.99, 3);"
            "INSERT OR IGNORE INTO PEDIDO (id_pedido, fecha_envio, total, id_carrito) VALUES(4, '2026-05-11', 498.00, 5);"
            "INSERT OR IGNORE INTO PEDIDO (id_pedido, fecha_envio, total, id_carrito) VALUES(5, '2026-05-16', 210.00, 7);"
            "INSERT OR IGNORE INTO PEDIDO (id_pedido, fecha_envio, total, id_carrito) VALUES(6, '2026-05-17', 450.00, 8);"
            "INSERT OR IGNORE INTO PEDIDO (id_pedido, fecha_envio, total, id_carrito) VALUES(7, '2026-05-22', 49.99, 10);"

            // RESEÑAS
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(1, 1, 4.5, 'Muy buen raton', '2026-05-21');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(2, 2, 5.0, 'Me encanta la consola', '2026-05-22');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(3, 3, 5.0, 'Potencia increible la grafica', '2026-05-21');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(4, 4, 3.5, 'El teclado hace mucho ruido', '2026-05-20');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(5, 6, 4.0, 'Colores del monitor perfectos', '2026-05-15');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(6, 7, 5.0, 'SSD rapidisimo', '2026-05-14');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(7, 8, 4.0, 'Mantiene el PC fresco', '2026-05-17');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(8, 9, 4.5, 'Silla super comoda', '2026-05-18');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(9, 10, 5.0, 'Mucha memoria y barata', '2026-05-21');"
            "INSERT OR IGNORE INTO RESENA (id_resena, id_usuario, puntuacion, comentario, fecha) VALUES(10, 1, 2.0, 'Se rompio el cable', '2026-05-22');";

    int resultado = sqlite3_exec(db, inserts, 0, 0, &mensaje);

    if (resultado != SQLITE_OK) {
        printf("Error al insertar datos en la base de datos: %s\n", mensaje);
        sqlite3_free(mensaje);
    } else {
        printf("Inserts completados\n");
    }
}
