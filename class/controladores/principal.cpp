#include "principal.h"

#include "../app/framework_impl/input.h"
#include <templates/parches_compat.h>

#include "../app/recursos.h"

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Herramientas_proyecto::Gestor_fuentes_TTF& f)
	:log(log),
	fuente(f.obtener_fuente("akashi", 20)),
	interruptor(true), angulo(0),
	camara(0,0,300,150,300,200),
	caja_movil{ {0,0,6,6}, DLibV::rgba8(255, 0, 0, 255)},
	puntos_movil(DLibV::rgba8(0, 255, 0, 255)),
	linea_movil{0,0, 32, 32, DLibV::rgba8(0, 0, 255, 255)}
{
	puntos_movil.insertar(0, 32);
	puntos_movil.insertar(0+32, 32);
	puntos_movil.insertar(0+32, 64);
	puntos_movil.insertar(0, 64);
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
	}

	if(input.es_input_pulsado(Input::arriba)) camara.movimiento_relativo(0, -1);
	else if(input.es_input_pulsado(Input::abajo)) camara.movimiento_relativo(0, 1);

	if(input.es_input_pulsado(Input::izquierda)) 
	{
		camara.movimiento_relativo(-1, 0);
		--angulo;
	}
	else if(input.es_input_pulsado(Input::derecha)) 
	{
		camara.movimiento_relativo(1, 0);
		++angulo;
	}

	else if(input.es_input_pulsado(Input::zoom_mas))
	{
		camara.mut_zoom(camara.acc_zoom()+0.01);
	}
	else if(input.es_input_pulsado(Input::zoom_menos))
	{
		camara.mut_zoom(camara.acc_zoom()-0.01);
	}
}

void Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(DLibV::rgba8(64, 64, 64, 255));

	int x=0;

	//Posición real de la cámara
	DLibV::Representacion_primitiva_caja_lineas ccam{{(int)camara.acc_pos_x(), (int)camara.acc_pos_y(), (unsigned int)camara.acc_pos_w(), (unsigned int)camara.acc_pos_h()}, DLibV::rgba8(255, 255, 255, 12)};
	ccam.volcar(pantalla);

	//Cuadro móvil a dónde enfoca...
	DLibV::Representacion_primitiva_caja_lineas ccam2{{(int)camara.acc_x(), (int)camara.acc_y(), (unsigned int)camara.acc_foco_w(), (unsigned int)camara.acc_foco_h()}, DLibV::rgba8(255, 255, 255, 64)};
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
	bmp_patron(pantalla, x); x+=70;
	bmp_rotar(pantalla, x); x+=40;
	bmp_alpha(pantalla, x); x+=64;
	ttf(pantalla, x); x+=80;
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
	//TODO: Add vector constructor and insert.
	puntos(pantalla, x, 255); x+=40;
	puntos_rotar(pantalla, x, 128); x+=40;
	//TODO: Add vector insert.
//	compuesta(pantalla, x);
}

void Controlador_principal::bmp(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_recorte(0,0,32,32);
	r.establecer_posicion(x, 32, 32, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_escalado(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_recorte(32,32,32,32);
	r.establecer_posicion(x, 32, 64, 64);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_flip(DLibV::Pantalla& pantalla, int x, int t)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_recorte(0,32,32,32);
	r.establecer_posicion(x, 32, 32, 32);

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
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_recorte(0,0,32,32);
	r.establecer_posicion(x, 32, 32, 32);
	r.establecer_pincel(10, 20);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_alpha(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_lens_flare));
	r.establecer_recorte(0,0,50,50);
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.establecer_alpha(64);
	r.establecer_posicion(x, 32, 50, 50);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_rotar(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_recorte(0,0,32,32);

	if(angulo)
	{
		r.transformar_rotar(angulo);
		r.transformar_centro_rotacion(16, 16);
	}
	r.establecer_posicion(x, 32, 128, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::ttf(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_TTF r(fuente, DLibV::rgba8(255, 255, 255, 255), "Hola "+std::to_string(camara.acc_zoom()));
	r.ir_a(x, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::caja(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_caja_lineas r{{x, 32, 32, 32}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::caja_rellena(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_caja r{{x, 32, 32, 32}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::linea(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_linea r(x, 32, x+32, 64, DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::linea_rotar(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_linea r(x, 32, x+32, 64, DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);

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
	DLibV::Representacion_primitiva_poligono_lineas r{ {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono_rotado(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_poligono_lineas r{ {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);

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
	DLibV::Representacion_primitiva_poligono r{ {{x, 40},{x+16,32},{x+32,40},{x+16,64}}, DLibV::rgba8(255, 0, 0, alpha)};
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::puntos(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_puntos r(DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.insertar(x, 32);
	r.insertar(x+32, 32);
	r.insertar(x+32, 64);
	r.insertar(x, 64);

	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::puntos_rotar(DLibV::Pantalla& pantalla, int x, int alpha)
{
	DLibV::Representacion_primitiva_puntos r(DLibV::rgba8(255, 0, 0, alpha));
	r.establecer_modo_blend(DLibV::Representacion::blends::BLEND_ALPHA);
	r.insertar(x, 32);
	r.insertar(x+32, 32);
	r.insertar(x+32, 64);
	r.insertar(x, 64);
	
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
/*
	DLibV::Representacion_agrupada r(true);

	DLibV::Representacion_bitmap * r1=new DLibV::Representacion_bitmap(DLibV::Gestor_texturas::obtener(g_sprites));
	r1->establecer_alpha(128);
	r1->transformar_rotar(45);
	r1->establecer_recorte(0,0,32,32);
	r1->establecer_posicion(0, 32, 32, 32);

	DLibV::Representacion_bitmap * r2=new DLibV::Representacion_bitmap(DLibV::Gestor_texturas::obtener(g_sprites));
	r2->establecer_pincel(16, 32);
	r2->establecer_recorte(32,0,32,32);
	r2->establecer_posicion(32, 32, 64, 64);

	DLibV::Representacion_TTF * r3=new DLibV::Representacion_TTF(fuente, DLibV::rgba8(255, 255, 255, 255), "Hola");
	r3->ir_a(0, 64);

	DLibV::Representacion_primitiva_caja * r4=new DLibV::Representacion_primitiva_caja{{32, 64, 32, 32}, DLibV::rgba8(255, 255, 255, 64)};
	DLibV::Representacion_primitiva_linea * r5=new DLibV::Representacion_primitiva_linea(0, 96, 32, 128, DLibV::rgba8(255, 64, 64, 255));
	DLibV::Representacion_primitiva_poligono * r6=new DLibV::Representacion_primitiva_poligono{ {{0, 128},{16,140},{32,128},{16,160}}, DLibV::rgba8(255, 64, 64, 255)};

	DLibV::Representacion_primitiva_puntos * r7=new DLibV::Representacion_primitiva_puntos(DLibV::rgba8(255, 64, 64, 255));
	r7->insertar(0, 160);
	r7->insertar(32, 160);
	r7->insertar(32, 170);
	r7->insertar(0, 170);

	r.insertar_representacion(r1);
	r.insertar_representacion(r2);
	r.insertar_representacion(r3);
	r.insertar_representacion(r4);
	r.insertar_representacion(r5);
	r.insertar_representacion(r6);
	r.insertar_representacion(r7);

	r.ir_a(x, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
*/
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
