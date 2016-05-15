#ifdef WINCOMPIL
//Terrible parche para SDL2_Gfx...
#include <SDL2_gfxPrimitives.c>
#include <SDL2_rotozoom.c>
#include <SDL2_imageFilter.c>
#endif

#include <libDan2.h>
#include <defDanSDL.h>
#include "class/framework/kernel.h"
#include "class/app/framework_impl/kernel_config.h"
#include "class/app/framework_impl/app_config.h"
#include "class/controladores/director_estados.h"

//Declaración del log de aplicación en espacio global.
DLibH::Log_base LOG;

int main(int argc, char ** argv)
{
	using namespace App;

	//Inicializar control de logs.
	LOG.inicializar("logs/global.log");
	LOG.activar();
	DLibH::Log_motor::arrancar("logs/libdansdl2.log");

	//Inicializar control de argumentos.
	Herramientas_proyecto::Controlador_argumentos CARG(argc, argv);

	DLibH::Log_base log_app("logs/app.log");
	log_app<<"Iniciando proceso principal..."<<std::endl;

	//Intentar inicializar sin cargar aún la SDL...
	try
	{
		log_app<<"Inicializando configuración aplicación..."<<std::endl;
		App_config config;

		//Si la inicialización ha tenido éxito podemos arrancar la SDL y el Kernel.
		log_app<<"Inicializando configuración kernel..."<<std::endl;
		Kernel_config kernel_config(config);

		log_app<<"Inicializando SDL2..."<<std::endl;	
		if(!DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
		{
			throw std::runtime_error("No ha sido posible inicializar la librería SD2L");
		}
		
		log_app<<"Creando kernel..."<<std::endl;
		DFramework::Kernel kernel(CARG);

		log_app<<"Inicializando kernel..."<<std::endl;
		kernel.inicializar(kernel_config, config);

		log_app<<"Inicializando director de estados..."<<std::endl;
		App::Director_estados APP(kernel, config, log_app);
		APP.iniciar(kernel);

		log_app<<"Finalizando proceso principal."<<std::endl;
	}
	catch(std::exception& e)
	{
		log_app<<"Ha ocurrido un error "<<e.what()<<std::endl;
		std::cout<<"Abandonando aplicación debido a error : "<<e.what()<<std::endl;

		log_app<<"Finalizando SDL2..."<<std::endl;
		DLibH::Herramientas_SDL::apagar_SDL();
		DLibH::Log_motor::finalizar();

		return 1;
	}

	log_app<<"Finalizando SDL2..."<<std::endl;
	DLibH::Herramientas_SDL::apagar_SDL();
	DLibH::Log_motor::finalizar();
	return 0;
}
