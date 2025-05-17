cmake_minimum_required(VERSION 3.15)
project(ProyectoPrograma)

set(CMAKE_CXX_STANDARD 20)



add_executable(ProyectoPrograma
        main.cpp
        ODBCConnect/ODBCConnection.cpp
        Database/Cita.cpp
        controllers/CitaControllers.cpp
        Database/Medico.cpp
        controllers/MedicoControllers.cpp
        Database/Paciente.cpp
        controllers/PacienteController.cpp
        Database/Usuarios.cpp
        Database/Usuarios.h
        Database/Especialidades.cpp
        Database/Especialidades.h
        controllers/especialidadescontrollers.cpp
        controllers/especialidadescontrollers.h
        Menus/medicos_menu.cpp
        Menus/medicos_menu.h
        Menus/pacientes_menu.cpp
        Menus/pacientes_menu.h
        Menus/citas_menu.cpp
        Menus/citas_menu.h
        Menus/especialidades_menu.cpp
        Menus/especialidades_menu.h
        Menus/factura.cpp
        Menus/factura.h

)

target_link_libraries(ProyectoPrograma odbc32)
