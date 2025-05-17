#ifndef MEDICOCONTROLLER_H
#define MEDICOCONTROLLER_H

#include "../Database/Medico.h"              // Incluye la definición del modelo Medico
#include "../ODBCConnect/ODBCConnection.h"   // Para manejar conexión ODBC

class MedicoController {
private:
    ODBCConnection* conexion; // Puntero a la conexión para ejecutar consultas

public:
    // Constructor con inyección de dependencia de la conexión
    MedicoController(ODBCConnection* conn);

    // Inserta un nuevo médico con nombre, especialidad y teléfono
    bool agregar(const std::string& nombre, const std::string& especialidad, const std::string& telefono);

    // Muestra todos los médicos registrados
    void mostrarTodos();
};

#endif
