//
// Created by Josué on 12/05/2025.
//

#ifndef ESPECIALIDADES_H
#define ESPECIALIDADES_H
#include <string> // Se necesita para usar std::string


class Especialidades{

private:
    int id;                 // ID único de la especialidad
    std::string nombre;      // Nombre de la especialidad
    std::string precio_consulta;

public:
    // Constructor por defecto
    Especialidades();

    // Constructor con todos los parámetros
    Especialidades(int id, const std::string& nombre, const std::string& precio_consulta);

    // Getters
    int getId() const;
    std::string getnombre() const;
    std::string getprecio_consulta() const;

    // Setters
    void setId(int id);

    void setnombre(const std::string& nombre);

    void setprecio_consulta(const std::string& precio_consulta);


};









#endif //ESPECIALIDADES_H
