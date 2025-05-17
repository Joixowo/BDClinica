#include "factura.h"
#include <iostream>
#include <iomanip>

Factura::Factura() : nombrePaciente(""), idEspecialidad(0), nombreEspecialidad(""), precioConsulta("") {}

Factura::Factura(const std::string& nombrePaciente, int idEspecialidad)
    : nombrePaciente(nombrePaciente), idEspecialidad(idEspecialidad), nombreEspecialidad(""), precioConsulta("") {}

void Factura::generarFactura(const std::vector<Especialidades>& listaEspecialidades) {
    // Buscar la especialidad por ID
    bool encontrada = false;
    for (const auto& especialidad : listaEspecialidades) {
        if (especialidad.getId() == idEspecialidad) {
            nombreEspecialidad = especialidad.getnombre();
            precioConsulta = especialidad.getprecio_consulta();
            encontrada = true;
            break;
        }
    }

    if (!encontrada) {
        std::cout << "❌ Especialidad no encontrada con ID: " << idEspecialidad << std::endl;
        return;
    }

    // Imprimir la factura con formato tabulado
    std::cout << "\n=========== 🧾 FACTURA MEDICA ===========" << std::endl;
    std::cout << std::left << std::setw(20) << "Paciente:" << nombrePaciente << std::endl;
    std::cout << std::left << std::setw(20) << "Especialidad:" << nombreEspecialidad << std::endl;
    std::cout << std::left << std::setw(20) << "Precio de Consulta:" << precioConsulta << std::endl;
    std::cout << "=========================================\n" << std::endl;
}

std::string Factura::getNombrePaciente() const {
    return nombrePaciente;
}

std::string Factura::getNombreEspecialidad() const {
    return nombreEspecialidad;
}

std::string Factura::getPrecioConsulta() const {
    return precioConsulta;
}