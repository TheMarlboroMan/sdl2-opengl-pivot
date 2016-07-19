#ifndef KERNEL_DRIVER_INTERFACE
#define KERNEL_DRIVER_INTERFACE

#include <vector>
#include <string>
#include <stdexcept>

/**
* Esta es la interface que tendríamos que personalizar por cada aplicación 
* para arrancar el Kernel. La idea es dar un paso adelante y que sean menos
* archivos los que haya que manejar de un lado para otro.
* Controla los aspectos configurables del Kernel. No controla la configuración,
* que es una clase que puede tener tipos propios y que se usará aparte.
*/

namespace dfw
{

struct screen_info
{
	int 		px_w, px_h;
	int 		logical_w, logical_h;
	std::string 	name;
	bool 		show_cursor;
};

struct input_pair
{
	enum class 	types {none, keyboard, mouse, joystick};
	types 		type;
	int 		key, sdl_key, device_index;
};

class kernel_driver_interface
{
	public:

	//Carga de recursos.
	virtual std::vector<std::string> 	get_audio_entries() const=0;
	virtual std::vector<std::string> 	get_music_entries() const=0;
	virtual std::vector<std::string> 	get_texture_entries() const=0;
	virtual std::vector<std::string> 	get_surface_entries() const=0;

	//Información para ventana.
	virtual screen_info			get_window_info() const=0;

	//Información del input.
	virtual std::vector<Par_input> 		get_input_pairs() const=0;
};

}

#endif
