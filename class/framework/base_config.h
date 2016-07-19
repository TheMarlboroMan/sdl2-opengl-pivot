#ifndef PROYECTO_SDL2_BASE_CONFIGURACION_H
#define PROYECTO_SDL2_BASE_CONFIGURACION_H

/*
* Esta clase lee y escribe en un archivo los datos de configuración de la 
* aplicación. Al leer los datos de configuración los almacena entre sus 
* propiedades. Podemos (y debemos) extenderla para la configuración de cada
* proyecto implementando todos los métodos virtuales puros que tiene.
*/

#include <class/dnot_parser.h>
#include <iostream>

namespace dfw
{

class base_config
{
	////////////////////////////////
	// Interface pública.

	public:

	int get_file_version() const		{return token_from_path(get_key_file_version());}
	int get_screen_double_buffer() const 	{return token_from_path(get_key_screen_double_buffer());}
	int get_audio_volume() const		{return token_from_path(get_key_audio_volume());}
	int get_music_volume() const		{return token_from_path(get_key_music_volume());}
	int get_audio_ratio() const		{return token_from_path(get_key_audio_ratio());}
	int get_audio_out() const		{return token_from_path(get_key_audio_out());}
	int get_audio_buffers() const		{return token_from_path(get_key_audio_buffers());}
	int get_audio_channels() const		{return token_from_path(get_key_audio_channels());}

	void set_screen_double_buffer(int p_valor)	{configurar(get_key_screen_double_buffer(), p_valor);}
	void set_audio_volume(int p_valor)		{configurar(get_key_audio_volume(), p_valor);}
	void set_music_volume(int p_valor)		{configurar(get_key_music_volume(), p_valor);}
	void set_audio_ratio(int p_valor)		{configurar(get_key_audio_ratio(), p_valor);}
	void set_audio_out(int p_valor)			{configurar(get_key_audio_out(), p_valor);}
	void set_audio_buffers(int p_valor)		{configurar(get_key_audio_buffers(), p_valor);}
	void set_audio_channels(int p_valor)		{configurar(get_key_audio_channels(), p_valor);}

	void save();

	base_config(const std::string& ruta);

	protected: 

	//Conversor de lo que sea a string...
	template <typename T>
	void 						set(const std::string& clave, T v)
	{
		auto& tok=token_from_path(clave);
		tok.set(v);
	}

	const tools::dnot_token&			token_from_path(const std::string& c) const;
	tools::dnot_token&				token_from_path(const std::string& c);

	////////////////////////////////////
	// A implementar por clases que la extiendan...

		//Estos simplemente devuelven claves de configuración.
	virtual std::string 				get_key_file_version() const=0;
	virtual std::string 				get_file_version() const=0;
	virtual std::string 				get_key_screen_double_buffer() const=0;
	virtual std::string 				get_key_audio_volume() const=0;
	virtual std::string 				get_key_music_volume() const=0;
	virtual std::string 				get_key_audio_ratio() const=0;
	virtual std::string 				get_key_audio_buffers() const=0;
	virtual std::string 				get_key_audio_out() const=0;
	virtual std::string 				get_key_audio_channels() const=0;

	///////////////////////////////////
	// Propiedades.

	private:	

	tools::dnot_token				token;
	const std::string				path;
};

}
#endif
