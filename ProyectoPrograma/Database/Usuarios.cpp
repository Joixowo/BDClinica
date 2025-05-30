#include "Usuarios.h"
#include <iostream>

// Clase privada: Admin (fuera del header para mantenerla privada)
class crearAdmin : public Usuarios {
public:
    crearAdmin() : Usuarios("admin", "admin_123", "admin") {}

    bool puedeRegistrarPaciente() const override { return true; }
    bool puedeRegistrarEspecialidad() override { return true; }
    bool puedeRegistrarMedico() const override { return true; }
    bool puedeCrearCita() const override { return true; }
    bool puedeVerDatos() const override { return true; }
    bool puedeActualizardatos() const override { return true; }
    bool puedeActualizarCita() const override { return true; }
    bool puedeBuscarCita() const override { return true; }
    bool puedeGenerarFactura() const override { return true; }
    bool puedeBuscarPaciente() const override { return true; }
    bool puedeCancelarCita() const override { return true; }
};

std::shared_ptr<Usuarios> login() {
    std::string user, pass;
    std::shared_ptr<Usuarios> usuario = nullptr;

    do { //sirve para encerrar en ciclo si se ingrea mal un parametro
        std::cout << "===== INICIO DE SESION =====\n";
        std::cout << "Ingrese su Usuario: ";
        std::getline(std::cin, user);
        std::cout << "Ingrese su Contraseña: ";
        std::getline(std::cin, pass);

        if (user.empty() || pass.empty()) {
            std::cout << "❌ Los campos de usuario y contrasena no pueden estar vacios. Intente de nuevo.\n\n";
            usuario = nullptr; // Aseguramos que usuario sea nulo para repetir el ciclo
            continue;
        }

        if (user == "admin" && pass == "admin_123") {
            usuario = std::make_shared<crearAdmin>();
        } else if (user == "medicos" && pass == "med_123") {
            usuario = std::make_shared<Medicos>();
        } else if (user == "recepcion" && pass == "rec_123") {
            usuario = std::make_shared<Recepcion>();
        }

        if (!usuario) {
            std::cout << "❌ Usuario o contraseña incorrectos. Intente de nuevo.\n\n";
        }

    } while (!usuario);

    std::cout << "Bienvenido, " << usuario->getUsername() << " (Rol: " << usuario->getRol() << ")\n";
    return usuario;
}
