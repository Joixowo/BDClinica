#include "citas_menu.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

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
        std::cout << "4. Buscar Cita por nombre\n";
        std::cout << "5. Buscar Cita por fecha\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcionCita;
        std::cin.ignore();

        switch (opcionCita) {
            case 1: {
                if (!usuario->puedeCrearCita()) {
                    std::cout << "❌ No tienes permisos para crear citas.\n";
                    break;
                }
                int id_paciente, id_medico;
                std::string fecha, motivo;
                std::string input;

                // ... (tu codigo actual para crear cita con validacion) ...
                do {
                    std::cout << "ID del paciente: ";
                    std::getline(std::cin, input);
                    if (input.empty()) {
                        std::cout << "❌ Este campo no puede estar vacio.\n";
                        continue;
                    }
                    try {
                        id_paciente = std::stoi(input);
                        break;
                    } catch (...) {
                        std::cout << "❌ Por favor ingresa un número valido.\n";
                    }
                } while (true);

                do {
                    std::cout << "ID del medico: ";
                    std::getline(std::cin, input);
                    if (input.empty()) {
                        std::cout << "❌ Este campo no puede estar vacio.\n";
                        continue;
                    }
                    try {
                        id_medico = std::stoi(input);
                        break;
                    } catch (...) {
                        std::cout << "❌ Por favor ingresa un número valido.\n";
                    }
                } while (true);

                do {
                    std::cout << "Fecha y hora (YYYY-MM-DD HH:MM:SS): ";
                    std::getline(std::cin, fecha);
                    if (fecha.empty()) {
                        std::cout << "❌ Este campo no puede estar vacio.\n";
                    }
                } while (fecha.empty());

                do {
                    std::cout << "Motivo: ";
                    std::getline(std::cin, motivo);
                    if (motivo.empty()) {
                        std::cout << "❌ Este campo no puede estar vacio.\n";
                    }
                } while (motivo.empty());

                citaCtrl.agregar(id_paciente, id_medico, fecha, motivo);
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

            std::string nuevaFecha, nuevaHora;
            std::cout << "Ingrese la nueva fecha (YYYY-MM-DD): ";
            std::getline(std::cin, nuevaFecha);
            std::cout << "Ingrese la nueva hora (HH:MM:SS): ";
            std::getline(std::cin, nuevaHora);

            std::string nuevaFechaHoraCompleta = nuevaFecha + " " + nuevaHora;

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
                }
                std::string nombrePacienteBusqueda;
                std::vector<Cita> citasEncontradas;
                std::cout << "Ingrese el nombre del paciente a buscar: ";
                std::getline(std::cin, nombrePacienteBusqueda);
                citasEncontradas = citaCtrl.buscarCitasPorNombrePaciente(nombrePacienteBusqueda);
                if (!citasEncontradas.empty()) {
                    std::cout << "\nCitas encontradas para el paciente '" << nombrePacienteBusqueda << "':\n";
                    for (const auto& cita : citasEncontradas) {
                        std::cout << "ID: " << cita.getId()
                        << ", Paciente ID: " << cita.getIdPaciente()
                        << ", Fecha: " << cita.getFecha()
                        << ", Hora: " << cita.getHora()
                        << ", Motivo: " << cita.getMotivo() << std::endl;
                    }
                } else {
                    std::cout << "No se encontraron citas para el paciente '" << nombrePacienteBusqueda << "'.\n";
                }
            }
                break;
            case 5: {
                std::vector<Cita> citasEncontradas;
                std::string fechaBusqueda;
                if (!usuario->puedeBuscarCita()) {
                    std::cout << "❌ No tienes permisos para buscar citas.\n";
                }
                std::cout << "Ingrese la fecha a buscar (YYYY-MM-DD): ";
                std::getline(std::cin, fechaBusqueda);
                citasEncontradas = citaCtrl.buscarCitasPorFecha(fechaBusqueda);
                if (!citasEncontradas.empty()) {
                    std::cout << "\nCitas encontradas para la fecha '" << fechaBusqueda << "':\n";
                    for (const auto& cita : citasEncontradas) {
                        std::cout << "ID: " << cita.getId()
                        << ", Paciente ID: " << cita.getIdPaciente()
                        << ", Fecha: " << cita.getFecha()
                        << ", Hora: " << cita.getHora()
                        << ", Motivo: " << cita.getMotivo() << std::endl;
                    }
                } else {
                    std::cout << "No se encontraron citas para la fecha '" << fechaBusqueda << "'.\n";
                }
            }
            break;
        case 0:
            std::cout << "Volviendo al menú principal...\n";
        break;
        default:
            std::cout << "❌ Opcion no valida.\n";
    }
    } while (opcionCita != 0);
}