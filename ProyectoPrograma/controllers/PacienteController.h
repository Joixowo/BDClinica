#ifndef PACIENTECONTROLLER_H
#define PACIENTECONTROLLER_H

#include "../Database/Paciente.h"            // Incluye la definición de la clase Paciente
#include "../ODBCConnect/ODBCConnection.h"   // Para usar la conexión a base de datos

class PacienteController {
private:
    ODBCConnection* conexion; // Puntero a la conexión compartida con la base de datos

public:
    // Constructor que recibe una conexión ya establecida
    PacienteController(ODBCConnection* conn);

    // Inserta un nuevo paciente usando nombre, apellido y teléfono
    bool agregar(const std::string& nombre, const std::string& apellido, const std::string& telefono,const std::string& fecha_registro, const std::string& dpi);

    // Muestra todos los pacientes registrados en la base de datos
    void mostrarTodos();

    void buscarPorDPI(const std::string& dpi);
    bool dpiExiste(const std::string& dpi) const;

};

#endif
