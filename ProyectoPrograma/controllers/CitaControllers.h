#ifndef CITACONTROLLER_H
#define CITACONTROLLER_H

#include "../Database/Cita.h"                // Modelo de datos para citas
#include "../ODBCConnect/ODBCConnection.h"
#include <vector> // Para devolver múltiples citas
#include <string>// Conexión ODBC a base de datos

class CitaController {
private:
    ODBCConnection* conexion;

public:
    CitaController(ODBCConnection* conn);
    bool agregar(int idPaciente, int idMedico, const std::string& fechaHora, const std::string& motivo);
    void mostrarTodos();
    bool cancelarCita(int idCita);
    bool reprogramarCita(int idCita, const std::string& nuevaFechaHora);

    // Nuevas funciones de búsqueda
    std::vector<Cita> buscarCitasPorNombrePaciente(const std::string& nombrePaciente);
    std::vector<Cita> buscarCitasPorFecha(const std::string& fecha);
};

#endif
