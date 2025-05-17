#ifndef PACIENTES_MENU_H
#define PACIENTES_MENU_H

#include <memory>
#include "../Database/Usuarios.h"
#include "../controllers/PacienteController.h"

void mostrarMenuPacientes(std::shared_ptr<Usuarios> usuario, PacienteController& pacienteCtrl);

#endif