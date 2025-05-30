#ifndef MEDICOS_MENU_H
#define MEDICOS_MENU_H

#include <memory>
#include "../Database/Usuarios.h"
#include "../controllers/MedicoControllers.h"

void mostrarMenuMedicos(std::shared_ptr<Usuarios> usuario, MedicoController& medicoCtrl);

#endif