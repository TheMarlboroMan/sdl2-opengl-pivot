#include "state_driver_interface.h"

using namespace dfw;

state_driver_interface::state_driver_interface(int e, std::function<bool(int)> f)
	:states(e, f), ci(nullptr), mri(nullptr)
{

}

void state_driver_interface::init(dfw::kernel& kernel)
{
	if(!controllers.size())
	{
		throw std::runtime_error("no controllers registered.");
	}

	if(!controllers.count(states.get_current()))
	{
		throw std::runtime_error("initial controller is unregistered.");
	}

	ci=controllers[states.get_current()];
	ci->awake();
	while(loop(kernel));
}

void state_driver_interface::register_controller(int index, controller_interface& controller)
{
	if(controllers.count(index))
	{
		throw std::runtime_error("duplicate index for controller");
	}
	
	controllers[index]=&controller;
	controller.inject_message_queue(message_q);
	controller.inject_state_controller(states);
}

bool state_driver_interface::loop(dfw::kernel& kernel)
{
	//Aquí se mide el tiempo desde el último paso por este loop...
	kernel.init_loop_step();
	float delta_step=kernel.get_delta_step();

	auto& input_i=kernel.get_input();

	common_step(delta_step);
	common_input(input_i, delta_step);

	ci->preloop(input_i, delta_step);

	//Aquí se consume el tiempo desde el último paso en bloques de "delta_step".
	while(kernel.consume_loop(delta_step))
	{
		input_i().loop();

		ci->loop(input_i, delta_step);

		kernel.do_audio_queue();

		if(ci->is_break_loop()) 
		{
			break;
		}

		if(states.is_change()) 
		{
			if(!ci->can_leave_state())
			{
				states.cancel();
			}
			else
			{
				prepare_state(states.get_next(), states.get_current());
				if(mri!=nullptr && message_q.size()) message_q.process(*mri);
				break;
			}
		}
	
		if(mri!=nullptr && message_q.size()) message_q.process(*mri);
	}

	if(states.is_change())
	{
		//Confirmación del cambio de states.
		int current=states.get_current();
		int next=states.get_next();

		controllers[current]->slumber();
		controllers[next]->awake();

		ci=controllers[next];

		states.confirm();
	}
	else
	{
		ci->postloop(input_i, delta_step);

		kernel.do_fps_count();

		ci->draw(kernel.get_screen());

		kernel.get_screen().update();
	}

	return !ci->is_leave();
}

void state_driver_interface::register_message_reader(message_reader_interface& i)
{
	mri=&i;
	message_q.set_reader(i);
}
