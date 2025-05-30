#ifndef MEDICO_H
#define MEDICO_H

#include <string> // Para std::string

// Clase que representa a un médico
class Medico {
private:
    int id;                 // ID único del médico
    std::string nombre;     // Nombre completo
    std::string especialidad; // Área de especialización
    std::string telefono;   // Número de teléfono

public:
    // Constructor por defecto
    Medico();

    // Constructor con parámetros
    Medico(int id, const std::string& nombre, const std::string& especialidad, const std::string& telefono);

    // Getters
    int getId() const;
    std::string getNombre() const;
    std::string getEspecialidad() const;
    std::string getTelefono() const;

    // Setters
    void setId(int id);
    void setNombre(const std::string& nombre);
    void setEspecialidad(const std::string& especialidad);
    void setTelefono(const std::string& telefono);
};

#endif
