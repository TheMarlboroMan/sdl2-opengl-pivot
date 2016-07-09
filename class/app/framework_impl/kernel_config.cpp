#include "kernel_config.h"
#include <SDL2/SDL.h>
#include "input.h"

#include <class/lector_txt.h>

#include <iostream>

using namespace App;

Kernel_config::Kernel_config(const App_config& c)
	:config(c)
{

}

std::vector<std::string> Kernel_config::obtener_entradas_audio() const
{
	return Herramientas_proyecto::obtener_entradas_lector_txt_desde_ruta(std::string("data/recursos/audio.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_musica() const
{
	return Herramientas_proyecto::obtener_entradas_lector_txt_desde_ruta(std::string("data/recursos/musica.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_texturas() const
{
	return Herramientas_proyecto::obtener_entradas_lector_txt_desde_ruta(std::string("data/recursos/texturas.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_superficies() const
{
	return Herramientas_proyecto::obtener_entradas_lector_txt_desde_ruta(std::string("data/recursos/superficies.txt"));
}

DFramework::Info_ventana Kernel_config::obtener_info_ventana() const
{
	return DFramework::Info_ventana{800, 400, 800, 400, "Migrating to OpenGL...", true};
}

std::vector<DFramework::Par_input> Kernel_config::obtener_pares_input() const
{
	auto tipo_desde_config=[](int t)
	{
		using namespace DFramework;

		switch(t)
		{
			case 0: return Par_input::tipos::teclado; break;
			case 1: return Par_input::tipos::joystick; break;
			case 2: return Par_input::tipos::raton; break;
		}

		return Par_input::tipos::teclado;
	};

	auto obtener_info_jugador=[this](std::map<int, App_config::input_jugador>& res, int iizq, int ider, int iarr, int iaba, int iesp, int zoommas, int zoommenos)
	{
		res[iizq]=config.acc_izquierda();
		res[ider]=config.acc_derecha();
		res[iarr]=config.acc_arriba();
		res[iaba]=config.acc_abajo();
		res[iesp]=config.acc_espacio();
		res[zoommas]=config.acc_zoom_mas();
		res[zoommenos]=config.acc_zoom_menos();
	};


	using namespace DFramework;
	std::vector<Par_input> res{
		Par_input{Par_input::tipos::teclado, Input::escape, SDL_SCANCODE_ESCAPE, 0}};

	std::map<int, App_config::input_jugador> mapa;
	obtener_info_jugador(mapa, Input::izquierda, Input::derecha, Input::arriba, Input::abajo, Input::espacio, Input::zoom_mas, Input::zoom_menos);
	for(const auto& p : mapa) res.push_back({tipo_desde_config(p.second.tipo), p.first, p.second.codigo, p.second.device});

	return res;
}
