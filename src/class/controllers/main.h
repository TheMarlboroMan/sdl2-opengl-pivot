#pragma once

//Local.
#include "states.h"

//Tools.
#include <ldtools/ttf_manager.h>
#include <tools/compatibility_patches.h>

//Framework.
#include <dfw/controller_interface.h>
#include <dfw/audio.h>

//dansdl2
#include <ldv/screen.h>
#include <ldv/resource_manager.h>
#include <ldv/ttf_font.h>
#include <ldv/camera.h>
#include <ldv/box_representation.h>
#include <ldv/point_representation.h>
#include <ldv/line_representation.h>
#include <ldv/ttf_representation.h>
#include <lda/resource_manager.h>

namespace app {

class main_controller:
	public dfw::controller_interface {
	public:

						main_controller(ldv::resource_manager&, lda::resource_manager&, lm::logger&, const ldtools::ttf_manager&, dfw::audio&);

	virtual void 				preloop(dfw::input& input, float delta, int fps);
	virtual void 				loop(dfw::input& input, const dfw::loop_iteration_data&);
	virtual void 				postloop(dfw::input& input, float delta, int fps);
	virtual void 				draw(ldv::screen& screen, int);
	virtual void 				awake(dfw::input& input);
	virtual void 				slumber(dfw::input& input);
	virtual bool				can_leave_state() const;

	private:

	void					bmp(ldv::screen& screen, int x);
	void					bmp_escalado(ldv::screen& screen, int x);
	void					bmp_alpha(ldv::screen& screen, int x);
	void					bmp_flip(ldv::screen& screen, int x, int t);
	void					bmp_rotar(ldv::screen& screen, int x);
	void					bmp_patron(ldv::screen& screen, int x);
	void					ttf(ldv::screen& screen, int x, const std::string&);
	void					changing_ttf(ldv::screen& screen, int l);
	void					ttf_align(ldv::screen& screen, int l);
	void					caja(ldv::screen& screen, int x, int alpha);
	void					caja_rellena(ldv::screen& screen, int x, int alpha);
	void					linea(ldv::screen& screen, int x, int alpha);
	void					linea_rotar(ldv::screen& screen, int x, int alpha);
	void					poligono(ldv::screen& screen, int x, int alpha);
	void					poligono_rotado(ldv::screen& screen, int x, int alpha);
	void					poligono_relleno(ldv::screen& screen, int x, int alpha);
	void					puntos(ldv::screen& screen, int x, int alpha);
	void					puntos_rotar(ldv::screen& screen, int x, int alpha);
	void					compuesta(ldv::screen& screen, int x);

	ldv::resource_manager&			v_manager;
	lda::resource_manager&			a_manager;
	lm::logger&				log;
	const ldv::ttf_font&			font;
	dfw::audio&				audio_sys;

	int					angle,
						alpha,
						changing_ttf_iterator=0;
	float				changing_ttf_delta;
	const std::string 	changing_ttf_str;

	ldv::camera				camera;
	ldv::box_representation			moving_box;
	ldv::point_representation 		moving_points;
	ldv::line_representation	 	moving_line;
	ldv::ttf_representation			fps_rep;

	class ogl_text {
		public:

		ogl_text(const ldv::ttf_font& font, int x, int y)
			:text(font, ldv::rgba8(255,255,255,255)) {
			text.go_to({60, 60});
			config(x, y);
		}

						void draw(ldv::screen& p) {
			text.draw(p);
		}

		void				config(int x, int y) {
			std::string nstring=compat::to_string(x)+","+compat::to_string(y);
			if(nstring!=text.get_text()) text.set_text(nstring);
		}

		private:


		ldv::ttf_representation 	text;
	}ogl_text;
};

}
