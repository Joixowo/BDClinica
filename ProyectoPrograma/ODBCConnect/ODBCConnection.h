#ifndef ODBCCONNECTION_H
#define ODBCCONNECTION_H

#include <windows.h>  // Librería necesaria para funciones del sistema en Windows
#include <sql.h>      // Definiciones básicas del API ODBC
#include <sqlext.h>   // Extensiones del API ODBC
#include <string>     // Para usar std::string

// Clase que maneja la conexión ODBC a la base de datos
class ODBCConnection {
private:
    SQLHENV hEnv;     // Manejador del entorno ODBC
    SQLHDBC hDbc;     // Manejador de la conexión a base de datos
    bool conectado;   // Indica si la conexión está activa

public:
    // Constructor: inicializa los manejadores y el estado de conexión
    ODBCConnection();

    // Destructor: libera los recursos al finalizar
    ~ODBCConnection();

    // Intenta establecer una conexión con la base de datos
    bool conectar(const std::string& dsn_unused, const std::string& usuario, const std::string& contrasena);

    // Cierra la conexión si está abierta
    void desconectar();

    // Devuelve el manejador de conexión activo (para ejecutar sentencias)
    SQLHDBC getConexion();

    // Ejecuta una consulta SQL (INSERT, UPDATE, DELETE)
    bool ejecutar(const std::string& query);
};

#endif
