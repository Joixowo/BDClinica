#include "Cita.h"

// Constructor por defecto: valores vacíos y 0
Cita::Cita() : id(0), idPaciente(0), fecha(""), hora(""), motivo("") {}


// Constructor con todos los parámetros
Cita::Cita(int id, int idPaciente, const std::string& fecha, const std::string& hora, const std::string& motivo)
 : id(id), idPaciente(idPaciente), fecha(fecha), hora(hora), motivo(motivo) {}


// Getters
int Cita::getId() const { return id; }
int Cita::getIdPaciente() const { return idPaciente; }
std::string Cita::getFecha() const { return fecha; }
std::string Cita::getHora() const { return hora; }
std::string Cita::getMotivo() const { return motivo; }



// Setters
void Cita::setId(int id) { this->id = id; }
void Cita::setIdPaciente(int idPaciente) { this->idPaciente = idPaciente; }
void Cita::setFecha(const std::string& fecha) { this->fecha = fecha; }
void Cita::setHora(const std::string& hora) { this->hora = hora; }
void Cita::setMotivo(const std::string& motivo) { this->motivo = motivo; }