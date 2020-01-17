#pragma once

//local.
#include "app_config.h"

//Controllers.
#include "../controllers/main.h"

//Framework
#include <dfw/state_driver_interface.h>

//std
#include <memory>

namespace app {

class state_driver:
	public dfw::state_driver_interface {

	public:
							state_driver(dfw::kernel& kernel, app::app_config& config);

	virtual void					common_pre_loop_input(dfw::input& input, float delta);
	virtual void					common_pre_loop_step(float delta);
	virtual void					common_loop_input(dfw::input& input, float delta);
	virtual void					common_loop_step(float delta);
	virtual void					prepare_state(int, int);
	virtual float					get_max_timestep() const {return 0.03f;}

	private:

	void						prepare_video(dfw::kernel&);
	void						prepare_audio(dfw::kernel&);
	void						prepare_input(dfw::kernel&);
	void						prepare_resources(dfw::kernel&);
	void						register_controllers(dfw::kernel&);
	void 						virtualize_input(dfw::input& input);

	//references
	app::app_config&				config;
	lm::logger&					log;

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;

	//controller instances here.
	ptr_controller						c_main;

	ldtools::ttf_manager				fonts;
};

}
