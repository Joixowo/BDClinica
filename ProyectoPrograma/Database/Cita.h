#ifndef CITA_H
#define CITA_H

#include <string> // Se necesita para usar std::string


// Clase que representa una cita médica
class Cita {
private:
    int id;                 // ID único de la cita
    int idPaciente;         // ID del paciente relacionado
    std::string fecha;      // Fecha de la cita (YYYY-MM-DD)
    std::string hora;       // Hora de la cita (HH:MM)
    std::string motivo;     // Motivo o descripción de la cita


public:
    // Constructor por defecto
    Cita();


    // Constructor con todos los parámetros
    Cita(int id, int idPaciente, const std::string& fecha, const std::string& hora, const std::string& motivo);


    // Getters
    int getId() const;
    int getIdPaciente() const;
    std::string getFecha() const;
    std::string getHora() const;
    std::string getMotivo() const;


    // Setters
    void setId(int id);
    void setIdPaciente(int idPaciente);
    void setFecha(const std::string& fecha);
    void setHora(const std::string& hora);
    void setMotivo(const std::string& motivo);
};

#endif
