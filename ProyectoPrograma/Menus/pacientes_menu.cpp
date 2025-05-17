#include "pacientes_menu.h"
#include <iostream>
#include <string>
#include <limits>

void mostrarMenuPacientes(std::shared_ptr<Usuarios> usuario, PacienteController& pacienteCtrl) {
    int opcionPaciente;
    do {
        std::cout << "\n===== MENÚ PACIENTES =====\n";
        std::cout << "1. Registrar paciente\n";
        std::cout << "2. Ver pacientes\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcionPaciente;
        std::cin.ignore();

        switch (opcionPaciente) {
            case 1: {
                if (!usuario->puedeRegistrarPaciente()) {
                    std::cout << "No tienes permisos para registrar pacientes.\n";
                    break;
                }
                std::string nombre, apellido, telefono,fecha_registro;
                // ... (tu código actual para registrar paciente con validación) ...
                do {
                    std::cout << "Nombre: "; std::getline(std::cin, nombre);
                    if (nombre.empty()) {
                        std::cout << "El campo esta vacío, ingrese un nombre por favor\n";
                    }
                } while (nombre.empty());
                do {
                    std::cout << "Apellido: "; std::getline(std::cin, apellido);
                    if (apellido.empty()) {
                        std::cout << "El campo esta vacío, ingrese un apellido por favor\n";
                    }
                } while (apellido.empty());
                do {
                    std::cout << "Teléfono: "; std::getline(std::cin, telefono);
                    if (telefono.empty()) {
                        std::cout << "El campo esta vacío, ingrese un teléfono por favor\n";
                    }
                } while (telefono.empty());
                do {
                    std::cout << "Fecha de registro: "; std::getline(std::cin, fecha_registro);
                    if (telefono.empty()) {
                        std::cout << "El campo esta vacío, ingrese un teléfono por favor\n";
                    }
                } while (telefono.empty());
                pacienteCtrl.agregar(nombre, apellido, telefono,fecha_registro);
                break;
            }
            case 2:
                if (!usuario->puedeVerDatos()) {
                    std::cout << "No tienes permisos para ver pacientes.\n";
                    break;
                }
                pacienteCtrl.mostrarTodos();
                break;
            case 0:
                std::cout << "Volviendo al menú principal...\n";
                break;
            default:
                std::cout << "❌ Opción no válida.\n";
        }
    } while (opcionPaciente != 0);
}