#include <iostream>
#include <string>
#include "ODBCConnect/ODBCConnection.h"
#include "Database/Usuarios.h"
#include "controllers/PacienteController.h"
#include "controllers/MedicoControllers.h"
#include "controllers/CitaControllers.h"
#include "controllers/especialidadescontrollers.h"
#include "menus/pacientes_menu.h"
#include "menus/medicos_menu.h"
#include "menus/citas_menu.h"
#include "menus/especialidades_menu.h"
#include "Menus/factura.h"
#include <windows.h>

#include <cstdlib> // Necesario para la función system()

// Función para limpiar la pantalla



void menuPrincipal() {
    std::cout << "\n======= SISTEMA DE CITAS MÉDICAS =======\n";
    std::cout << "1. Pacientes\n";
    std::cout << "2. Médicos\n";
    std::cout << "3. Citas\n";
    std::cout << "4. Especialidades\n";
    std::cout << "5. Generar Factura\n";
    std::cout << "0. Salir\n";
    std::cout << "Seleccione una opcion: ";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::shared_ptr<Usuarios> usuario = login();
    if (!usuario) return 1;

    ODBCConnection conexion;
    if (!conexion.conectar("", "", "")) {
        std::cerr << "No se pudo establecer la conexion con la base de datos.\n";
        return 1;
    }

    PacienteController pacienteCtrl(&conexion);
    MedicoController medicoCtrl(&conexion);
    CitaController citaCtrl(&conexion);
    EspecialidadesController especialidadesCtrl(&conexion);


    int opcionPrincipal;

    do {
        menuPrincipal();
        std::cin >> opcionPrincipal;
        std::cin.ignore();

        switch (opcionPrincipal) {
            case 1:

                mostrarMenuPacientes(usuario, pacienteCtrl);
                break;
            case 2:

                mostrarMenuMedicos(usuario, medicoCtrl);
                break;
            case 3:

                mostrarMenuCitas(usuario, citaCtrl);
                break;
            case 4:

                mostrarMenuEspecialidades(usuario, especialidadesCtrl);
                break;
            case 5: {

                if (!usuario->puedeGenerarFactura()) {
                    std::cout << "No tienes permisos para Generar facturas.\n";
                }
                std::string nombrePaciente;
                std::string input;
                int idEspecialidad;

                do {
                    std::cout << "Ingrese el nombre del paciente: ";
                    std::getline(std::cin, nombrePaciente);
                    if (nombrePaciente.empty()) {
                        std::cout << "El campo no puede estar vacío.\n";
                    }
                }while (nombrePaciente.empty());
                do {
                    std::cout << "Ingrese el ID de la especialidad: ";
                    std::getline(std::cin, input);
                    if (input.empty()) {
                        std::cout << "Este campo no puede estar vacio.\n";
                        continue;
                    }
                    try {
                        idEspecialidad = std::stoi(input);
                        break;
                    } catch (...) {
                        std::cout << "Por favor ingresa un número valido.\n";
                    }
                } while (true);

                // Obtener lista de especialidades desde el controlador
                std::vector<Especialidades> listaEspecialidades = especialidadesCtrl.obtenerTodas();

                // Crear factura y generarla
                Factura factura(nombrePaciente, idEspecialidad);
                factura.generarFactura(listaEspecialidades);
                break;
            }

            case 0:
                std::cout << "Saliendo del sistema...\n";
                break;
            default:
                std::cout << "Opcion no valida.\n";
        }
    } while (opcionPrincipal != 0);

    conexion.desconectar();
    return 0;
}