#include "principal.h"

#include "../app/framework_impl/input.h"
#include <templates/parches_compat.h>

#include "../app/recursos.h"

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Herramientas_proyecto::Gestor_fuentes_TTF& f)
	:log(log),
	fuente(f.obtener_fuente("akashi", 20)),
	interruptor(true), angulo(90), alpha(255),
	camara({32,0,600,300},{0,300}),
	caja_movil{DLibV::Representacion_primitiva_poligono::tipo::relleno, {0,0,6,6}, DLibV::rgba8(255, 0, 0, 255)},
	puntos_movil({{0, 32}, {32,32}, {32,64}, {0, 64}}, DLibV::rgba8(0, 255, 0, 255)),
	linea_movil{0,0, 32, 32, DLibV::rgba8(0, 0, 255, 255)},
	texto_ogl(f.obtener_fuente("akashi", 20), 0, 0)
{

}

void Controlador_principal::preloop(DFramework::Input& input, float delta)
{
	
}

void Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::escape))
	{
		abandonar_aplicacion();
		return;
	}

	if(input.es_input_down(Input::espacio)) 
	{
		camara.mut_zoom(1.0);
		camara.enfocar_a({0, 0});
		angulo=0;
	}
	else if(input.es_input_down(Input::num1)) camara.mut_zoom(1.0);
	else if(input.es_input_down(Input::num2)) camara.mut_zoom(2.0);
	else if(input.es_input_down(Input::num3)) camara.mut_zoom(3.0);

	if(input.es_input_pulsado(Input::arriba)) camara.movimiento_relativo(0, -1);
	else if(input.es_input_pulsado(Input::abajo)) camara.movimiento_relativo(0, 1);

	if(input.es_input_pulsado(Input::izquierda)) camara.movimiento_relativo(-1, 0);
	else if(input.es_input_pulsado(Input::derecha)) camara.movimiento_relativo(1, 0);

	if(input.es_input_pulsado(Input::key_a)) 
	{
		--angulo;
		--alpha; if(alpha < 0) alpha=0;
	}
	else if(input.es_input_pulsado(Input::key_s)) 
	{
		++angulo;
		++alpha; if(alpha > 255) alpha=255;
	}

	if(input.es_input_pulsado(Input::zoom_mas)) camara.mut_zoom(camara.acc_zoom()+0.01);
	else if(input.es_input_pulsado(Input::zoom_menos)) camara.mut_zoom(camara.acc_zoom()-0.01);

	texto_ogl.configurar(camara.acc_x(), camara.acc_y());
}

void Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(DLibV::rgba8(64, 64, 64, 255));

	int x=0;

	//Posición real de la cámara
	DLibV::Representacion_primitiva_caja ccam{DLibV::Representacion_primitiva_poligono::tipo::lineas,{(int)camara.acc_pos_x(), (int)camara.acc_pos_y(), (unsigned int)camara.acc_pos_w(), (unsigned int)camara.acc_pos_h()}, DLibV::rgba8(255, 255, 255, 12)};
	ccam.volcar(pantalla);

	//Cuadro móvil a dónde enfoca...
	DLibV::Representacion_primitiva_caja ccam2{DLibV::Representacion_primitiva_poligono::tipo::lineas,{(int)camara.acc_x(), (int)camara.acc_y(), (unsigned int)camara.acc_foco_w(), (unsigned int)camara.acc_foco_h()}, DLibV::rgba8(255, 255, 255, 64)};
	ccam2.volcar(pantalla);

	//Comprobaciones de ir_a.
	caja_movil.ir_a(camara.acc_x(), camara.acc_y());
	caja_movil.volcar(pantalla);

	puntos_movil.ir_a(camara.acc_x()+camara.acc_foco_w(), camara.acc_y());
	puntos_movil.volcar(pantalla);

	linea_movil.ir_a(camara.acc_x(), camara.acc_y()+camara.acc_foco_h());
	linea_movil.volcar(pantalla);

	//These are all the representations currently in use.
	bmp(pantalla, x); x+=40;
	bmp_escalado(pantalla, x); x+=70;
	bmp_flip(pantalla, x, 1); x+=40;
	bmp_flip(pantalla, x, 2); x+=40;
	bmp_flip(pantalla, x, 3); x+=40;
	compuesta(pantalla, x); x+=128;
	bmp_patron(pantalla, x); x+=70;
	bmp_rotar(pantalla, x); x+=40;
	bmp_alpha(pantalla, x); x+=64;
	ttf(pantalla, x, !angulo ? "This is some string" : "Change"); x+=80;
	caja(pantalla, x, 128); x+=40;
	caja(pantalla, x, 255); x+=40;
	caja_rellena(pantalla, x, 255); x+=40;
	caja_rellena(pantalla, x, 128); x+=40;
	linea(pantalla, x, 192); x+=40;
	linea_rotar(pantalla, x, 255); x+=40;
	poligono(pantalla, x, 128); x+=40;
	poligono(pantalla, x, 255); x+=40;
	poligono_rotado(pantalla, x, 128); x+=40;
	poligono_relleno(pantalla, x, 255); x+=40;
	poligono_relleno(pantalla, x, 128); x+=40;
	puntos(pantalla, x, 255); x+=40;
	puntos_rotar(pantalla, x, 128); x+=40;
//	compuesta(pantalla, x); x+=128;

	texto_ogl.volcar(pantalla);
}

void Controlador_principal::bmp(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites), {x,32,32,32}, {0,0,32,32}, DLibV::Representacion_grafica::sampling::completo);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_escalado(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites), {x, 32, 64, 64}, {32,32,32,32}, DLibV::Representacion_grafica::sampling::completo);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_flip(DLibV::Pantalla& pantalla, int x, int t)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites), {x, 32, 32, 32}, {0,32,32,32}, DLibV::Representacion_grafica::sampling::completo);

	switch(t)
	{
		case 0: break;
		case 1: r.transformar_invertir_horizontal(true); break;
		case 2: r.transformar_invertir_vertical(true); break;
		case 3: r.transformar_invertir_horizontal(true); 
			r.transformar_invertir_vertical(true); break;
	}

	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_patron(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites), {x, 32, 32, 32}, {0,0,32,32}, DLibV::Representacion_grafica::sampling::completo);
	r.establecer_pincel(10, 20);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_alpha(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_lens_flare), {x, 32, 50, 50}, {0,0,50,50}, DLibV::Representacion_grafica::sampling::completo);
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.establecer_alpha(64);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_rotar(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites), {x, 32, 128, 32}, {0,0,32,32}, DLibV::Representacion_grafica::sampling::completo);
	if(angulo)
	{
		r.transformar_rotar(angulo);
		r.transformar_centro_rotacion(16, 16);
	}
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::ttf(DLibV::Pantalla& pantalla, int x, const std::string& cad)
{
	DLibV::Representacion_TTF r(fuente, DLibV::rgba8(255, 255, 255, 255), cad);
	r.ir_a(x, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::caja(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_caja r{DLibV::Representacion_primitiva_poligono::tipo::lineas, {x, 32, 32, 32}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::caja_rellena(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_caja r{DLibV::Representacion_primitiva_poligono::tipo::relleno, {x, 32, 32, 32}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::linea(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_linea r(x, 32, x+32, 64, DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::linea_rotar(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_linea r(x, 32, x+32, 64, DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);

	if(angulo)
	{
		r.transformar_rotar(angulo);
		r.transformar_centro_rotacion(16, 16);
	}

	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_poligono r{DLibV::Representacion_primitiva_poligono::tipo::lineas, {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono_rotado(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_poligono r{DLibV::Representacion_primitiva_poligono::tipo::lineas, {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);

	if(angulo)
	{
		r.transformar_rotar(angulo);
		r.transformar_centro_rotacion(16, 16);
	}

	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono_relleno(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_poligono r{DLibV::Representacion_primitiva_poligono::tipo::relleno, {{x, 40},{x+16,32},{x+32,40},{x+16,64}}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::puntos(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_puntos r({{x,32},{x+32,32},{x+32, 64},{x, 64}}, DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::puntos_rotar(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_puntos r({{x,32},{x+32,32},{x+32, 64},{x, 64}}, DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::alpha);
	
	if(angulo)
	{
		r.transformar_rotar(angulo);
		r.transformar_centro_rotacion(16, 16);
	}

	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::compuesta(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_agrupada r({x, 32}, true);

	//TODO: Also, does something wrong with the camera and sometimes dissapears.
	//TODO: Something is wrong with the first one: always.
	DLibV::Representacion_primitiva_caja * r4=new DLibV::Representacion_primitiva_caja{DLibV::Representacion_primitiva_poligono::tipo::relleno, {32, 64, 32, 32}, DLibV::rgba8(0, 0, 255, 255)};
	r.insertar_representacion(r4);

	DLibV::Representacion_TTF * r3=new DLibV::Representacion_TTF(fuente, DLibV::rgba8(255, 255, 255, 255), "Hola");
	r3->ir_a(0, 64);
	r.insertar_representacion(r3);
	
	DLibV::Representacion_bitmap * r2=new DLibV::Representacion_bitmap(DLibV::Gestor_texturas::obtener(g_sprites),
		{64, 32, 64, 64}, {32,0,32,32}, DLibV::Representacion_grafica::sampling::completo);
	r2->establecer_pincel(16, 32);
	r.insertar_representacion(r2);

	DLibV::Representacion_bitmap * r1=new DLibV::Representacion_bitmap(DLibV::Gestor_texturas::obtener(g_sprites),
		{-32, 32, 32, 32}, {32,32,32,32}, DLibV::Representacion_grafica::sampling::completo);
	r1->establecer_alpha(64);
	r1->establecer_modo_blend(DLibV::Representacion::blends::alpha);
	r.insertar_representacion(r1);

	DLibV::Representacion_primitiva_linea * r5=new DLibV::Representacion_primitiva_linea(0, 0, 64, 64, DLibV::rgba8(0, 255, 0, 255));
	r.insertar_representacion(r5);

	DLibV::Representacion_primitiva_poligono * r6=new DLibV::Representacion_primitiva_poligono{DLibV::Representacion_primitiva_poligono::tipo::relleno, {{32, 0},{0,32},{32,32}}, DLibV::rgba8(255, 0, 0, 255)};
	r.insertar_representacion(r6);

	DLibV::Representacion_primitiva_puntos * r7=new DLibV::Representacion_primitiva_puntos({{64,0},{92,0},{92,16},{64,16}}, DLibV::rgba8(255, 64, 64, 255));
	r.insertar_representacion(r7);

	r.ir_a(x, 32);
	if(angulo)
	{
		r.transformar_rotar(angulo);
		r.transformar_centro_rotacion(32, 32);
	}

	r.establecer_alpha(alpha);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}


void  Controlador_principal::despertar()
{

}

void  Controlador_principal::dormir()
{

}

bool Controlador_principal::es_posible_abandonar_estado() const
{
	return true;
}
