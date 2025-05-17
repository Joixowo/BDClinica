#include "MedicoControllers.h"
#include <iostream>

// Constructor que recibe una conexión activa a la base de datos
MedicoController::MedicoController(ODBCConnection* conn) : conexion(conn) {}

// Inserta un nuevo médico en la tabla Medicos
bool MedicoController::agregar(const std::string& nombre, const std::string& especialidad, const std::string& telefono) {
    // Construcción del query de inserción
    std::string query = "INSERT INTO Medicos (nombre, especialidad, telefono) VALUES ('" +
                        nombre + "', '" + especialidad + "', '" + telefono + "')";
    // Ejecuta la consulta y retorna el resultado
    return conexion->ejecutar(query);
}

// Consulta y muestra todos los médicos registrados
void MedicoController::mostrarTodos() {
    SQLHSTMT hStmt; // Manejador de sentencia SQL
    std::string query = "SELECT id, nombre, especialidad, telefono FROM Medicos";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    // Variables que almacenan temporalmente los datos
    int id;
    char nombre[100], especialidad[100], telefono[50];

    // Asociar columnas a variables
    SQLBindCol(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, especialidad, sizeof(especialidad), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, telefono, sizeof(telefono), NULL);

    std::cout << "\n📋 Lista de Médicos:\n";

    // Recorre todos los resultados
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::cout << "ID: " << id
                  << ", Nombre: " << nombre
                  << ", Especialidad: " << especialidad
                  << ", Teléfono: " << telefono << std::endl;
    }

    // Libera el manejador al finalizar
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
