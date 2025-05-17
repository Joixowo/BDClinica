#include "Paciente.h"

// Constructor por defecto: asigna valores vacíos o cero
Paciente::Paciente() : id(0), nombre(""), apellido(""), telefono(""), fecha_registro("") {}

// Constructor con parámetros: inicializa los atributos con los valores dados
Paciente::Paciente(int id, const std::string& nombre, const std::string& apellido, const std::string& telefono,const std::string& fecha_registro)
    : id(id), nombre(nombre), apellido(apellido), telefono(telefono) {}

// Retorna el ID del paciente
int Paciente::getId() const { return id; }

// Retorna el nombre del paciente
std::string Paciente::getNombre() const { return nombre; }

// Retorna el apellido del paciente
std::string Paciente::getApellido() const { return apellido; }

// Retorna el número de teléfono del paciente
std::string Paciente::getTelefono() const { return telefono; }

// Modifica el ID del paciente
void Paciente::setId(int id) { this->id = id; }

// Modifica el nombre del paciente
void Paciente::setNombre(const std::string& nombre) { this->nombre = nombre; }

// Modifica el apellido del paciente
void Paciente::setApellido(const std::string& apellido) { this->apellido = apellido; }

// Modifica el teléfono del paciente
void Paciente::setTelefono(const std::string& telefono) { this->telefono = telefono; }

void Paciente::setFechaRegistro(const std::string& fecha_registro) {this->fecha_registro = fecha_registro;}

