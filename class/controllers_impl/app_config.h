#ifndef APP_CONFIGURACION_H
#define APP_CONFIGURACION_H

//std
#include <iostream>

//framework
#include <class/base_config.h>
#include <class/input.h>


namespace app
{

class app_config:
	public dfw::base_config
{
	////////////////////////////////
	// Interface pública.

	public:

	app_config();


	struct user_input
	{
		enum devs{keyboard=0, joystick=1, mouse=2, none=3};
		int type, device, code;
	};

	//Traduce el tipo del framework al de la configuración.
	static user_input fw_to_config(dfw::input::input_description& e)
	{
		int type=user_input::none;

		switch(e.type)
		{
			case dfw::input::input_description::types::keyboard: 	type=user_input::keyboard; break;
			case dfw::input::input_description::types::joystick: 	type=user_input::joystick; break;
			case dfw::input::input_description::types::mouse:	type=user_input::mouse; break;
			case dfw::input::input_description::types::none: 	type=user_input::none; break;
		}

		return user_input{type, e.device, e.code};
	};

	int get_w_screen_logical() const {return token_from_path("config:video:window_w_logical");}
	int get_h_screen_logical() const {return token_from_path("config:video:window_h_logical");}
	int get_w_screen_px() const {return token_from_path("config:video:window_w_px");}
	int get_h_screen_px() const {return token_from_path("config:video:window_h_px");}

	user_input get_left() const	{return token_from_path_input("left");}
	user_input get_right() const 	{return token_from_path_input("right");}
	user_input get_up() const 	{return token_from_path_input("up");}
	user_input get_down() const		{return token_from_path_input("down");}
	user_input get_space() const	{return token_from_path_input("space");}
	user_input get_zoom_more() const	{return token_from_path_input("zoom_more");}
	user_input get_zoom_less() const	{return token_from_path_input("zoom_less");}
	user_input get_num_1() const	{return token_from_path_input("num_1");}
	user_input get_num_2() const	{return token_from_path_input("num_2");}
	user_input get_num_3() const	{return token_from_path_input("num_3");}
	user_input get_key_a() const	{return token_from_path_input("key_a");}
	user_input get_key_s() const	{return token_from_path_input("key_s");}

	void set_w_screen_logical(int p_valor) {set("config:video:window_w_logical", p_valor);}
	void set_h_screen_logical(int p_valor) {set("config:video:window_h_logical", p_valor);}
	void set_w_screen_px(int p_valor) {set("config:video:window_w_px", p_valor);}
	void set_h_screen_px(int p_valor) {set("config:video:window_h_px", p_valor);}

	private:

	user_input token_from_path_input(const std::string& tipo) const
	{	
		const auto& tok=token_from_path("config:input:"+tipo);
		return user_input{tok[0], tok[1], tok[2]};
	}

	////////////////////////////////////
	// Implementacion...

	protected: 

	virtual std::string generate_file_version() const {return "1";}
	virtual std::string get_key_file_version() const {return "config:meta:v";}
	virtual std::string get_key_screen_double_buffer() const {return "config:video:double_buffer";}
	virtual std::string get_key_sound_volume() const {return "config:audio:sound_volume";}
	virtual std::string get_key_music_volume() const {return "config:audio:music_volume";}
	virtual std::string get_key_audio_ratio() const {return "config:audio:audio_ratio";}
	virtual std::string get_key_audio_buffers() const {return "config:audio:audio_buffers";}
	virtual std::string get_key_audio_out() const {return "config:audio:audio_out";}
	virtual std::string get_key_audio_channels() const {return "config:audio:audio_channels";}

	///////////////////////////////////
	// Propiedades.

	private:

	std::string get_file_path() const {return "data/config/config.dnot";}
};

}
#endif
