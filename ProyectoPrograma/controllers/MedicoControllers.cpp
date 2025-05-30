#include "MedicoControllers.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>





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

std::string centrarTexto3(const std::string& texto, int anchoTotal = 50) {
    int espacios = (anchoTotal - texto.length()) / 2;
    if (espacios < 0) espacios = 0;
    return std::string(espacios, ' ') + texto;
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

    // Encabezado tabulado
    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(25) << "Nombre"
              << std::setw(25) << "Especialidad"
              << std::setw(15) << "Teléfono"
              << "\n" << std::string(70, '-') << "\n";

    // Recorre todos los resultados
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::cout << std::left
                  << std::setw(5) << id
                  << std::setw(25) << nombre
                  << std::setw(25) << especialidad
                  << std::setw(15) << telefono
                  << std::endl;
    }

    // Libera el manejador al finalizar
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
std::vector<Medico> MedicoController::buscarMedicosPorNombre(const std::string& nombreMedico) {
    std::vector<Medico> medicosEncontrados;
    SQLHSTMT hStmt;

    std::string query = "SELECT id, nombre, especialidad, telefono FROM Medicos WHERE nombre LIKE '%" + nombreMedico + "%'";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "❌ Error al ejecutar la consulta SQL en buscarMedicosPorNombre." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return medicosEncontrados;
    }

    int id;
    char nombre[100], especialidad[100], telefono[50];

    SQLBindCol(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, especialidad, sizeof(especialidad), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, telefono, sizeof(telefono), NULL);

    std::cout << "\n" << centrarTexto3("====== Médicos Encontrados ======") << "\n" << std::string(75, '-') << "\n";
    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(25) << "Nombre"
              << std::setw(25) << "Especialidad"
              << std::setw(15) << "Teléfono"
              << "\n" << std::string(75, '-') << "\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        Medico medico;
        medico.setId(id);
        medico.setNombre(nombre);
        medico.setEspecialidad(especialidad);
        medico.setTelefono(telefono);
        medicosEncontrados.push_back(medico);

        std::cout << std::left
                  << std::setw(5) << id
                  << std::setw(25) << nombre
                  << std::setw(25) << especialidad
                  << std::setw(15) << telefono
                  << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return medicosEncontrados;
}
