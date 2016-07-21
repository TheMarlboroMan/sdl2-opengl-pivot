#ifndef APP_STATE_DRIVER_H
#define APP_STATE_DRIVER_H

#include <memory>

#include "../framework/state_driver_interface.h"
#include "../app/framework_impl/app_config.h"

//Estados y controladores.
#include "state.h"
#include "main.h"

//Específicos de la aplicación.
#include <tools/log/log.h>
#include <class/ttf_manager.h>

namespace app
{

class state_driver:
	public DFramework::state_driver_interface
{
	public:

							state_driver(DFramework::Kernel& kernel, App::App_config& config, DLibH::Log_base&);
	
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
