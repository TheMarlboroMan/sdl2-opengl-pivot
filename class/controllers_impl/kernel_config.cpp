#include "kernel_config.h"

//std
#include <iostream>

//sdl
#include <SDL2/SDL.h>

//libdansdl2
#include <input/sdl_input/sdl_input.h>

//tools
#include <class/text_reader.h>

//local
#include "input.h"

using namespace app;

kernel_config::kernel_config(const app_config& c)
	:config(c)
{

}

std::vector<std::string> kernel_config::get_sound_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/audio.txt"));
}

std::vector<std::string> kernel_config::get_music_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/music.txt"));
}

std::vector<std::string> kernel_config::get_texture_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/textures.txt"));
}

std::vector<std::string> kernel_config::get_surface_entries() const
{
	return tools::explode_lines_from_file(std::string("data/resources/surfaces.txt"));
}

dfw::window_info kernel_config::get_window_info() const
{
	return dfw::window_info{800, 400, 800, 400, "Migrating to OpenGL...", true};
}

std::vector<dfw::input_pair> kernel_config::get_input_pairs() const
{
	auto type_from_config=[](int t)
	{
		using namespace dfw;

		switch(t)
		{
			case 0: return input_pair::types::keyboard; break;
			case 1: return input_pair::types::joystick; break;
			case 2: return input_pair::types::mouse; break;
		}

		return input_pair::types::keyboard;
	};

	auto get_user_info=[this](std::map<int, app_config::user_input>& res, int iizq, int ider, int iarr, int iaba, int iesp, int zoommas, int zoommenos, int n1, int n2, int n3, int ka, int ks)
	{
		res[iizq]=config.get_left();
		res[ider]=config.get_right();
		res[iarr]=config.get_up();
		res[iaba]=config.get_down();
		res[iesp]=config.get_space();
		res[zoommas]=config.get_zoom_more();
		res[zoommenos]=config.get_zoom_less();
		res[n1]=config.get_num_1();
		res[n2]=config.get_num_2();
		res[n3]=config.get_num_3();
		res[ka]=config.get_key_a();
		res[ks]=config.get_key_s();
	};

	using namespace dfw;
	std::vector<input_pair> res{
		input_pair{input_pair::types::keyboard, input_app::escape, SDL_SCANCODE_ESCAPE, 0}};

	std::map<int, app_config::user_input> mapa;

	get_user_info(mapa, input_app::left, input_app::right, input_app::up, input_app::down, input_app::space, input_app::zoom_more, input_app::zoom_less, input_app::num1, input_app::num2, input_app::num3, input_app::key_a, input_app::key_s);
	for(const auto& p : mapa) res.push_back({type_from_config(p.second.type), p.first, p.second.code, p.second.device});

	return res;
}
