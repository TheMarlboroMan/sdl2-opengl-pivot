#ifndef APP_STATE_DRIVER_H
#define APP_STATE_DRIVER_H

//std
#include <memory>

//Framework
#include <class/state_driver_interface.h>

//Tools
#include <class/ttf_manager.h>

//local.
#include "app_config.h"
#include "states.h"
#include "main.h"


namespace app
{

class state_driver:
	public dfw::state_driver_interface
{
	public:

							state_driver(dfw::kernel& kernel, app::app_config& config);
	
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
