#include "PacienteController.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor que recibe un puntero a una conexión ODBC ya abierta
std::string centrarTexto2(const std::string& texto, int anchoTotal = 50) {
    int espacios = (anchoTotal - texto.length()) / 2;
    if (espacios < 0) espacios = 0;
    return std::string(espacios, ' ') + texto;
}

PacienteController::PacienteController(ODBCConnection* conn) : conexion(conn) {}

// Inserta un nuevo paciente en la base de datos
bool PacienteController::agregar(const std::string& nombre, const std::string& apellido,const std::string& telefono,const std::string& fecha_registro,const std::string& dpi) {
    // Construye la consulta SQL con los datos del paciente
    // CORRECCIÓN: Añadir 'dpi' a la lista de columnas y el valor de 'dpi' a la lista de VALUES,
    // asegurándose de que también esté entre comillas simples ya que es VARCHAR.
    std::string query = "INSERT INTO Pacientes (nombre, apellido, telefono, fecha_registro, dpi) VALUES ('" +
                        nombre + "', '" + apellido + "', '" + telefono + "','" + fecha_registro + "','" + dpi + "')";
    // Ejecuta la consulta y devuelve si fue exitosa o no
    return conexion->ejecutar(query);
}

// Consulta y muestra todos los pacientes de la base de datos
void PacienteController::mostrarTodos() {
    SQLHSTMT hStmt;

    std::string query = R"(SELECT
                                id,
                                nombre,
                                apellido,
                                telefono,
                                CONVERT(VARCHAR(10), fecha_registro, 120) AS fecha,
                                FORMAT(fecha_registro, 'hh:mm tt') AS hora,
                                dpi
                           FROM Pacientes)";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    int id;
    char nombre[100], apellido[100], telefono[50], fecha[20], hora[20], dpi[20];

    SQLBindCol(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, apellido, sizeof(apellido), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, telefono, sizeof(telefono), NULL);
    SQLBindCol(hStmt, 5, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
    SQLBindCol(hStmt, 6, SQL_C_CHAR, hora, sizeof(hora), NULL);
    SQLBindCol(hStmt, 7, SQL_C_CHAR, dpi, sizeof(dpi), NULL);

    std::cout << "\n" << centrarTexto2("====== Lista de Pacientes ======") << "\n"
              << std::string(100, '-') << "\n";

    std::cout << std::left;

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::cout << std::setw(25) << "ID:" << id << "\n"
                  << std::setw(25) << "Nombre:" << nombre << "\n"
                  << std::setw(25) << "Apellido:" << apellido << "\n"
                  << std::setw(26) << "Teléfono:" << telefono << "\n"
                  << std::setw(25) << "Fecha de Registro:" << fecha << "\n"
                  << std::setw(25) << "Hora de Registro:" << hora << "\n"
                  << std::setw(25) << "DPI:" << dpi << "\n"
                  << std::string(100, '-') << "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// Busca y muestra un paciente por su DPI
void PacienteController::buscarPorDPI(const std::string& dpiBusqueda) {
    SQLHSTMT hStmt;

    std::string query = "SELECT id, nombre, apellido, telefono, "
                        "CONVERT(VARCHAR(10), fecha_registro, 120) AS fecha, "
                        "FORMAT(fecha_registro, 'hh:mm tt') AS hora, dpi "
                        "FROM Pacientes WHERE dpi = '" + dpiBusqueda + "'";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    int id;
    char nombre[100], apellido[100], telefono[50], fecha[20], hora[20], dpi[20];

    SQLBindCol(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, apellido, sizeof(apellido), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, telefono, sizeof(telefono), NULL);
    SQLBindCol(hStmt, 5, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
    SQLBindCol(hStmt, 6, SQL_C_CHAR, hora, sizeof(hora), NULL);
    SQLBindCol(hStmt, 7, SQL_C_CHAR, dpi, sizeof(dpi), NULL);

    bool encontrado = false;

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        encontrado = true;
        std::cout << "\n" << centrarTexto2("====== Paciente Encontrado ======") << "\n"
                  << std::string(100, '-') << "\n"
                  << std::left
                  << std::setw(25) << "ID:" << id << "\n"
                  << std::setw(25) << "Nombre:" << nombre << "\n"
                  << std::setw(25) << "Apellido:" << apellido << "\n"
                  << std::setw(26) << "Teléfono:" << telefono << "\n"
                  << std::setw(25) << "Fecha de Registro:" << fecha << "\n"
                  << std::setw(25) << "Hora de Registro:" << hora << "\n"
                  << std::setw(25) << "DPI:" << dpi << "\n"
                  << std::string(100, '-') << "\n";
    }

    if (!encontrado) {
        std::cout << "\nNo se encontró ningún paciente con el DPI: " << dpiBusqueda << "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
// Asumiendo que tienes un vector de pacientes:
bool PacienteController::dpiExiste(const std::string& dpi) const {
    SQLHSTMT hStmt;
    SQLRETURN ret;

    std::string query = "SELECT COUNT(*) FROM Pacientes WHERE dpi = '" + dpi + "'";

    ret = SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        // Error al crear handle
        return false;
    }

    ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }

    SQLINTEGER count = 0;
    ret = SQLBindCol(hStmt, 1, SQL_C_SLONG, &count, 0, NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }

    if (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return (count > 0);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return false;
}




