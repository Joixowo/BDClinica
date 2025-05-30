#ifndef CITAS_MENU_H
#define CITAS_MENU_H

#include <memory>
#include "../Database/Usuarios.h"
#include "../controllers/CitaControllers.h"

void mostrarMenuCitas(std::shared_ptr<Usuarios> usuario, CitaController& citaCtrl);


#endif