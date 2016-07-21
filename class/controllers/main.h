#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

//Dependencias...
#include "states.h"
#include "../framework/controller_interface.h"

//Específicos de la aplicación...
#include <class/ttf_manager.h>
#include <templates/compatibility_patches.h>
#include <def_video.h>


namespace app
{

class main_controller:
	public dfw::controller_interface
{
	public:

						main_controller(ldv::resource_manager&, ldt::log&, const tools::ttf_manager&);

	virtual void 				preloop(dfw::input& input, float delta);
	virtual void 				loop(dfw::input& input, float delta);
	virtual void 				postloop(dfw::input& input, float delta);
	virtual void 				draw(ldv::screen& screen);
	virtual void 				awake();
	virtual void 				slumber();
	virtual bool				can_leave_state() const;

	private:
	
	void					bmp(ldv::screen& screen, int x);
	void					bmp_escalado(ldv::screen& screen, int x);
	void					bmp_alpha(ldv::screen& screen, int x);
	void					bmp_flip(ldv::screen& screen, int x, int t);
	void					bmp_rotar(ldv::screen& screen, int x);
	void					bmp_patron(ldv::screen& screen, int x);
	void					ttf(ldv::screen& screen, int x, const std::string&);
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
	ldt::log&				log;
	const ldv::ttf_font&			font;

	int					angle,
						alpha;

	ldv::camera				camera;
	ldv::box_representation			moving_box;
	ldv::point_representation 		moving_points;
	ldv::line_representation	 	moving_line;

	class ogl_text
	{
		public:

		ogl_text(const ldv::ttf_font& font, int x, int y)
			:text(font, ldv::rgba8(255,255,255,255))
		{
			text.go_to(60, 60);
			config(x, y);
		}

						void draw(ldv::screen& p)
		{
			text.draw(p);
		}

		void				config(int x, int y)
		{
			std::string nstring=compat::to_string(x)+","+compat::to_string(y);
			if(nstring!=text.get_text()) text.set_text(nstring);
		}

		private:
	

		ldv::ttf_representation 	text;
	}ogl_text;
};

}

#endif
