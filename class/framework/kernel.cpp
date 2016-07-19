#include "kernel.h"
#include <cstdlib>

using namespace dfw;

kernel::kernel(tools::arg_manager& carg)
	:delta_step(0.01), arg_manager_i(carg), 
	fps_counter_i(), screen_i()
{

}

void kernel::init(const kernel_driver_interface& kdi, const base_config& config)
{
	init_video_environment(kdi.get_window_info());
	init_audio_environment(config);

	//Inicializar recursos.
	resource_loader r_loader;
	r_loader.generar_recursos_texturas(kdi.get_textures());
	r_loader.generar_recursos_superficies(kdi.get_surfaces(), screen_i);
	r_loader.generar_recursos_audio(kdi.get_sounds());
	r_loader.generar_recursos_musica(kdi.get_musics());
	
	//Inicializar controles.
	input_i.configure(kdi.get_input()); 

	//Inicialización controlador tiempo.
	fps_counter_i.init();
}

//Inicialización con valores hardcodeados.
void kernel::init_video_environment(const info_screen& iv)
{
	screen_i.init(iv.px_w, iv.px_h);
	screen_i.set_logical_size(iv.logical_w, iv.logical_h);
	screen_i.set_title(iv.title);
	ldv::set_cursor(iv.show_cursor);
}

void kernel::init_audio_environment(const base_config& config)
{
	throw std::runtime_error("AUDIO ENV MUST CHANGE!!!");
/*
	Audio::init_entorno_audio(
		config.acc_audio_ratio(), 
		config.acc_audio_salidas(),
		config.acc_audio_buffers(),
		config.acc_audio_canales());

	Audio::establecer_volumen(config.acc_volumen_audio());
	Audio::establecer_volumen_musica(config.acc_volumen_musica());
*/
}
