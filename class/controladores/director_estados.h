#ifndef DIRECTOR_ESTADOS
#define DIRECTOR_ESTADOS

#include <memory>

#include "../framework/director_estados_interface.h"
#include "../app/framework_impl/app_config.h"

//Estados y controladores.
#include "estados_controladores.h"
#include "principal.h"

//Específicos de la aplicación.
#include <herramientas/log_base/log_base.h>
#include <class/gestor_fuentes_ttf.h>

namespace App
{

class Director_estados:
	public DFramework::Director_estados_interface
{
	public:

							Director_estados(DFramework::Kernel& kernel, App::App_config& config, DLibH::Log_base&);
	
	virtual void					input_comun(DFramework::Input& input, float delta);
	virtual void					paso_comun(float delta);
	virtual void					preparar_cambio_estado(int deseado, int actual);

	private:

	void						preparar_video(DFramework::Kernel& kernel);
	void						registrar_controladores(DFramework::Kernel& kernel);
	void						registrar_fuentes();
	void 						virtualizar_joysticks(DFramework::Input& input);

	App::App_config&				config;
	DLibH::Log_base&				log;

	Herramientas_proyecto::Gestor_fuentes_TTF	fuentes;
	
	std::unique_ptr<Controlador_principal>		controlador_principal;
};

}
#endif
