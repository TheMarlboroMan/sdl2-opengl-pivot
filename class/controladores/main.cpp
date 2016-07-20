#include "main.h"

#include "../app/framework_impl/input.h"
#include <templates/compatibility_patches.h>

using namespace app;

main_controller::main_controller(ldv::resource_manager& v_manager, ldt::log& log, const tools::ttf_manager& f)
	:v_manager(v_manager), log(log),
	font(f.get("akashi", 20)),
	angle(90), alpha(255),
	camera({32,0,600,300},{0,300}),
	moving_box{ldv::polygon_representation::type::fill, {0,0,6,6}, ldv::rgba8(255, 0, 0, 255)},
	moving_points({{0, 32}, {32,32}, {32,64}, {0, 64}}, ldv::rgba8(0, 255, 0, 255)),
	moving_line{0,0, 32, 32, ldv::rgba8(0, 0, 255, 255)},
	ogl_text(f.get("akashi", 20), 0, 0)
{

}

void main_controller::preloop(dfw::input& input, float delta)
{
	
}

void main_controller::loop(dfw::input& input, float delta)
{
	if(input().is_exit_signal() || input.is_input_down(input_app::escape))
	{
		set_leave(true);
		return;
	}

	if(input.is_input_down(input_app::space)) 
	{
		camera.set_zoom(1.0);
		camera.go_to({0, 0});
		angle=0;
	}
	else if(input.is_input_down(input_app::num1)) camera.set_zoom(1.0);
	else if(input.is_input_down(input_app::num2)) camera.set_zoom(2.0);
	else if(input.is_input_down(input_app::num3)) camera.set_zoom(3.0);

	if(input.is_input_pressed(input_app::up)) camera.move_by(0, -1);
	else if(input.is_input_pressed(input_app::down)) camera.move_by(0, 1);

	if(input.is_input_pressed(input_app::left)) camera.move_by(-1, 0);
	else if(input.is_input_pressed(input_app::right)) camera.move_by(1, 0);

	if(input.is_input_pressed(input_app::key_a)) 
	{
		--angle;
		--alpha; if(alpha < 0) alpha=0;
	}
	else if(input.is_input_pressed(input_app::key_s)) 
	{
		++angle;
		++alpha; if(alpha > 255) alpha=255;
	}

	if(input.is_input_pressed(input_app::zoom_more)) camera.set_zoom(camera.get_zoom()+0.01);
	else if(input.is_input_pressed(input_app::zoom_less)) camera.set_zoom(camera.get_zoom()-0.01);

	ogl_text.config(camera.get_x(), camera.get_y());
}

void main_controller::postloop(dfw::input& input, float delta)
{

}

void main_controller::draw(ldv::screen& screen)
{
	screen.clear(ldv::rgba8(64, 64, 64, 255));

	int x=100;

	//Posición real de la cámara
	ldv::box_representation ccam{ldv::polygon_representation::type::line,{(int)camera.get_pos_box().origin.x, (int)camera.get_pos_box().origin.y, (unsigned int)camera.get_pos_box().w, (unsigned int)camera.get_pos_box().h}, ldv::rgba8(255, 255, 255, 12)};
	ccam.draw(screen);

	//Cuadro móvil a dónde enfoca...
	ldv::box_representation ccam2{ldv::polygon_representation::type::line,{(int)camera.get_x(), (int)camera.get_y(), (unsigned int)camera.get_focus_box().w, (unsigned int)camera.get_focus_box().h}, ldv::rgba8(255, 255, 255, 64)};
	ccam2.draw(screen);

	//Comprobaciones de go_to.
	moving_box.go_to(camera.get_x(), camera.get_y());
	moving_box.draw(screen);

	moving_points.go_to(camera.get_x()+camera.get_focus_box().w, camera.get_focus_box().origin.y);
	moving_points.draw(screen);

	moving_line.go_to(camera.get_x(), camera.get_y()+camera.get_focus_box().h);
	moving_line.draw(screen);


	//These are all the representations currently in use.
	bmp(screen, x); x+=40;
	bmp_escalado(screen, x); x+=70;
	bmp_flip(screen, x, 1); x+=40;
	bmp_flip(screen, x, 2); x+=40;
	bmp_flip(screen, x, 3); x+=40;
	compuesta(screen, x); x+=128;
	bmp_patron(screen, x); x+=70;
	bmp_rotar(screen, x); x+=40;
	bmp_alpha(screen, x); x+=64;
	ttf(screen, x, !angle ? "This is some string" : "Change"); x+=80;
	caja(screen, x, 128); x+=40;
	caja(screen, x, 255); x+=40;
	caja_rellena(screen, x, 255); x+=40;
	caja_rellena(screen, x, 128); x+=40;
	linea(screen, x, 192); x+=40;
	linea_rotar(screen, x, 255); x+=40;
	poligono(screen, x, 128); x+=40;
	poligono(screen, x, 255); x+=40;
	poligono_rotado(screen, x, 128); x+=40;
	poligono_relleno(screen, x, 255); x+=40;
	poligono_relleno(screen, x, 128); x+=40;
	puntos(screen, x, 255); x+=40;
	puntos_rotar(screen, x, 128); x+=40;

	ogl_text.draw(screen);
}

void main_controller::bmp(ldv::screen& screen, int x)
{
	ldv::bitmap_representation r(v_manager.get_texture(1), {x,32,32,32}, {0,0,32,32}, ldv::raster_representation::sampling::complete);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::bmp_escalado(ldv::screen& screen, int x)
{
	ldv::bitmap_representation r(v_manager.get_texture(1), {x, 32, 64, 64}, {32,32,32,32}, ldv::raster_representation::sampling::complete);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::bmp_flip(ldv::screen& screen, int x, int t)
{
	ldv::bitmap_representation r(v_manager.get_texture(1), {x, 32, 32, 32}, {0,32,32,32}, ldv::raster_representation::sampling::complete);

	switch(t)
	{
		case 0: break;
		case 1: r.set_invert_horizontal(true); break;
		case 2: r.set_invert_vertical(true); break;
		case 3: r.set_invert_horizontal(true); 
			r.set_invert_vertical(true); break;
	}

	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::bmp_patron(ldv::screen& screen, int x)
{
	ldv::bitmap_representation r(v_manager.get_texture(1), {x, 32, 32, 32}, {0,0,32,32}, ldv::raster_representation::sampling::complete);
	r.set_brush(10, 20);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::bmp_alpha(ldv::screen& screen, int x)
{
	ldv::bitmap_representation r(v_manager.get_texture(2), {x, 32, 50, 50}, {0,0,50,50}, ldv::raster_representation::sampling::complete);
	r.set_blend(ldv::representation::blends::alpha);
	r.set_alpha(64);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::bmp_rotar(ldv::screen& screen, int x)
{
	ldv::bitmap_representation r(v_manager.get_texture(1), {x, 32, 128, 32}, {0,0,32,32}, ldv::raster_representation::sampling::complete);
	if(angle)
	{
		r.set_rotation(angle);
		r.set_rotation_center(16, 16);
	}
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::ttf(ldv::screen& screen, int x, const std::string& cad)
{
	ldv::ttf_representation r(font, ldv::rgba8(255, 255, 255, 255), cad);
	r.go_to(x, 32);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::caja(ldv::screen& screen, int x, int alpha)
{
	ldv::box_representation r{ldv::polygon_representation::type::line, {x, 32, 32, 32}, ldv::rgba8(255, 0, 0, alpha)};
	r.set_blend(ldv::representation::blends::alpha);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::caja_rellena(ldv::screen& screen, int x, int alpha)
{
	ldv::box_representation r{ldv::polygon_representation::type::fill, {x, 32, 32, 32}, ldv::rgba8(255, 0, 0, alpha)};
	r.set_blend(ldv::representation::blends::alpha);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::linea(ldv::screen& screen, int x, int alpha)
{
	ldv::line_representation r(x, 32, x+32, 64, ldv::rgba8(255, 0, 0, alpha));
	r.set_blend(ldv::representation::blends::alpha);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::linea_rotar(ldv::screen& screen, int x, int alpha)
{
	ldv::line_representation r(x, 32, x+32, 64, ldv::rgba8(255, 0, 0, alpha));
	r.set_blend(ldv::representation::blends::alpha);

	if(angle)
	{
		r.set_rotation(angle);
		r.set_rotation_center(16, 16);
	}

	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::poligono(ldv::screen& screen, int x, int alpha)
{
	ldv::polygon_representation r{ldv::polygon_representation::type::line, {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, ldv::rgba8(255, 0, 0, alpha)};
	r.set_blend(ldv::representation::blends::alpha);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::poligono_rotado(ldv::screen& screen, int x, int alpha)
{
	ldv::polygon_representation r{ldv::polygon_representation::type::line, {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, ldv::rgba8(255, 0, 0, alpha)};
	r.set_blend(ldv::representation::blends::alpha);

	if(angle)
	{
		r.set_rotation(angle);
		r.set_rotation_center(16, 16);
	}

	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::poligono_relleno(ldv::screen& screen, int x, int alpha)
{
	ldv::polygon_representation r{ldv::polygon_representation::type::fill, {{x, 40},{x+16,32},{x+32,40},{x+16,64}}, ldv::rgba8(255, 0, 0, alpha)};
	r.set_blend(ldv::representation::blends::alpha);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::puntos(ldv::screen& screen, int x, int alpha)
{
	ldv::point_representation r({{x,32},{x+32,32},{x+32, 64},{x, 64}}, ldv::rgba8(255, 0, 0, alpha));
	r.set_blend(ldv::representation::blends::alpha);
	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::puntos_rotar(ldv::screen& screen, int x, int alpha)
{
	ldv::point_representation r({{x,32},{x+32,32},{x+32, 64},{x, 64}}, ldv::rgba8(255, 0, 0, alpha));
	r.set_blend(ldv::representation::blends::alpha);
	
	if(angle)
	{
		r.set_rotation(angle);
		r.set_rotation_center(16, 16);
	}

	r.draw(screen);
	r.draw(screen, camera);
}

void main_controller::compuesta(ldv::screen& screen, int x)
{
	ldv::representation_agrupada r({x, 32}, true);

	ldv::box_representation * r4=new ldv::box_representation{ldv::polygon_representation::type::fill, {32, 32, 32, 32}, ldv::rgba8(0, 0, 255, 255)};
	r.insertar_representacion(r4);

	ldv::ttf_representation * r3=new ldv::ttf_representation(font, ldv::rgba8(255, 255, 255, 255), "Hola");
	r3->go_to(0, 64);
	r.insertar_representacion(r3);

	ldv::bitmap_representation * r2=new ldv::bitmap_representation(v_manager.get_texture(1),
		{64, 32, 64, 64}, {32,0,32,32}, ldv::raster_representation::sampling::complete);
	r2->set_brush(16, 32);
	r.insertar_representacion(r2);

	ldv::bitmap_representation * r1=new ldv::bitmap_representation(v_manager.get_texture(1),
//		{-32, 32, 32, 32}, {32,32,32,32},
		{-32, 32, 32, 32}, {32,32,32,32}, ldv::raster_representation::sampling::complete);
	r1->set_alpha(64);
	r1->set_blend(ldv::representation::blends::alpha);
	r.insertar_representacion(r1);

	ldv::line_representation * r5=new ldv::line_representation(0, 0, 64, 64, ldv::rgba8(0, 255, 0, 255));
	r.insertar_representacion(r5);

	ldv::polygon_representation * r6=new ldv::polygon_representation{ldv::polygon_representation::type::fill, {{32, 0},{0,32},{32,32}}, ldv::rgba8(255, 0, 0, 255)};
	r.insertar_representacion(r6);

	ldv::point_representation * r7=new ldv::point_representation({{64,0},{92,0},{92,16},{64,16}}, ldv::rgba8(255, 64, 64, 255));
	r.insertar_representacion(r7);

	r.go_to(x, 32);
	if(angle)
	{
		r.set_rotation(angle);
		r.set_rotation_center(32, 32);
	}

	r.set_alpha(alpha);
	r.draw(screen);
	r.draw(screen, camera);
}


void  main_controller::awake()
{

}

void  main_controller::slumber()
{

}

bool main_controller::can_leave_state() const
{
	return true;
}
