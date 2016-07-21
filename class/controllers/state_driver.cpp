#include "state_driver.h"

#include <algorithm>
#include <class/dnot_parser.h>
#include <source/string_utilidades.h>

using namespace app;

extern DLibH::Log_base LOG;

state_driver::state_driver(DFramework::Kernel& kernel, App::App_config& c, DLibH::Log_base& log)
	:state_driver_interface(t_estados::principal, std::function<bool(int)>([](int v){return v > estado_min && v < estado_max;})),
	config(c), log(log)
{
	preparar_video(kernel);
	registrar_fuentes();
	registrar_controladores(kernel);
	virtualizar_joysticks(kernel.acc_input());
}

void state_driver::preparar_video(DFramework::Kernel& kernel)
{
	auto& pantalla=kernel.acc_pantalla();

	int wf=config.acc_w_fisica_pantalla(), 
		hf=config.acc_h_fisica_pantalla(),
		wl=config.acc_w_logica_pantalla(),
		hl=config.acc_h_logica_pantalla();

	pantalla.inicializar(wf, hf);
	pantalla.establecer_medidas_logicas(wl, hl);
	pantalla.establecer_modo_ventana(config.acc_modo_pantalla());
}

void state_driver::registrar_controladores(DFramework::Kernel& kernel)
{
	controlador_principal.reset(new Controlador_principal(log, fuentes));

	registrar_controlador(t_estados::principal, *controlador_principal);
}

void state_driver::preparar_cambio_estado(int deseado, int actual)
{
	switch(deseado)
	{
		case t_estados::principal: 
		break;
	}
}

void state_driver::input_comun(DFramework::Input& input, float delta)
{
	if(input.es_nuevo_joystick_conectado())
	{
		log<<"Detectado nuevo joystick..."<<std::endl;
		virtualizar_joysticks(input);
	}
}

void state_driver::paso_comun(float delta)
{

}

void state_driver::virtualizar_joysticks(DFramework::Input& input)
{
	for(int i=0; i < input.obtener_cantidad_joysticks(); ++i)
	{
		input.virtualizar_hats_joystick(i);
		input.virtualizar_ejes_joystick(i,15000);
		log<<"Virtualizado joystick "<<i<<std::endl;
	}
}

void state_driver::registrar_fuentes()
{	
	using namespace Herramientas_proyecto;

	auto v=obtener_entradas_lector_txt_desde_ruta("data/recursos/fuentes.txt");
	for(const auto& l : v)
	{
		auto p=Herramientas_proyecto::explotar(l, '\t');
		if(p.size()!=3)
		{
			throw std::runtime_error("Error al parsear fichero de fuentes");
		}
		else
		{
			fuentes.registrar_fuente(p[0], std::atoi( p[1].c_str() ), p[2] );
		}
	}	
}
