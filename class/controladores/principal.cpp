#include "principal.h"

#include "../app/framework_impl/input.h"
#include <templates/parches_compat.h>

#include "../app/recursos.h"

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Herramientas_proyecto::Gestor_fuentes_TTF& f)
	:log(log),
	fuente(f.obtener_fuente("akashi", 20)),
	camara(0,0,100,100,200,200), 
	t(0.0f),
	direccion(1)
{
}

void Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::loop(DFramework::Input& input, float delta)
{
	t+=delta;
	camara.movimiento_relativo(direccion, 0);
	if(camara.acc_x() >= 800)
	{
		direccion=-direccion;
		camara.enfocar_a(799, 0);
	}
	else if(camara.acc_x() < 0)
	{
		direccion=-direccion;
		camara.enfocar_a(0, 0);
	}
	
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}

	if(input.es_input_down(Input::arriba) ||
		input.es_input_down(Input::abajo) ||
		input.es_input_down(Input::izquierda) ||
		input.es_input_down(Input::derecha) ||
		input.es_input_down(Input::escape))
	{
		abandonar_aplicacion();
		return;
	}
}

void Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);
	int x=32;

	DLibV::Representacion_primitiva_caja ccam{{200, 200, 100, 100}, 255, 255, 255};
	ccam.establecer_alpha(64);
	ccam.volcar(pantalla);

	//These are all the representations currently in use.

	bmp(pantalla, x+=40);
	//TODO: Fix clipping.
	bmp_rotar(pantalla, x+=40);
	bmp_alpha(pantalla, x+=40);
	//TODO: Remove SDL_Color dependency
	ttf(pantalla, x+=40);
	//TODO: Normalize color params and alpha.
	caja(pantalla, x+=40);
	caja_rellena(pantalla, x+=40);
	//TODO: Normalize color params and alpha
	linea(pantalla, x+=40);
	//TODO: Normalize color params and alpha
	//TODO: Fix camera problem.
	poligono(pantalla, x+=40);
	poligono_relleno(pantalla, x+=40);
	//TODO: Normalize color params and alpha
	//TODO: Add vector constructor and insert.
	puntos(pantalla, x+=40);
}

void Controlador_principal::bmp(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_recorte(0,0,32,32);
	r.establecer_posicion(x, 32, 32, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_alpha(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_lens_flare));
	r.establecer_recorte(0,0,50,50);
	r.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	r.establecer_alpha(255);
	r.establecer_posicion(x, 32, 50, 50);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::bmp_rotar(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_recorte(0,0,32,32);
	r.transformar_rotar(45);
	r.establecer_posicion(x, 32, 32, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::ttf(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_TTF r(fuente, SDL_Color{255, 255, 255, 255}, "Hola");
	r.ir_a(x, 32);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);

}

void Controlador_principal::caja(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_caja_lineas r{{x, 32, 32, 32}, 255, 64, 64};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::caja_rellena(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_caja r{{x, 32, 32, 32}, 255, 64, 64};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::linea(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_linea r(x, 32, x+32, 64, 255, 64, 64);
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_poligono_lineas r{ {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, 255, 64, 64};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono_relleno(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_poligono r{ {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, 255, 64, 64};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::puntos(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_puntos r(255, 32, 32);
	r.insertar(x, 32);
	r.insertar(x+32, 32);
	r.insertar(x+32, 64);
	r.insertar(x, 64);

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
