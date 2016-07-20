#include "resource_loader.h"
#include "kernel_config_interface.h"
#include <source/string_utils.h>

using namespace dfw;

resource_loader::resource_loader(ldv::resource_manager& vrm, lda::resource_manager& arm)
	:video_m(vrm), audio_m(arm), screen(nullptr)
{

}

void resource_loader::process(const std::vector<std::string>& entries, void (resource_loader::*process_values)(const std::vector<std::string>&))
{
	const char delim='\t';
	for(auto& line : entries)
	{
		try
		{
			(this->*process_values)(tools::explode(line, delim));
		}
		catch(std::exception& e)
		{
			throw std::runtime_error(e.what()+std::string(" in line ")+line);
		}
	}
}

void resource_loader::generate_textures(const std::vector<std::string>& resources)
{
	try
	{
		process(resources, &resource_loader::process_texture);
	}
	catch(std::exception& e)
	{
		throw std::runtime_error(std::string("unable to load textures: ")+e.what());
	}
}

void resource_loader::generate_surfaces(const std::vector<std::string>& resources, ldv::screen &p)
{
	screen=&p;
	try
	{
		process(resources, &resource_loader::process_surface);
	}
	catch(std::exception& e)
	{
		throw std::runtime_error(std::string("unable to load surfaces: ")+e.what());
	}
}

void resource_loader::generate_sounds(const std::vector<std::string>& resources)
{
	try
	{
		process(resources, &resource_loader::process_sound);
	}
	catch(std::exception& e)
	{
		throw std::runtime_error(std::string("unable to load sounds: ")+e.what());
	}
}

void resource_loader::generate_music(const std::vector<std::string>& resources)
{
	try
	{
		process(resources, &resource_loader::process_music);
	}
	catch(std::exception& e)
	{
		throw std::runtime_error(std::string("unable to load music: ")+e.what());
	}
}

void resource_loader::process_texture(const std::vector<std::string>& values)
{
	if(values.size()!=2) 
	{
		throw std::runtime_error("2 parameters expected for texture");
	}

	unsigned int indice=std::atoi(values[0].c_str());
	std::string ruta=values[1];

	ldv::image img(ruta);
	ldv::texture * t=new ldv::texture(img);
	video_m.insert(indice, t);
}

void resource_loader::process_surface(const std::vector<std::string>& values)
{
	if(values.size()!=6)
	{
		throw std::runtime_error("6 parameters expected for surface");
	}

	unsigned int indice=std::atoi(values[0].c_str());
	std::string ruta=values[1];
	unsigned int transparencia=std::atoi(values[2].c_str());

	SDL_Surface * surface=ldv::load_image(ruta, screen->get_window());
	if(transparencia)
	{
		unsigned int r=std::atoi(values[3].c_str());
		unsigned int g=std::atoi(values[4].c_str());
		unsigned int b=std::atoi(values[5].c_str());
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
	}
		
	ldv::image * t=new ldv::image(surface);
	video_m.insert(indice, t);
}


void resource_loader::process_sound(const std::vector<std::string>& values)
{
	if(values.size()!=2) 
	{
		throw std::runtime_error("2 parameters expected for sound");
	}

	unsigned int indice=std::atoi(values[0].c_str());
	std::string ruta=values[1];
	audio_m.insert_sound(indice, ruta);
}

void resource_loader::process_music(const std::vector<std::string>& values)
{
	if(values.size()!=2)
	{
		throw std::runtime_error("2 parameters expected for music");
	}

	unsigned int indice=std::atoi(values[0].c_str());
	std::string ruta=values[1];
	audio_m.insert_music(indice, ruta);
}
