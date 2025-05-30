#include "medicos_menu.h"
#include "../controllers/MedicoControllers.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>



void mostrarMenuMedicos(std::shared_ptr<Usuarios> usuario, MedicoController& medicoCtrl) {
    int opcionMedico;
    do {
        std::cout << "\n===== MENÚ MÉDICOS =====\n";
        std::cout << "1. Registrar médico\n";
        std::cout << "2. Ver médicos\n";
        std::cout << "3. Buscar Médico por nombre\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcionMedico;
        std::cin.ignore();

        switch (opcionMedico) {
            case 1: {
                if (!usuario->puedeRegistrarMedico()) {
                    std::cout << "❌ No tienes permisos para registrar médicos.\n";
                    break;
                }
                std::string nombre, especialidad, telefono;
                // ... (tu código actual para registrar médico con validación) ...
                do {
                    std::cout << "Nombre: "; std::getline(std::cin, nombre);
                    if (nombre.empty()) {
                        std::cout << "❌ El campo esta vacío, ingrese un nombre por favor\n";
                    }
                } while (nombre.empty());
                do {
                    std::cout << "Especialidad: "; std::getline(std::cin, especialidad);
                    if (especialidad.empty()) {
                        std::cout << "❌ El campo esta vacío, ingrese una especialidad por favor\n";
                    }
                } while (especialidad.empty());
                do {
                    std::cout << "Teléfono: "; std::getline(std::cin, telefono);
                    if (telefono.empty()) {
                        std::cout << "❌ El campo esta vacío, ingrese un teléfono por favor\n";
                    }
                } while (telefono.empty());
                medicoCtrl.agregar(nombre, especialidad, telefono);
                break;
            }
            case 2:
                if (!usuario->puedeVerDatos()) {
                    std::cout << "❌ No tienes permisos para ver médicos.\n";
                    break;
                }
                medicoCtrl.mostrarTodos();
                break;
            case 3: {
                std::string nombreBuscado;
                std::cout << "\nIngrese el nombre del médico a buscar: ";
                std::getline(std::cin >> std::ws, nombreBuscado);

                std::vector<Medico> resultados = medicoCtrl.buscarMedicosPorNombre(nombreBuscado);


                if (resultados.empty()) {
                    std::cout << "\n⚠️ No se encontraron médicos con ese nombre.\n";
                }
                break;
            }

            case 0:
                std::cout << "Volviendo al menú principal...\n";
                break;
            default:
                std::cout << "❌ Opción no válida.\n";
        }
    } while (opcionMedico != 0);
}