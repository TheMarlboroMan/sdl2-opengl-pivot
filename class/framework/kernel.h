#ifndef DFRAMEWORK_KERNEL_H
#define DFRAMEWORK_KERNEL_H

#include <string>
#include "kernel_driver_interface.h"
#include "base_config.h"
#include "input_i.h"
#include "audio.h"
#include "resource_loader.h"
#include <class/arg_manager.h>

/**
* El kernel es propietario de los recursos y la interface de input_i. No es
* propietario de la configuración, que puede variar para cada proyecto.
*/

namespace dfw
{

class kernel
{
	public:

				kernel(tools::arg_manager&);
	int			get_fps() const {return fps_counter_i.get_frame_count();}
	void 			init(const kernel_driver_interface&, const base_config&);

	input&			get_input() {return input_i;}
	ldv::screen&	 	get_screen() {return screen_i;}
	tools::arg_manager& 	get_arg_manager() {return arg_manager_i;}

	float 			get_delta_step() const {return delta_step;}
	void 			set_delta_step(float v) {delta_step=v;}
	void			do_fps_count() {fps_counter_i.tic();}
	void			init_loop_step() {fps_counter_i.init_loop_step();}
	bool			consume_loop(float delta) {return fps_counter_i.consume_loop(delta);}
	void			do_audio_queue() {
	//TODO TODO TODO
	//Audio::procesar_cola_sonido();	
		throw std::runtime_error("DO AUDIO QUEUE IS MISSING STILL");
	
	}


	///////////////////
	// Propiedades

	private:

	float delta_step;

	tools::arg_manager& 				arg_manager_i;
	ldt::fps_counter	 			fps_counter_i;
	ldv::screen 					screen_i;
	input						input_i;

	///////////////////////////
	// Internos...

	private:

	void 			init_video_environment(const info_screen&);
	void 			init_audio_environment(const base_config& config);
};

}
#endif
