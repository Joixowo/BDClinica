#include "pacientes_menu.h"
#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <sstream>
#include <iomanip>
#include <ctime>

bool fechaEsValida2(const std::string& fechaHora) {
    std::regex formatoFechaHora(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$)");
    if (!std::regex_match(fechaHora, formatoFechaHora)) return false;

    std::tm tm = {};
    std::istringstream ss(fechaHora);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) return false;

    // Comprobación final con mktime (detecta fechas como 30 feb)
    tm.tm_isdst = -1;
    std::time_t t = std::mktime(&tm);
    std::tm* verificada = std::localtime(&t);

    return verificada->tm_year == tm.tm_year &&
           verificada->tm_mon  == tm.tm_mon &&
           verificada->tm_mday == tm.tm_mday;
}


void mostrarMenuPacientes(std::shared_ptr<Usuarios> usuario, PacienteController& pacienteCtrl) {
    int opcionPaciente;
    do {
        std::cout << "\n===== MENÚ PACIENTES =====\n";
        std::cout << "1. Registrar paciente\n";
        std::cout << "2. Ver pacientes\n";
        std::cout << "3. Buscar pacientes\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcionPaciente;
        std::cin.ignore();

        switch (opcionPaciente) {
            case 1: {
                if (!usuario->puedeRegistrarPaciente()) {
                    std::cout << "❌ No tienes permisos para registrar pacientes.\n";
                    break;
                }

                std::string nombre, apellido, telefono, fecha_registro, dpi;

                // Nombre
                do {
                    std::cout << "Nombre: ";
                    std::getline(std::cin, nombre);
                    if (nombre.empty()) {
                        std::cout << "❌ El campo está vacío, ingrese un nombre por favor.\n";
                    }
                } while (nombre.empty());

                // Apellido
                do {
                    std::cout << "Apellido: ";
                    std::getline(std::cin, apellido);
                    if (apellido.empty()) {
                        std::cout << "❌ El campo está vacío, ingrese un apellido por favor.\n";
                    }
                } while (apellido.empty());

                // Teléfono
                do {
                    std::cout << "Teléfono (8 dígitos): ";
                    std::getline(std::cin, telefono);

                    bool valido = true;
                    if (telefono.length() != 8) {
                        std::cout << "❌ El teléfono debe tener exactamente 8 dígitos.\n";
                        valido = false;
                    } else {
                        for (char c : telefono) {
                            if (!isdigit(c)) {
                                std::cout << "❌ El teléfono solo debe contener números.\n";
                                valido = false;
                                break;
                            }
                        }
                    }

                    if (valido) break;

                } while (true);

                // Fecha de registro
                do {
                    std::cout << "Fecha de registro (YYYY-MM-DD HH:MM:SS): ";
                    std::getline(std::cin, fecha_registro);

                    if (fecha_registro.empty()) {
                        std::cout << "❌ El campo está vacío.\n";
                    } else if (!fechaEsValida2(fecha_registro)) {
                        std::cout << "❌ Fecha inválida. Usa formato correcto y verifica que la fecha exista.\n";
                    } else {
                        break;
                    }

                } while (true);

                // DPI
                do {
                    std::cout << "DPI (13 dígitos): ";
                    std::getline(std::cin, dpi);

                    bool valido = true;
                    if (dpi.length() != 13) {
                        std::cout << "❌ El DPI debe tener exactamente 13 dígitos.\n";
                        valido = false;
                    } else {
                        for (char c : dpi) {
                            if (!isdigit(c)) {
                                std::cout << "❌ El DPI solo debe contener números.\n";
                                valido = false;
                                break;
                            }
                        }
                    }

                    if (valido && pacienteCtrl.dpiExiste(dpi)) {
                        std::cout << "❌ Este DPI ya está registrado. Verifica los datos.\n";
                        valido = false;
                    }

                    if (valido) break;

                } while (true);

                // Registro final
                if (pacienteCtrl.agregar(nombre, apellido, telefono, fecha_registro, dpi)) {
                    std::cout << "✅ Paciente registrado exitosamente.\n";
                } else {
                    std::cout << "❌ No se pudo registrar el paciente. Revisa la base de datos.\n";
                }

                break;
            }



            case 2:
                if (!usuario->puedeVerDatos()) {
                    std::cout << "No tienes permisos para ver pacientes.\n";
                    break;
                }
                pacienteCtrl.mostrarTodos();
                break;
            case 3: {
                if (!usuario->puedeVerDatos()) {
                    std::cout << "No tienes permisos para buscar pacientes.\n";
                    break;
                }

                std::string dpiBuscar;
                do {
                    std::cout << "Ingrese el DPI del paciente a buscar: ";
                    std::getline(std::cin, dpiBuscar);
                    if (dpiBuscar.empty()) {
                        std::cout << "❌ El campo está vacío. Por favor, ingrese un DPI.\n";
                    }
                } while (dpiBuscar.empty());

                pacienteCtrl.buscarPorDPI(dpiBuscar);
                break;
            }
            case 0:
                std::cout << "Volviendo al menú principal...\n";
                break;
            default:
                std::cout << "❌ Opción no válida.\n";
        }
    } while (opcionPaciente != 0);
}