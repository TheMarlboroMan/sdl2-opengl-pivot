#ifndef DFRAMEWORK_KERNEL_H
#define DFRAMEWORK_KERNEL_H

#include <string>
#include <memory>
#include "kernel_config_interface.h"
#include "base_config.h"
#include "input.h"
#include "audio.h"
#include "resource_loader.h"
#include <class/arg_manager.h>
#include <input/sdl_input/sdl_input.h>

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
	void 			init(const kernel_config_interface&, const base_config&);

	input&			get_input() {return input_i;}
	audio&			get_audio() {return *audio_i;}
	ldv::screen&	 	get_screen() {return screen_i;}
	ldv::resource_manager&	get_video_resource_manager() {return v_manager;}
	lda::resource_manager&	get_audio_resource_manager() {return a_manager;}
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

	void 			init_video_environment(const window_info&);
	void 			init_audio_environment(const base_config& config);	

	float delta_step;
	
	std::unique_ptr<lda::audio_controller>		audiocontroller;
	ldi::sdl_input	 				sdlinput;
	ldv::resource_manager				v_manager;
	lda::resource_manager				a_manager;
	ldt::fps_counter	 			fps_counter_i;
	ldv::screen 					screen_i;

	input						input_i;
	std::unique_ptr<audio>				audio_i;

	tools::arg_manager& 				arg_manager_i;
};

}
#endif
