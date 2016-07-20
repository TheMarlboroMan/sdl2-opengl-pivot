#ifndef DFRAMEWORK_STATE_DRIVER_INTERFACE_H
#define DFRAMEWORK_STATE_DRIVER_INTERFACE_H

#include <stdexcept>
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include "messages.h"
#include "state_controller.h"
#include "controller_interface.h"
#include "kernel.h"

/**
* Interface para el director de estados que es, en pocas palabras, la aplicación
* en si. Todos los recursos ajenos al Kernel deberían ser propiedad de un 
* director de estados, que los gestiona y los pasa a sus controladores.
*/

namespace dfw
{

class state_driver_interface
{
	public:

						state_driver_interface(int e, std::function<bool(int)> f);

	virtual					~state_driver_interface() 
	{}

	//Este método se ejecutará justo antes de confirmar un cambio de estado. Se 
	//puede usar para intercambiar información entre controladores justo antes
	//de despertar a uno nuevo. Se ejecuta antes de poner a dormir el estado
	//original y despertar el nuevo.

	virtual void				prepare_state(int next, int current)=0;
	virtual void				common_input(dfw::input&, float delta)=0;
	virtual void				common_step(float delta)=0;

	void					init(dfw::kernel&);
	void					register_controller(int, controller_interface&);
	void					register_message_reader(message_reader_interface& i);

	protected:
	
	std::map<int, controller_interface *>	controllers;
	state_controller			states;
	message_queue				message_q;

	private:

	bool					loop(dfw::kernel&);
	controller_interface *			ci;
	message_reader_interface *		mri;

};


}
#endif
