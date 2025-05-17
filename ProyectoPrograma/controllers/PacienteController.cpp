#include "PacienteController.h"
#include <iostream>
#include <iomanip>

// Constructor que recibe un puntero a una conexión ODBC ya abierta
PacienteController::PacienteController(ODBCConnection* conn) : conexion(conn) {}

// Inserta un nuevo paciente en la base de datos
bool PacienteController::agregar(const std::string& nombre, const std::string& apellido,const std::string& telefono,const std::string& fecha_registro) {
    // Construye la consulta SQL con los datos del paciente
    std::string query = "INSERT INTO Pacientes (nombre, apellido, telefono,fecha_registro) VALUES ('" +
                        nombre + "', '" + apellido + "', '" + telefono + "','" + fecha_registro + "')";
    // Ejecuta la consulta y devuelve si fue exitosa o no
    return conexion->ejecutar(query);
}

// Consulta y muestra todos los pacientes de la base de datos
void PacienteController::mostrarTodos() {
    SQLHSTMT hStmt; // Manejador de sentencia

    // Consulta SQL para obtener todos los pacientes
    std::string query = "SELECT id, nombre, apellido, telefono,fecha_registro FROM Pacientes";

    // Asigna el manejador y ejecuta la consulta
    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    // Variables para almacenar resultados
    int id;
    char nombre[100], apellido[100], telefono[50], fecha_registro[100];

    // Vincula cada columna con una variable
    SQLBindCol(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, apellido, sizeof(apellido), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, telefono, sizeof(telefono), NULL);
    SQLBindCol(hStmt, 5, SQL_C_CHAR, fecha_registro, sizeof(fecha_registro), NULL);

    std::cout << "\nLista de Pacientes:\n";
    std::cout << std::left << std::setw(5) << "ID"
              << std::left << std::setw(15) << "Nombre"
              << std::left << std::setw(15) << "Apellido"
              << std::left << std::setw(15) << "Teléfono"
              << std::left << std::setw(20) << "Fecha Registro" << std::endl;
    std::cout << std::setfill('-') << std::setw(70) << "" << std::setfill(' ') << std::endl;

    // Itera sobre los resultados y los imprime
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::cout << std::left << std::setw(5) << id
                  << std::left << std::setw(15) << nombre
                  << std::left << std::setw(15) << apellido
                  << std::left << std::setw(15) << telefono
                  << std::left << std::setw(20) << fecha_registro << std::endl;
    }


    // Libera recursos del manejador
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
