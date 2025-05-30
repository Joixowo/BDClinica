//
// Created by Josué on 12/05/2025.
//

#include "Especialidades.h"


Especialidades::Especialidades() : id(0),nombre(""),precio_consulta(""){}

// Constructor con todos los parámetros
Especialidades::Especialidades(int id, const std::string& nombre,const std::string& precio_consulta)
    : id(id), nombre(nombre), precio_consulta(precio_consulta) {}

// Getters
int Especialidades::getId() const { return id; }
std::string Especialidades::getnombre() const{ return nombre; }
std::string Especialidades::getprecio_consulta() const{ return precio_consulta; }

// Setters
void Especialidades::setId(int id) { this->id = id; }
void Especialidades::setnombre(const std::string& nombre) { this->nombre = nombre; }
void Especialidades::setprecio_consulta(const std::string& precio_consulta) { this->precio_consulta = precio_consulta; }


