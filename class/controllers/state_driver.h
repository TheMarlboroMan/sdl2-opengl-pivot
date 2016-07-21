#ifndef APP_STATE_DRIVER_H
#define APP_STATE_DRIVER_H

#include <memory>

//TODO: COMPLETELY SEPARATE THE FRAMEWORK INTO A DIFFERENT PROJECT.

#include "../framework/state_driver_interface.h"
#include "../app/framework_impl/app_config.h"

//Estados y controladores.
#include "states.h"
#include "main.h"

//Específicos de la aplicación.
#include <tools/log/log.h>
#include <class/ttf_manager.h>

namespace app
{

class state_driver:
	public dfw::state_driver_interface
{
	public:

							state_driver(dfw::kernel& kernel, app::app_config& config, ldt::log&);
	
	virtual void					common_input(dfw::input& input, float delta);
	virtual void					common_step(float delta);
	virtual void					prepare_state(int, int);

	private:

	void						prepare_video(dfw::kernel& kernel);
	void						register_controllers(dfw::kernel& kernel);
	void						register_fonts();
	void 						virtualize_input(dfw::input& input);

	app::app_config&				config;
	ldt::log&					log;

	tools::ttf_manager				fonts;
	
	std::unique_ptr<main_controller>		c_main;
};

}
#endif
