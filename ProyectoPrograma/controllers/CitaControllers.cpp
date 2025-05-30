#include "CitaControllers.h"
#include "../ODBCConnect/ODBCConnection.h"
#include <iostream>
#include <sstream>
#include <iomanip> // Asegúrate de incluir esta librería
#include <ctime>

// Constructor que recibe el puntero a una conexión ODBC existente
 CitaController::CitaController(ODBCConnection* conn) : conexion(conn) {}


 // Agrega una nueva cita con los datos de paciente, médico, fecha y motivo
 bool CitaController::agregar(int idPaciente, int idMedico, const std::string& fechaHora, const std::string& motivo) {
  // Query SQL de inserción de la cita
  std::string query = "INSERT INTO Citas (idPaciente, idMedico, fechaHora, motivo) VALUES (" +
                      std::to_string(idPaciente) + ", " + std::to_string(idMedico) + ", '" +
                      fechaHora + "', '" + motivo + "')";
  // Ejecuta el query
  return conexion->ejecutar(query);
 }
std::string centrarTexto(const std::string& texto, int anchoTotal = 50) {
  int espacios = (anchoTotal - texto.length()) / 2;
  if (espacios < 0) espacios = 0;
  return std::string(espacios, ' ') + texto;
 }

 // Consulta todas las citas almacenadas
void CitaController::mostrarTodos() {
    SQLHSTMT hStmt;

    std::string query = R"(SELECT
                                c.id,
                                p.id,
                                p.nombre,
                                p.apellido,
                                m.id,
                                m.nombre,
                                CONVERT(VARCHAR(10), c.fechaHora, 120),
                                FORMAT(c.fechaHora, 'hh:mm tt'),
                                c.motivo,
                                c.estado
                           FROM Citas c
                           JOIN Pacientes p ON c.idPaciente = p.id
                           JOIN Medicos m ON c.idMedico = m.id)";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    int idCita, idPaciente, idMedico,estado;
    char nombrePaciente[100], apellidoPaciente[100];
    char nombreMedico[100];
    char fecha[100], hora[100], motivo[255];


    SQLBindCol(hStmt, 1, SQL_C_LONG, &idCita, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_LONG, &idPaciente, 0, NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, nombrePaciente, sizeof(nombrePaciente), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, apellidoPaciente, sizeof(apellidoPaciente), NULL);
    SQLBindCol(hStmt, 5, SQL_C_LONG, &idMedico, 0, NULL);
    SQLBindCol(hStmt, 6, SQL_C_CHAR, nombreMedico, sizeof(nombreMedico), NULL);
    SQLBindCol(hStmt, 7, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
    SQLBindCol(hStmt, 8, SQL_C_CHAR, hora, sizeof(hora), NULL);
    SQLBindCol(hStmt, 9, SQL_C_CHAR, motivo, sizeof(motivo), NULL);
     SQLBindCol(hStmt, 10, SQL_C_LONG, &estado, 0, NULL);

  std::cout << "\n" << centrarTexto("====== Lista de Citas ======") << "\n"<< std::string(50, '-') << "\n";
  std::cout << std::left;

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::cout << std::setw(25) << "ID Cita:" << idCita << "\n"
                  << std::setw(25) << "ID Paciente:" << idPaciente << "\n"
                  << std::setw(25) << "Nombre Paciente:"
                  << std::string(nombrePaciente) + " " + apellidoPaciente << "\n"
                  << std::setw(26) << "ID Médico:" << idMedico << "\n"
                  << std::setw(26) << "Nombre Médico:" << nombreMedico << "\n"
                  << std::setw(25) << "Fecha:" << fecha << "\n"
                  << std::setw(25) << "Hora:" << hora << "\n"
                  << std::setw(25) << "Motivo:" << motivo << "\n";
        std::cout << std::setw(25) << "Estado:" << (estado == 1 ? "Activa" : "Cancelada") << "\n"
                << std::string(50, '-') << "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}



 bool CitaController::cancelarCita(int idCita) {
  std::stringstream ss;
  ss << "UPDATE Citas SET estado = 0 WHERE id = " << idCita; // Asumiendo columna 'estado'
  return conexion->ejecutar(ss.str());
 }


 bool CitaController::reprogramarCita(int idCita, const std::string& nuevaFechaHora) {
  std::stringstream ss;
  ss << "UPDATE Citas SET fechaHora = '" << nuevaFechaHora << "' WHERE id = " << idCita;
  return conexion->ejecutar(ss.str());
 }


std::vector<Cita> CitaController::buscarCitasPorNombrePaciente(const std::string& nombrePaciente) {
    std::vector<Cita> citasEncontradas;
    SQLHSTMT hStmt;

    std::string query = R"(SELECT
                                Citas.id,
                                Pacientes.id,
                                Pacientes.nombre,
                                Pacientes.apellido,
                                Pacientes.dpi,
                                Medicos.nombre,
                                DATENAME(DAY, Citas.fechaHora) + ' ' +
                                DATENAME(MONTH, Citas.fechaHora) + ' ' +
                                DATENAME(YEAR, Citas.fechaHora) AS fecha,
                                FORMAT(Citas.fechaHora, 'hh:mm tt') AS hora,
                                Citas.motivo,
                                Citas.estado
                           FROM Citas
                           JOIN Pacientes ON Citas.idPaciente = Pacientes.id
                           JOIN Medicos ON Citas.idMedico = Medicos.id
                           WHERE Pacientes.nombre LIKE '%)" + nombrePaciente + "%'";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Error al ejecutar la consulta SQL en buscarCitasPorNombrePaciente." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return citasEncontradas;
    }

    int id_sql, idPaciente_sql,estado;
    char nombrePaciente_sql[100], apellidoPaciente_sql[100], dpi_sql[100];
    char nombreMedico_sql[100], fecha_sql[100], hora_sql[100], motivo_sql[255];

    SQLBindCol(hStmt, 1, SQL_C_LONG, &id_sql, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_LONG, &idPaciente_sql, 0, NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, nombrePaciente_sql, sizeof(nombrePaciente_sql), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, apellidoPaciente_sql, sizeof(apellidoPaciente_sql), NULL);
    SQLBindCol(hStmt, 5, SQL_C_CHAR, dpi_sql, sizeof(dpi_sql), NULL);
    SQLBindCol(hStmt, 6, SQL_C_CHAR, nombreMedico_sql, sizeof(nombreMedico_sql), NULL);
    SQLBindCol(hStmt, 7, SQL_C_CHAR, fecha_sql, sizeof(fecha_sql), NULL);
    SQLBindCol(hStmt, 8, SQL_C_CHAR, hora_sql, sizeof(hora_sql), NULL);
    SQLBindCol(hStmt, 9, SQL_C_CHAR, motivo_sql, sizeof(motivo_sql), NULL);
     SQLBindCol(hStmt, 10, SQL_C_LONG, &estado, 0, NULL);



    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        Cita cita;
        cita.setId(id_sql);
        cita.setIdPaciente(idPaciente_sql);
        cita.setFecha(fecha_sql);
        cita.setHora(hora_sql);
        cita.setMotivo(motivo_sql);
        citasEncontradas.push_back(cita);

        std::cout << "\n" << centrarTexto("====== Lista de Citas por Nombre ======") << "\n"<< std::string(50, '-') << "\n";
        std::cout << std::left;

        std::cout << "\n"
                  << "ID de la cita:        " << cita.getId() << "\n"
                  << "ID del paciente:      " << cita.getIdPaciente() << "\n"
                  << "Paciente:             " << nombrePaciente_sql << " " << apellidoPaciente_sql << "\n"
                  << "DPI:                  " << dpi_sql << "\n"
                  << "Médico:               " << nombreMedico_sql << "\n"
                  << "Fecha:                " << cita.getFecha() << "\n"
                  << "Hora:                 " << cita.getHora() << "\n"
                  << "Motivo:               " << cita.getMotivo() << "\n";
        std::cout << "Estado:               " << (estado == 1 ? "Activa" : "Cancelada") << "\n"
                  << std::string(50, '-') << "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return citasEncontradas;
}




std::vector<Cita> CitaController::buscarCitasPorFecha(const std::string& fechaBuscar) {
    std::vector<Cita> citasEncontradas;
    SQLHSTMT hStmt;

     std::string query = R"(SELECT
                            Citas.id,
                            Pacientes.id,
                            Pacientes.nombre,
                            Pacientes.apellido,
                            Pacientes.dpi,
                            Medicos.nombre,
                            DATENAME(DAY, Citas.fechaHora) + ' ' +
                            DATENAME(MONTH, Citas.fechaHora) + ' ' +
                            DATENAME(YEAR, Citas.fechaHora) AS fecha,
                            FORMAT(Citas.fechaHora, 'hh:mm tt') AS hora,
                            Citas.motivo,
                            Citas.estado
                       FROM Citas
                       JOIN Pacientes ON Citas.idPaciente = Pacientes.id
                       JOIN Medicos ON Citas.idMedico = Medicos.id
                       WHERE CONVERT(VARCHAR(10), Citas.fechaHora, 120) LIKE ')" + fechaBuscar + "%'";


    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Error al ejecutar la consulta SQL en buscarCitasPorFecha." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return citasEncontradas;
    }

    int id_sql, idPaciente_sql,estado;
    char nombrePaciente_sql[100], apellidoPaciente_sql[100], dpi_sql[100];
    char nombreMedico_sql[100], fecha_sql[100], hora_sql[100], motivo_sql[255];

    SQLBindCol(hStmt, 1, SQL_C_LONG, &id_sql, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_LONG, &idPaciente_sql, 0, NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, nombrePaciente_sql, sizeof(nombrePaciente_sql), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, apellidoPaciente_sql, sizeof(apellidoPaciente_sql), NULL);
    SQLBindCol(hStmt, 5, SQL_C_CHAR, dpi_sql, sizeof(dpi_sql), NULL);
    SQLBindCol(hStmt, 6, SQL_C_CHAR, nombreMedico_sql, sizeof(nombreMedico_sql), NULL);
    SQLBindCol(hStmt, 7, SQL_C_CHAR, fecha_sql, sizeof(fecha_sql), NULL);
    SQLBindCol(hStmt, 8, SQL_C_CHAR, hora_sql, sizeof(hora_sql), NULL);
    SQLBindCol(hStmt, 9, SQL_C_CHAR, motivo_sql, sizeof(motivo_sql), NULL);
     SQLBindCol(hStmt, 10, SQL_C_LONG, &estado, 0, NULL);


    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        Cita cita;
        cita.setId(id_sql);
        cita.setIdPaciente(idPaciente_sql);
        cita.setFecha(fecha_sql);
        cita.setHora(hora_sql);
        cita.setMotivo(motivo_sql);
        citasEncontradas.push_back(cita);

        std::cout << "\n" << centrarTexto("====== Lista de Citas por Nombre Fecha ======") << "\n"<< std::string(50, '-') << "\n";
        std::cout << std::left;

        std::cout << "\n"
                  << "ID de la cita:        " << cita.getId() << "\n"
                  << "ID del paciente:      " << cita.getIdPaciente() << "\n"
                  << "Paciente:             " << nombrePaciente_sql << " " << apellidoPaciente_sql << "\n"
                  << "DPI:                  " << dpi_sql << "\n"
                  << "Médico:               " << nombreMedico_sql << "\n"
                  << "Fecha:                " << cita.getFecha() << "\n"
                  << "Hora:                 " << cita.getHora() << "\n"
                  << "Motivo:               " << cita.getMotivo() << "\n";
        std::cout << "Estado:               " << (estado == 1 ? "Activa" : "Cancelada") << "\n"
                  << std::string(50, '-') << "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return citasEncontradas;
}

std::vector<Cita> CitaController::buscarCitasPorDPI(const std::string& dpiBuscar) {
    std::vector<Cita> citasEncontradas;
    SQLHSTMT hStmt;

    std::string query = R"(SELECT
                                C.id,
                                P.id,
                                P.nombre,
                                P.apellido,
                                P.dpi,
                                M.nombre,
                                DATENAME(DAY, C.fechaHora) + ' ' +
                                DATENAME(MONTH, C.fechaHora) + ' ' +
                                DATENAME(YEAR, C.fechaHora) AS fecha,
                                FORMAT(C.fechaHora, 'hh:mm tt') AS hora,
                                C.motivo,
                                C.estado
                           FROM Citas C
                           JOIN Pacientes P ON C.idPaciente = P.id
                           JOIN Medicos M ON C.idMedico = M.id
                           WHERE P.dpi = ')" + dpiBuscar + "'";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "❌ Error al ejecutar la consulta SQL en buscarCitasPorDPI." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return citasEncontradas;
    }

    int idCita, idPaciente,estado;
    char nombrePaciente[100], apellidoPaciente[100], dpi[100];
    char nombreMedico[100], fecha[100], hora[100], motivo[255];

    SQLBindCol(hStmt, 1, SQL_C_LONG, &idCita, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_LONG, &idPaciente, 0, NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, nombrePaciente, sizeof(nombrePaciente), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, apellidoPaciente, sizeof(apellidoPaciente), NULL);
    SQLBindCol(hStmt, 5, SQL_C_CHAR, dpi, sizeof(dpi), NULL);
    SQLBindCol(hStmt, 6, SQL_C_CHAR, nombreMedico, sizeof(nombreMedico), NULL);
    SQLBindCol(hStmt, 7, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
    SQLBindCol(hStmt, 8, SQL_C_CHAR, hora, sizeof(hora), NULL);
    SQLBindCol(hStmt, 9, SQL_C_CHAR, motivo, sizeof(motivo), NULL);
     SQLBindCol(hStmt, 10, SQL_C_LONG, &estado, 0, NULL);

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        Cita cita;
        cita.setId(idCita);
        cita.setIdPaciente(idPaciente);
        cita.setFecha(fecha);
        cita.setHora(hora);
        cita.setMotivo(motivo);
        citasEncontradas.push_back(cita);

        std::cout << "\n" << centrarTexto("====== Lista de Citas por Nombre DPI ======") << "\n"<< std::string(50, '-') << "\n";
        std::cout << std::left;

        std::cout << "\n"
                  << "ID de la cita:        " << cita.getId() << "\n"
                  << "ID del paciente:      " << cita.getIdPaciente() << "\n"
                  << "Paciente:             " << nombrePaciente << " " << apellidoPaciente << "\n"
                  << "DPI:                  " << dpi << "\n"
                  << "Médico:               " << nombreMedico << "\n"
                  << "Fecha:                " << cita.getFecha() << "\n"
                  << "Hora:                 " << cita.getHora() << "\n"
                  << "Motivo:               " << cita.getMotivo() << "\n";
        std::cout << "Estado:               " << (estado == 1 ? "Activa" : "Cancelada") << "\n"
                  << std::string(50, '-') << "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return citasEncontradas;
}
void CitaController::mostrarPorEstado(bool activas) {
    SQLHSTMT hStmt;

    std::string query = R"(SELECT
                                c.id,
                                p.id,
                                p.nombre,
                                p.apellido,
                                m.id,
                                m.nombre,
                                CONVERT(VARCHAR(10), c.fechaHora, 120),
                                FORMAT(c.fechaHora, 'hh:mm tt'),
                                c.motivo,
                                c.estado
                           FROM Citas c
                           JOIN Pacientes p ON c.idPaciente = p.id
                           JOIN Medicos m ON c.idMedico = m.id
                           WHERE c.estado = )" + std::to_string(activas ? 1 : 0);

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    int idCita, idPaciente, idMedico, estado;
    char nombrePaciente[100], apellidoPaciente[100];
    char nombreMedico[100];
    char fecha[100], hora[100], motivo[255];

    SQLBindCol(hStmt, 1, SQL_C_LONG, &idCita, 0, NULL);
    SQLBindCol(hStmt, 2, SQL_C_LONG, &idPaciente, 0, NULL);
    SQLBindCol(hStmt, 3, SQL_C_CHAR, nombrePaciente, sizeof(nombrePaciente), NULL);
    SQLBindCol(hStmt, 4, SQL_C_CHAR, apellidoPaciente, sizeof(apellidoPaciente), NULL);
    SQLBindCol(hStmt, 5, SQL_C_LONG, &idMedico, 0, NULL);
    SQLBindCol(hStmt, 6, SQL_C_CHAR, nombreMedico, sizeof(nombreMedico), NULL);
    SQLBindCol(hStmt, 7, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
    SQLBindCol(hStmt, 8, SQL_C_CHAR, hora, sizeof(hora), NULL);
    SQLBindCol(hStmt, 9, SQL_C_CHAR, motivo, sizeof(motivo), NULL);
    SQLBindCol(hStmt, 10, SQL_C_LONG, &estado, 0, NULL);

    std::cout << "\n" << centrarTexto(activas ? "== Citas Activas ==" : "== Citas Canceladas ==") << "\n"
              << std::string(50, '-') << "\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::cout << std::setw(25) << "ID Cita:" << idCita << "\n"
                  << std::setw(25) << "ID Paciente:" << idPaciente << "\n"
                  << std::setw(25) << "Nombre Paciente:"
                  << std::string(nombrePaciente) + " " + apellidoPaciente << "\n"
                  << std::setw(26) << "ID Médico:" << idMedico << "\n"
                  << std::setw(26) << "Nombre Médico:" << nombreMedico << "\n"
                  << std::setw(25) << "Fecha:" << fecha << "\n"
                  << std::setw(25) << "Hora:" << hora << "\n"
                  << std::setw(25) << "Motivo:" << motivo << "\n"
                  << std::setw(25) << "Estado:" << (estado == 1 ? "Activa" : "Cancelada") << "\n"
                  << std::string(50, '-') << "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}


bool fechaHoraEsValida(const std::string& fechaHora) {
     std::tm tm = {};
     std::istringstream ss(fechaHora);
     ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

     if (ss.fail()) return false;  // Formato incorrecto

     std::tm original = tm;
     std::mktime(&tm);

     return (
         original.tm_year == tm.tm_year &&
         original.tm_mon == tm.tm_mon &&
         original.tm_mday == tm.tm_mday &&
         original.tm_hour == tm.tm_hour &&
         original.tm_min == tm.tm_min &&
         original.tm_sec == tm.tm_sec
     );
 }




