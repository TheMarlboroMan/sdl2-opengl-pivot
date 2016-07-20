#ifndef DFRAMEWORK_CONTROLLER_INTERFACE_H
#define DFRAMEWORK_CONTROLLER_INTERFACE_H

#include "messages.h"
#include "state_controller.h"
#include "input.h"
#include <video/screen/screen.h>

/**
* Interface para un controlador. Los controladores deben ser registrados en
* el director de estados para que los cambios de estado sean posibles y 
* pueda inyectarse la cola de eventos.
*/

namespace dfw
{

class controller_interface
{
	public:

	controller_interface()
		:message_queue_instance(nullptr), states(nullptr), leave(false), break_loop(false) 
	{}

	virtual ~controller_interface() 
	{}

	void 				set_debug(const std::string& c) {debug=c;}
	const std::string& 		get_debug() const {return debug;}

	bool 				is_leave() const {return leave;}
	bool 				is_break_loop() const {return break_loop;}

	void 				set_leave(bool v) {leave=v;}
	void 				set_break_loop(bool v) {break_loop=v;}

	void				inject_message_queue(message_queue& c) {message_queue_instance=&c;}
	void				inject_state_controller(state_controller& c) {states=&c;}

	void				queue_message(dfw::uptr_message& ev) {message_queue_instance->insert(ev);}
	void				consume_message(dfw::uptr_message& ev) {message_queue_instance->consume(ev);}

	void				set_state(int v) {states->set(v);}

	virtual void 			preloop(input&, float delta)=0;
	virtual void 			loop(input&, float delta)=0;
	virtual void 			postloop(input&, float delta)=0;
	virtual void 			draw(ldv::screen&)=0;
	virtual void 			awake()=0;
	virtual void 			slumber()=0;
	virtual bool			can_leave_state() const=0;

	private:

	message_queue *			message_queue_instance;
	state_controller *		states;
	std::string 			debug;
	bool 				leave,
					break_loop;
};

}
#endif
