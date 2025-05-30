#include "Medico.h"

// Constructor por defecto
Medico::Medico() : id(0), nombre(""), especialidad(""), telefono("") {}

// Constructor con todos los datos del médico
Medico::Medico(int id, const std::string& nombre, const std::string& especialidad, const std::string& telefono)
    : id(id), nombre(nombre), especialidad(especialidad), telefono(telefono) {}

// Getters
int Medico::getId() const { return id; }
std::string Medico::getNombre() const { return nombre; }
std::string Medico::getEspecialidad() const { return especialidad; }
std::string Medico::getTelefono() const { return telefono; }

// Setters
void Medico::setId(int id) { this->id = id; }
void Medico::setNombre(const std::string& nombre) { this->nombre = nombre; }
void Medico::setEspecialidad(const std::string& especialidad) { this->especialidad = especialidad; }
void Medico::setTelefono(const std::string& telefono) { this->telefono = telefono; }
