#ifndef USUARIOS_H
#define USUARIOS_H

#include <string>
#include <memory>

class Usuarios {
protected:
    std::string username;
    std::string password;
    std::string rol;

public:
    Usuarios(const std::string& user, const std::string& pass, const std::string& role)
        : username(user), password(pass), rol(role) {}

    virtual ~Usuarios() {}

    std::string getUsername() const { return username; }
    std::string getRol() const { return rol; }
    bool verificarCredenciales(const std::string& user, const std::string& pass) const {
        return username == user && password == pass;
    }

    virtual bool puedeRegistrarPaciente() const = 0;
    virtual bool puedeRegistrarEspecialidad() = 0;
    virtual bool puedeRegistrarMedico() const = 0;
    virtual bool puedeCrearCita() const = 0;
    virtual bool puedeVerDatos() const = 0;
    virtual bool puedeActualizardatos() const = 0;
    virtual bool puedeActualizarCita() const = 0;
    virtual bool puedeBuscarCita() const = 0;
    virtual bool puedeGenerarFactura() const = 0;
};

// Clase privada Admin (dentro del .cpp)
class crearAdmin;

// Médico
class Medicos : public Usuarios {
public:
    Medicos() : Usuarios("medicos", "med_123", "medico") {}

    bool puedeRegistrarPaciente() const override { return false; }// este metodo me da acceso a las funciones por usuario
    bool puedeRegistrarEspecialidad() override { return false; }
    bool puedeRegistrarMedico() const override { return false; }
    bool puedeCrearCita() const override { return false; }
    bool puedeVerDatos() const override { return true; }
    bool puedeActualizardatos() const override { return false;}
    bool puedeActualizarCita() const override { return false; }
    bool puedeBuscarCita() const override { return true; }
    bool puedeGenerarFactura() const override { return false; }
};

// Recepción
class Recepcion : public Usuarios {
public:
    Recepcion() : Usuarios("recepcion", "rec_123", "recepcion") {}

    bool puedeRegistrarPaciente() const override { return true; }
    bool puedeRegistrarEspecialidad() override { return false; }
    bool puedeRegistrarMedico() const override { return false; }
    bool puedeCrearCita() const override { return true; }
    bool puedeVerDatos() const override { return true; }
    bool puedeActualizardatos() const override { return true;}
    bool puedeActualizarCita() const override { return true; }
    bool puedeBuscarCita() const override { return true; }
    bool puedeGenerarFactura() const override { return true; }
};

// Función para autenticar usuarios y devolver puntero dinámico a la instancia
std::shared_ptr<Usuarios> login();

#endif
