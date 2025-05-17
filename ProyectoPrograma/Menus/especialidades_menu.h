#ifndef ESPECIALIDADES_MENU_H
#define ESPECIALIDADES_MENU_H

#include <memory>
#include "../Database/Usuarios.h"
#include "../controllers/especialidadescontrollers.h"

void mostrarMenuEspecialidades(std::shared_ptr<Usuarios> usuario, EspecialidadesController& especialidadesCtrl);

#endif