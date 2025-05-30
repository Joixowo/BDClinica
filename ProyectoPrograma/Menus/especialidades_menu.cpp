#include "especialidades_menu.h"
#include <iostream>
#include <string>
#include <limits>

void mostrarMenuEspecialidades(std::shared_ptr<Usuarios> usuario, EspecialidadesController& especialidadesCtrl) {
    int opcionEspecialidad;
    do {
        std::cout << "\n===== MENÚ ESPECIALIDADES =====\n";
        std::cout << "1. Agregar especialidad\n";
        std::cout << "2. Ver especialidades\n";
        std::cout << "3. Actualizar especialidad\n";
        std::cout << "4. Eliminar especialidad\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcionEspecialidad;
        std::cin.ignore();

        switch (opcionEspecialidad) {
            case 1: {
                std::string nombre,precio_consulta;
                if (!usuario->puedeRegistrarEspecialidad()) {
                    std::cout << "❌ No tienes permisos para ver especialidades.\n";
                    break;
                }
                do {
                    std::cout << "Nombre de la especialidad: ";
                    std::getline(std::cin, nombre);
                    if (nombre.empty()) {
                        std::cout << "❌ El campo no puede estar vacío.\n";
                    }
                } while (nombre.empty());
                do {
                    std::cout << "Precio de la consulta: ";
                    std::getline(std::cin, precio_consulta);
                    if (precio_consulta.empty()) {
                        std::cout << "❌ El campo no puede estar vacío.\n";
                    }
                }while (precio_consulta.empty());
                especialidadesCtrl.agregar(nombre,precio_consulta);
                break;
            }
            case 2:
                if (!usuario->puedeVerDatos()) {
                    std::cout << "❌ No tienes permisos para ver especialidades.\n";
                    break;
                }
            especialidadesCtrl.mostrarTodos();
            break;
            case 3: {
                int id;
                std::string nombre, precio_consulta;
                if (!usuario->puedeActualizardatos()) {
                    std::cout << "❌ No tienes permisos para ver especialidades.\n";
                    break;
                }
                std::cout << "Ingrese el ID de la especialidad que desea actualizar: ";
                std::cin >> id;
                std::cin.ignore(); // Consume el newline que deja std::cin

                do {
                    std::cout << "Nuevo nombre de la especialidad: ";
                    std::getline(std::cin, nombre);
                    if (nombre.empty()) {
                        std::cout << "❌ El campo no puede estar vacío.\n";
                    }
                } while (nombre.empty());

                do {
                    std::cout << "Nuevo precio de la consulta: ";
                    std::getline(std::cin,precio_consulta);
                    if (precio_consulta.empty()) {
                        std::cout << "❌ El campo no puede estar vacío.\n";
                    }
                } while (precio_consulta.empty());

                especialidadesCtrl.actualizar(id, nombre, precio_consulta);
                break;
            }
            case 0:
                std::cout << "Volviendo al menú principal...\n";
            break;
            default:
                std::cout << "❌ Opción no válida.\n";
        }
    } while (opcionEspecialidad != 0);
}