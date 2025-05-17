#ifndef FACTURA_H
#define FACTURA_H

#include <string>
#include "../Database/Especialidades.h"
#include <vector>

class Factura {
private:
    std::string nombrePaciente;
    int idEspecialidad;
    std::string nombreEspecialidad;
    std::string precioConsulta;

public:
    Factura();
    Factura(const std::string& nombrePaciente, int idEspecialidad);

    void generarFactura(const std::vector<Especialidades>& listaEspecialidades);

    // Getters (si los necesitas)
    std::string getNombrePaciente() const;
    std::string getNombreEspecialidad() const;
    std::string getPrecioConsulta() const;
};

#endif // FACTURA_H
