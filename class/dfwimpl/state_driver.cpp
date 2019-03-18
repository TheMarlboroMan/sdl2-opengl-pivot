#include "state_driver.h"

//std
#include <algorithm>

//tools
#include <class/dnot_parser.h>
#include <source/string_utils.h>
#include "../input.h"
#include "../controllers/states.h"

using namespace app;

extern tools::log LOG;

state_driver::state_driver(dfw::kernel& kernel, app::app_config& c)
	:state_driver_interface(t_states::state_main),
	config(c), log(kernel.get_log()) {

	log<<"setting state check function..."<<std::endl;
	states.set_function([](int v){
		return v > state_min && v < state_max;
	});

	log<<"init state driver building: preparing video..."<<std::endl;
	prepare_video(kernel);

	log<<"preparing audio..."<<std::endl;
	prepare_audio(kernel);

	log<<"preparing input..."<<std::endl;
	prepare_input(kernel);

	log<<"preparing resources..."<<std::endl;
	prepare_resources(kernel);

	log<<"registering controllers..."<<std::endl;
	register_controllers(kernel);

	log<<"virtualizing input..."<<std::endl;
	virtualize_input(kernel.get_input());

	log<<"state driver fully constructed"<<std::endl;
}

void state_driver::prepare_video(dfw::kernel& kernel) {

	kernel.init_video_system({
		config.int_from_path("config:video:window_w_px"),
		config.int_from_path("config:video:window_h_px"),
		config.int_from_path("config:video:window_w_logical"),
		config.int_from_path("config:video:window_h_logical"),
		config.string_from_path("config:video:window_title"),
		config.bool_from_path("config:video:window_show_cursor"),
		config.get_screen_vsync()
	});

	auto& screen=kernel.get_screen();
	screen.set_fullscreen(config.bool_from_path("config:video:fullscreen"));

	log<<"registering fonts..."<<std::endl;

	using namespace tools;
	auto v=explode_lines_from_file("data/resources/fonts.txt");
	for(const auto& l : v) {
		auto p=explode(l, '\t');
		if(p.size()!=3) {
			throw std::runtime_error("unable to parse font file: 3 parameters expected");
		}

		fonts.insert(p[0], std::atoi( p[1].c_str() ), p[2] );
	}
}

void state_driver::prepare_audio(dfw::kernel& kernel) {

	kernel.init_audio_system({
		config.get_audio_ratio(),
		config.get_audio_out(),
		config.get_audio_buffers(),
		config.get_audio_channels(),
		config.get_audio_volume(),
		config.get_music_volume()
	});
}

void state_driver::prepare_input(dfw::kernel& kernel) {

	using namespace dfw;

	std::vector<input_pair> pairs{
		{{input_description::types::keyboard, SDL_SCANCODE_ESCAPE, 0}, input_app::escape},
		{input_description_from_config_token(config.token_from_path("config:input:left")), input_app::left},
		{input_description_from_config_token(config.token_from_path("config:input:right")), input_app::right},
		{input_description_from_config_token(config.token_from_path("config:input:up")), input_app::up},
		{input_description_from_config_token(config.token_from_path("config:input:down")), input_app::down},
		{input_description_from_config_token(config.token_from_path("config:input:space")), input_app::space},
		{input_description_from_config_token(config.token_from_path("config:input:zoom_more")), input_app::zoom_more},
		{input_description_from_config_token(config.token_from_path("config:input:zoom_less")), input_app::zoom_less},
		{input_description_from_config_token(config.token_from_path("config:input:num_1")), input_app::num1},
		{input_description_from_config_token(config.token_from_path("config:input:num_2")), input_app::num2},
		{input_description_from_config_token(config.token_from_path("config:input:num_3")), input_app::num3},
		{input_description_from_config_token(config.token_from_path("config:input:key_a")), input_app::key_a},
		{input_description_from_config_token(config.token_from_path("config:input:key_s")), input_app::key_s}
	};

	kernel.init_input_system(pairs);
}

void state_driver::prepare_resources(dfw::kernel& kernel) {

	dfw::resource_loader r_loader(kernel.get_video_resource_manager(), kernel.get_audio_resource_manager());

	r_loader.generate_textures(tools::explode_lines_from_file(std::string("data/resources/textures.txt")));
	r_loader.generate_sounds(tools::explode_lines_from_file(std::string("data/resources/audio.txt")));
	r_loader.generate_music(tools::explode_lines_from_file(std::string("data/resources/music.txt")));
}

void state_driver::register_controllers(dfw::kernel& kernel) {

	auto reg=[this](ptr_controller& _ptr, int _i, dfw::controller_interface * _ci) {
		_ptr.reset(_ci);
		register_controller(_i, *_ptr);
	};

	//register controllers here.
	reg(c_main, t_states::state_main,
		new main_controller(
			kernel.get_video_resource_manager(),
			kernel.get_audio_resource_manager(),
			log,
			fonts,
			kernel.get_audio()
		)
	);
}

void state_driver::prepare_state(int /*next*/, int /*current*/) {

/*
	switch(next) {
		case t_states::state_placeholder:
		break;
		default:
		break;
	}
*/
}

void state_driver::common_pre_loop_input(dfw::input& input, float /*delta*/) {

	if(input().is_event_joystick_connected()) {
		log<<"New joystick detected..."<<std::endl;
		virtualize_input(input);
	}
}

void state_driver::common_loop_input(dfw::input& /*input*/, float /*delta*/) {

}

void state_driver::common_pre_loop_step(float /*delta*/) {

}

void state_driver::common_loop_step(float /*delta*/) {

}

void state_driver::virtualize_input(dfw::input& input) {

	log<<"trying to virtualize "<<input().get_joysticks_size()<<" controllers..."<<std::endl;

	for(size_t i=0; i < input().get_joysticks_size(); ++i) {
		input().virtualize_joystick_hats(i);
		input().virtualize_joystick_axis(i, 15000);
		log<<"Joystick virtualized "<<i<<std::endl;
	}
}
