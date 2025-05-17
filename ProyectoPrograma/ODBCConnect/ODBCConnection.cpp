#include "ODBCConnection.h"
#include <iostream> // Para mostrar mensajes de error por consola

// Constructor: asigna y configura los manejadores de entorno y conexión
ODBCConnection::ODBCConnection() : conectado(false) {
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv); // Crear manejador de entorno
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); // Versión del ODBC
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc); // Crear manejador de conexión
}

// Destructor: cierra conexión si está activa y libera los manejadores
ODBCConnection::~ODBCConnection() {
    desconectar(); // Cierra la conexión si estaba abierta
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc); // Libera conexión
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv); // Libera entorno
}

// Método para conectar a la base de datos usando cadena de conexión manual
bool ODBCConnection::conectar(const std::string& dsn_unused, const std::string& usuario, const std::string& contrasena) {
    SQLCHAR outstr[1024];         // Para almacenar mensajes de salida
    SQLSMALLINT outstrlen;        // Longitud del mensaje de salida

    // Cadena de conexión completa con parámetros necesarios
    std::string connStr =
        "Driver={ODBC Driver 17 for SQL Server};"
        "Server=rds11g.isbelasoft.com,1433;"            // Nombre del servidor
        "Database=PrograIBG2;"      // Nombre de la base de datos
        "Uid=p1bg2;"                            // Usuario
        "Pwd=Umg123;"              // Contraseña
        "Encrypt=yes;"                       // Encriptar la conexión
        "TrustServerCertificate=yes;";       // Aceptar el certificado del servidor

    // Ejecuta la conexión usando SQLDriverConnect
    SQLRETURN ret = SQLDriverConnect(hDbc, NULL,
        (SQLCHAR*)connStr.c_str(), SQL_NTS,  // Entrada: cadena de conexión
        outstr, sizeof(outstr), &outstrlen,  // Salida: resultado
        SQL_DRIVER_COMPLETE);                // Modo de conexión

    if (SQL_SUCCEEDED(ret)) {
        conectado = true;
        return true; // Conexión exitosa
    } else {
        std::cerr << "❌ Error al conectar con SQL Server usando ODBC Driver 18.\n";
        return false; // Falló la conexión
    }
}

// Método para cerrar la conexión
void ODBCConnection::desconectar() {
    if (conectado) {
        SQLDisconnect(hDbc); // Cierra la conexión activa
        conectado = false;   // Marca como desconectado
    }
}

// Devuelve el manejador actual de conexión, útil para consultas directas
SQLHDBC ODBCConnection::getConexion() {
    return hDbc;
}

// Ejecuta una consulta que no devuelve resultados (INSERT, UPDATE, DELETE)
bool ODBCConnection::ejecutar(const std::string& query) {
    SQLHSTMT hStmt;           // Manejador de sentencia
    SQLRETURN ret;

    // Prepara el manejador de sentencia
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "❌ No se pudo asignar el manejador de sentencia." << std::endl;
        return false;
    }

    // Ejecuta la consulta SQL
    ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "❌ Error al ejecutar la consulta: " << query << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt); // Libera el manejador
    return true; // Consulta ejecutada exitosamente
}
