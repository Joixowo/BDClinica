#include "CitaControllers.h"
#include <iostream>
#include <iostream>
#include <sstream>
#include <iomanip> // Asegúrate de incluir esta librería

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
                                c.motivo
                           FROM Citas c
                           JOIN Pacientes p ON c.idPaciente = p.id
                           JOIN Medicos m ON c.idMedico = m.id)";

    SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
    SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    int idCita, idPaciente, idMedico;
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
                  << std::setw(25) << "Motivo:" << motivo << "\n"
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
  std::string query = "SELECT Citas.id, Citas.idPaciente, LEFT(Citas.fechaHora, 10), SUBSTRING(CONVERT(VARCHAR(20), Citas.fechaHora, 120), 12, 8), Citas.motivo "
                    "FROM Citas "
                    "JOIN Pacientes ON Citas.idPaciente = Pacientes.id "
                    "WHERE Pacientes.nombre LIKE '%" + nombrePaciente + "%'";


  SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
  SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);


  int id_sql, idPaciente_sql;
  char fecha_sql[100], hora_sql[100], motivo_sql[255];


  SQLBindCol(hStmt, 1, SQL_C_LONG, &id_sql, 0, NULL);
  SQLBindCol(hStmt, 2, SQL_C_LONG, &idPaciente_sql, 0, NULL);
  SQLBindCol(hStmt, 3, SQL_C_CHAR, fecha_sql, sizeof(fecha_sql), NULL);
  SQLBindCol(hStmt, 4, SQL_C_CHAR, hora_sql, sizeof(hora_sql), NULL);
  SQLBindCol(hStmt, 5, SQL_C_CHAR, motivo_sql, sizeof(motivo_sql), NULL);


  while (SQLFetch(hStmt) == SQL_SUCCESS) {
   Cita cita;
   cita.setId(id_sql);
   cita.setIdPaciente(idPaciente_sql);
   cita.setFecha(fecha_sql);
   cita.setHora(hora_sql);
   cita.setMotivo(motivo_sql);
   citasEncontradas.push_back(cita);
  }


  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
  return citasEncontradas;
 }


 std::vector<Cita> CitaController::buscarCitasPorFecha(const std::string& fechaBuscar) {
  std::vector<Cita> citasEncontradas;
  SQLHSTMT hStmt;
  std::string query = "SELECT id, idPaciente, LEFT(fechaHora, 10), SUBSTRING(CONVERT(VARCHAR(20), fechaHora, 120), 12, 8), motivo "
                    "FROM Citas "
                    "WHERE LEFT(CONVERT(VARCHAR, fechaHora, 120), 10) LIKE '" + fechaBuscar + "%'";


  std::cout << "Consulta de búsqueda por fecha: " << query << std::endl; // Para depuración


  SQLAllocHandle(SQL_HANDLE_STMT, conexion->getConexion(), &hStmt);
  SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);


  int id_sql, idPaciente_sql;
  char fecha_sql[100], hora_sql[100], motivo_sql[255];


  SQLBindCol(hStmt, 1, SQL_C_LONG, &id_sql, 0, NULL);
  SQLBindCol(hStmt, 2, SQL_C_LONG, &idPaciente_sql, 0, NULL);
  SQLBindCol(hStmt, 3, SQL_C_CHAR, fecha_sql, sizeof(fecha_sql), NULL);
  SQLBindCol(hStmt, 4, SQL_C_CHAR, hora_sql, sizeof(hora_sql), NULL);
  SQLBindCol(hStmt, 5, SQL_C_CHAR, motivo_sql, sizeof(motivo_sql), NULL);


  while (SQLFetch(hStmt) == SQL_SUCCESS) {
   Cita cita;
   cita.setId(id_sql);
   cita.setIdPaciente(idPaciente_sql);
   cita.setFecha(fecha_sql);
   cita.setHora(hora_sql);
   cita.setMotivo(motivo_sql);
   citasEncontradas.push_back(cita);
  }


  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
  return citasEncontradas;
 }