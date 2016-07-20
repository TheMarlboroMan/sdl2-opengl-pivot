#ifndef DFRAMEWORK_STATE_CONTROLLER_H
#define DFRAMEWORK_STATE_CONTROLLER_H

#include <exception>
#include <iostream>
#include <functional>

/*
* Pequeña clase de control de estados: básicamente controla con un entero el
* estado current y el estado al que se desea cambiar. Su única finalidad es
* eliminar dependencias cíclicas: tanto el director como los controladores
* dependen de esta clase.
*/

namespace dfw
{

class state_controller
{
	public:

	state_controller(int e, std::function<bool(int)> f)
		:f_validate(f), current(e), next(e)
	{}


	void					set(int v)
	{
		if(!f_validate(v)) throw std::runtime_error("invalid state for state_controller");
		next=v;
	}
	
	bool					is_change() const {return current!=next;}
	void					confirm() {current=next;}
	void					cancel() {next=current;}
	int					get_next() const {return next;}
	int					get_current() const {return current;}

	private:

	std::function<bool(int)>		f_validate;
	int 					current, 
						next;
};

}
#endif
