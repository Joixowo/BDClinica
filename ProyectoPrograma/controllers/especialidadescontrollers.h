#ifndef ESPECIALIDADESCONTROLLERS_H
#define ESPECIALIDADESCONTROLLERS_H

#include "../ODBCConnect/ODBCConnection.h" // Incluye la clase para conexión a la base de datos
#include "../Database/Especialidades.h"    // Incluye la clase Especialidades
#include <string>           // Para usar std::string
#include <iostream>         // Para salida por consola
#include <vector>           // Para usar std::vector

// Clase que manage las operaciones CRUD (crear, leer, actualizar, eliminar) de especialidades
class EspecialidadesController {
private:
    ODBCConnection* conexion; // Puntero a la conexión ODBC

public:
    // Constructor que recibe un puntero a la conexión
    EspecialidadesController(ODBCConnection* conn);

    // Método para agregar una nueva especialidad
    bool agregar(const std::string& nombre, const std::string& precio_consulta);

    // Método para mostrar todas las especialidades
    void mostrarTodos();

    // Método para eliminar una especialidad por id
    bool eliminar(int id);

    // Método para actualizar el nombre de una especialidad por id
    bool actualizar(int id, const std::string& nuevoNombre,const std::string& precio_consulta);

    // Método para obtener todas las especialidades (para uso en Factura)
    std::vector<Especialidades> obtenerTodas();
};

#endif // ESPECIALIDADESCONTROLLERS_H
