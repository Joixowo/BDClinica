#ifndef PACIENTE_H
#define PACIENTE_H

#include <string> // Necesario para usar std::string

// Clase que representa a un paciente del sistema
class Paciente {
private:
    int id;                 // ID único del paciente
    std::string nombre;     // Nombre del paciente
    std::string apellido;   // Apellido del paciente
    std::string telefono;   // Número de teléfono del paciente
    std::string fecha_registro;

public:
    // Constructor por defecto: inicializa los campos con valores vacíos o cero
    Paciente();

    // Constructor con parámetros: permite crear un paciente con todos los datos
    Paciente(int id, const std::string& nombre, const std::string& apellido, const std::string& telefono,const std::string& fecha_registro);

    // Getter: devuelve el ID del paciente
    int getId() const;

    // Getter: devuelve el nombre del paciente
    std::string getNombre() const;

    // Getter: devuelve el apellido del paciente
    std::string getApellido() const;

    // Getter: devuelve el teléfono del paciente
    std::string getTelefono() const;

    // Setter: permite modificar el ID del paciente
    void setId(int id);

    // Setter: permite modificar el nombre del paciente
    void setNombre(const std::string& nombre);

    // Setter: permite modificar el apellido del paciente
    void setApellido(const std::string& apellido);

    // Setter: permite modificar el numero del paciente
    void setTelefono(const std::string& telefono);

    void setFechaRegistro(const std::string& fecha_registro);

};
    // Setter: per
#endif
