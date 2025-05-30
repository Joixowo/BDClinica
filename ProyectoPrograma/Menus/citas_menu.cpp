#include "citas_menu.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <iomanip>
#include <regex>
#include <sstream>


bool fechaEsValida(const std::string& fechaHora) {
    std::regex formato("^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}$");
    if (!std::regex_match(fechaHora, formato)) return false;

    std::tm tm = {};
    std::istringstream ss(fechaHora);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) return false;

    // Copia original antes de mktime
    std::tm tmOriginal = tm;

    std::time_t tiempo = std::mktime(&tm);
    std::tm* tiempoNormalizado = std::localtime(&tiempo);

    // Comparamos si se alteró (ej. 31 abril → 1 mayo)
    return tmOriginal.tm_year == tiempoNormalizado->tm_year &&
           tmOriginal.tm_mon == tiempoNormalizado->tm_mon &&
           tmOriginal.tm_mday == tiempoNormalizado->tm_mday &&
           tmOriginal.tm_hour == tiempoNormalizado->tm_hour &&
           tmOriginal.tm_min == tiempoNormalizado->tm_min &&
           tmOriginal.tm_sec == tiempoNormalizado->tm_sec;
}



void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void mostrarMenuCitas(std::shared_ptr<Usuarios> usuario, CitaController& citaCtrl) {
    int opcionCita;
    do {
        std::cout << "\n===== MENÚ CITAS =====\n";
        std::cout << "1. Agendar cita\n";
        std::cout << "2. Ver citas\n";
        std::cout << "3. Actualizar cita\n";
        std::cout << "4. Buscar cita\n";
        std::cout << "5. Cancelar cita\n";
        std::cout << "6. Filtrar citas\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcionCita;

        if (std::cin.fail()) {
            std::cin.clear(); // Limpia el estado de error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpia el buffer
            std::cout << "❌ Entrada inválida. Por favor ingrese un número.\n";
            opcionCita = -1; // Fuerza una iteración inválida
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpia cualquier carácter extra
        ;

        switch (opcionCita) {
            case 1: {
                if (!usuario->puedeCrearCita()) {
                    std::cout << "❌ No tienes permisos para crear citas.\n";
                    break;
                }

                int id_paciente, id_medico;
                std::string fechaHora, motivo, input;

                // Validar ID del paciente
                do {
                    std::cout << "ID del paciente: ";
                    std::getline(std::cin, input);
                    if (input.empty()) {
                        std::cout << "❌ Este campo no puede estar vacío.\n";
                        continue;
                    }
                    try {
                        id_paciente = std::stoi(input);
                        break;
                    } catch (...) {
                        std::cout << "❌ Por favor ingrese un número válido.\n";
                    }
                } while (true);

                // Validar ID del médico
                do {
                    std::cout << "ID del médico: ";
                    std::getline(std::cin, input);
                    if (input.empty()) {
                        std::cout << "❌ Este campo no puede estar vacío.\n";
                        continue;
                    }
                    try {
                        id_medico = std::stoi(input);
                        break;
                    } catch (...) {
                        std::cout << "❌ Por favor ingrese un número válido.\n";
                    }
                } while (true);

                // Validar fecha y hora
                do {
                    std::cout << "Fecha y hora (YYYY-MM-DD HH:MM:SS): ";
                    std::getline(std::cin, fechaHora);
                    if (fechaHora.empty()) {
                        std::cout << "❌ Este campo no puede estar vacío.\n";
                        continue;
                    }
                    if (!fechaEsValida(fechaHora)) {
                        std::cout << "❌ Fecha inválida. Asegúrate de que sea una fecha real.\n";
                        continue;
                    }
                    break;
                } while (true);

                // Validar motivo
                do {
                    std::cout << "Motivo: ";
                    std::getline(std::cin, motivo);
                    if (motivo.empty()) {
                        std::cout << "❌ Este campo no puede estar vacío.\n";
                    }
                } while (motivo.empty());

                // Agregar la cita (suponiendo que tienes una función agregarCita)
                if (citaCtrl.agregar(id_paciente, id_medico, fechaHora, motivo)) {
                    std::cout << "✅ Cita agregada correctamente.\n";
                } else {
                    std::cout << "❌ Error al agregar la cita.\n";
                }

                break;
            }

            case 2:
                if (!usuario->puedeVerDatos()) {
                    std::cout << "❌ No tienes permisos para ver citas.\n";
                    break;
                }
            citaCtrl.mostrarTodos();
            break;
            case 3:{
                if (!usuario->puedeActualizarCita()) {
                    std::cout << "❌ No tienes permisos para actualizar citas.\n";
                    break;
                }
                int idCita, idMedico;
            std::pmr::string fecha;
            std::cout << "Ingrese el ID de la cita a reprogramar: ";
            std::cin >> idCita;
            limpiarBuffer();

                std::string nuevaFecha, nuevaHora, nuevaFechaHoraCompleta;
                do {
                    std::cout << "Ingrese la nueva fecha (YYYY-MM-DD): ";
                    std::getline(std::cin, nuevaFecha);
                    std::cout << "Ingrese la nueva hora (HH:MM:SS): ";
                    std::getline(std::cin, nuevaHora);

                    nuevaFechaHoraCompleta = nuevaFecha + " " + nuevaHora;

                    if (!fechaEsValida(nuevaFechaHoraCompleta)) {
                        std::cout << "❌ Fecha y hora inválida. Asegúrate que la fecha u hora exista.\n";
                        continue;
                    }
                    break;
                } while (true);

            if (citaCtrl.reprogramarCita(idCita, nuevaFechaHoraCompleta)) {
                std::cout << "Cita reprogramada con exito a: " << nuevaFechaHoraCompleta << ".\n";
            } else {
                std::cout << "No se pudo reprogramar la cita.\n";
            }
            // Fin del bloque del case
            break;
        }
        case 4: {
    if (!usuario->puedeBuscarCita()) {
        std::cout << "❌ No tienes permisos para buscar citas.\n";
        break;
    }

    int subopcionBusqueda;
    do {
        std::cout << "\n--- Búsqueda de Citas ---\n";
        std::cout << "1. Buscar por nombre del paciente\n";
        std::cout << "2. Buscar por fecha\n";
        std::cout << "3. Buscar por DPI\n";
        std::cout << "0. Volver al menú anterior\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> subopcionBusqueda;

        if (std::cin.fail()) {
            std::cin.clear(); // Limpia error de tipo
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Elimina caracteres restantes
            std::cout << "❌ Entrada inválida. Por favor ingrese un número.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (subopcionBusqueda) {
            case 1: {
                std::string nombrePacienteBusqueda;
                std::cout << "Ingrese el nombre del paciente a buscar: ";
                std::getline(std::cin, nombrePacienteBusqueda);

                std::vector<Cita> citasEncontradas = citaCtrl.buscarCitasPorNombrePaciente(nombrePacienteBusqueda);

                if (citasEncontradas.empty()) {
                    std::cout << "No se encontraron citas para el paciente '" << nombrePacienteBusqueda << "'.\n";
                }
                break;
            }
            case 2: {
                std::string fechaBusqueda;
                std::cout << "Ingrese la fecha de la cita a buscar (YYYY-MM-DD): ";
                std::getline(std::cin, fechaBusqueda);

                std::vector<Cita> citasEncontradas = citaCtrl.buscarCitasPorFecha(fechaBusqueda);

                if (citasEncontradas.empty()) {
                    std::cout << "No se encontraron citas para la fecha '" << fechaBusqueda << "'.\n";
                }
                break;
            }
            case 3: {
                std::string dpiBuscar;
                std::cout << "Ingrese el DPI del paciente a buscar: ";
                std::getline(std::cin, dpiBuscar);

                std::vector<Cita> citasEncontradas = citaCtrl.buscarCitasPorDPI(dpiBuscar);

                if (citasEncontradas.empty()) {
                    std::cout << "No se encontraron citas para el paciente con DPI '" << dpiBuscar << "'.\n";
                }
                break;
            }
            case 0:
                std::cout << "Volviendo al menú de citas...\n";
                break;
            default:
                std::cout << "❌ Opción no válida. Intente nuevamente.\n";
        }

    } while (subopcionBusqueda != 0);

    break; // <<== IMPORTANTE: rompe el case 4 cuando se selecciona volver
}
            case 5: {
                if (!usuario->puedeCancelarCita()) {
                    std::cout << "❌ No tienes permisos para cancelar citas.\n";
                    break;
                }

                int idCita;
                std::cout << "Ingrese el ID de la cita a cancelar: ";
                std::cin >> idCita;
                limpiarBuffer();

                if (citaCtrl.cancelarCita(idCita)) {
                    std::cout << "✅ Cita cancelada correctamente.\n";
                } else {
                    std::cout << "❌ No se pudo cancelar la cita (¿ID incorrecto?).\n";
                }

                break;
            }


            case 6: {
                if (!usuario->puedeVerDatos()) {
                    std::cout << "❌ No tienes permisos para ver citas.\n";
                }else {
                    int filtro;
                    std::cout << "Mostrar citas:\n1. Activas\n2. Canceladas\nSeleccione una opción: ";
                    std::cin >> filtro;
                    std::cin.ignore();
                    if (filtro == 1 || filtro == 2)
                        citaCtrl.mostrarPorEstado(filtro == 1);
                    else
                        std::cout << "❌ Opción no válida.\n";
                }

            break;
        }

        case 0:
            std::cout << "Volviendo al menú principal...\n";
        break;
        default:
            std::cout << "❌ Opcion no valida.\n";
    }
    } while (opcionCita != 0);
}