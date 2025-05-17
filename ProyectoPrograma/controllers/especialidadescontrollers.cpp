#include "especialidadescontrollers.h"
#include <vector>
#include <iomanip> // Para std::setw

// Constructor: asigna la conexión ODBC proporcionada a la variable 'conexion'
EspecialidadesController::EspecialidadesController(ODBCConnection* conn) : conexion(conn) {}

// Metodo para agregar una especialidad
bool EspecialidadesController::agregar(const std::string& nombre, const std::string& precio_consulta) {
    // Construcción de la consulta SQL para insertar una nueva especialidad
    std::string query = "INSERT INTO Especialidades (nombre, precio_consulta) VALUES ('" + nombre + "', '" + precio_consulta + "')";
    // Ejecuta la consulta usando el metodo 'ejecutar' de la conexión
    return conexion->ejecutar(query);
}

// Metodo para mostrar todas las especialidades
void EspecialidadesController::mostrarTodos() {
    SQLHSTMT hStmt; // Manejador para la sentencia SQL
    // Consulta SQL para seleccionar todas las especialidades
    std::string query = "SELECT id, nombre, precio_consulta FROM Especialidades";

    // Se asigna y obtiene el manejador para la sentencia
    if (SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt) != SQL_SUCCESS) {
        // Si falla, muestra un error y termina
        std::cerr << "Error al asignar manejador para sentencia." << std::endl;
        return;
    }

    // Ejecuta la consulta SQL
    if (SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        // Si falla, muestra un error, libera recursos y termina
        std::cerr << "Error al ejecutar consulta." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    // Variables para almacenar los datos que se obtienen de cada fila
    int id;                         // id de la especialidad
    char nombre[100];
    char precio_consulta[20]; // Aumenta el tamaño para evitar desbordamientos

    // Enlaza los resultados de la consulta con las variables
    SQLBindCol(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, precio_consulta, sizeof(precio_consulta), NULL);

    // Define el ancho de las columnas para la impresión
    const int idAncho = 5;
    const int nombreAncho = 30;
    const int precioAncho = 10;

    // Muestra el encabezado alineado
    std::cout << "\n📋 Lista de Especialidades:\n";
    std::cout << std::left << std::setw(idAncho) << "ID"
              << std::left << std::setw(nombreAncho) << "Nombre"
              << std::left << std::setw(precioAncho) << "Precio" << std::endl;
    std::cout << std::setfill('-') << std::setw(idAncho + nombreAncho + precioAncho) << "" << std::setfill(' ') << std::endl;

    // Bucle para recorrer cada fila de resultados
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        // Para cada fila, muestra los datos alineados en consola
        std::cout << std::left << std::setw(idAncho) << id
                  << std::left << std::setw(nombreAncho) << nombre
                  << std::left << std::setw(precioAncho) << precio_consulta << std::endl;
    }

    // Libera el manejador de la sentencia una vez terminado
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// Método para eliminar una especialidad por id
bool EspecialidadesController::eliminar(int id) {
    // Construcción de consulta SQL para eliminar una fila específica por id
    std::string query = "DELETE FROM Especialidades WHERE id = " + std::to_string(id);
    // Ejecuta la sentencia
    return conexion->ejecutar(query);
}

// Método para actualizar el nombre de una especialidad por id
bool EspecialidadesController::actualizar(int id, const std::string& nuevoNombre, const std::string& precio_consulta) {
    // Construcción de la consulta SQL para actualizar el nombre
    std::string query = "UPDATE Especialidades SET nombre = '" + nuevoNombre + "', precio_consulta = '" + precio_consulta + "' WHERE id = " + std::to_string(id);
    // Ejecuta la consulta
    return conexion->ejecutar(query);
}
std::vector<Especialidades> EspecialidadesController::obtenerTodas() {
    std::vector<Especialidades> lista;

    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLCHAR query[] = "SELECT id, nombre, precio_consulta FROM Especialidades";

    if (SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &stmt) == SQL_SUCCESS) {
        ret = SQLExecDirect(stmt, query, SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int id;
            char nombre[100];
            char precio[50];

            while (SQLFetch(stmt) == SQL_SUCCESS) {
                SQLGetData(stmt, 1, SQL_C_LONG, &id, 0, nullptr);
                SQLGetData(stmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), nullptr);
                SQLGetData(stmt, 3, SQL_C_CHAR, precio, sizeof(precio), nullptr);

                lista.emplace_back(id, nombre, precio);
            }
        }
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    }

    return lista;
}