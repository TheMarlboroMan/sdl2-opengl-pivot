#include "principal.h"

#include "../app/framework_impl/input.h"
#include <templates/parches_compat.h>

#include "../app/recursos.h"

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Herramientas_proyecto::Gestor_fuentes_TTF& f)
	:log(log),
	fuente(f.obtener_fuente("akashi", 20)),
	camara(0,0,100,100,200,200), id_tex(0), id_tex2(0), id_tex3(0)
{
	load_crap(id_tex, "data/graficos/lens_flare.png");
	load_crap(id_tex2, "data/graficos/fondo.bmp");
	load_crap(id_tex3, "data/graficos/sprites.png");
	std::cout<<id_tex<<","<<id_tex2<<","<<id_tex3<<std::endl;
}

void Controlador_principal::load_crap(GLuint& t, const std::string& r)
{
        SDL_Surface * superficie=IMG_Load(r.c_str());
	if(!superficie)
	{
		throw std::runtime_error("FUUUCK 2"+r);
	}

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);

	int mode=GL_RGB;
 	if(superficie->format->BytesPerPixel==4) mode=GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, superficie->w, superficie->h, 0, mode, GL_UNSIGNED_BYTE, superficie->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	SDL_FreeSurface(superficie);
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

	if(input.es_input_pulsado(Input::arriba)) camara.movimiento_relativo(0, -1);
	if(input.es_input_pulsado(Input::abajo)) camara.movimiento_relativo(0, 1);
	if(input.es_input_pulsado(Input::izquierda)) camara.movimiento_relativo(-1, 0);
	if(input.es_input_pulsado(Input::derecha)) camara.movimiento_relativo(1, 0);	
}

void Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(DLibV::rgba8(64, 64, 64, 255));
	int x=0;

	DLibV::Representacion_primitiva_caja ccam{{200, 200, 100, 100}, DLibV::rgba8(255, 255, 255, 64)};
	ccam.volcar(pantalla);

	DLibV::Representacion_primitiva_caja_lineas ccam2{{camara.acc_x(), camara.acc_y(), 100, 100}, DLibV::rgba8(255, 255, 255, 64)};
	ccam2.volcar(pantalla);

	//These are all the representations currently in use.
	bmp(pantalla, x); x+=40;
	bmp_escalado(pantalla, x); x+=70;
	bmp_patron(pantalla, x); x+=70;
	//TODO: Fix clipping.
	bmp_rotar(pantalla, x); x+=64;
	bmp_alpha(pantalla, x); x+=64;
	//TODO: Remove SDL_Color dependency. Normalize color params and alpha.
	ttf(pantalla, x); x+=80;
	//TODO: Normalize color params and alpha.
	caja(pantalla, x); x+=40;
	caja_rellena(pantalla, x); x+=40;
	//TODO: Normalize color params and alpha
	linea(pantalla, x); x+=40;
	//TODO: Normalize color params and alpha
	//TODO: Fix camera problem.
	poligono(pantalla, x); x+=40;
	poligono_relleno(pantalla, x); x+=40;
	//TODO: Normalize color params and alpha
	//TODO: Add vector constructor and insert.
	puntos(pantalla, x); x+=40;
	//TODO: Add vector insert.
	compuesta(pantalla, x);



	std::vector<DLibV::Representacion_primitiva::punto> pt{ {100, 100},
		{250, 100}, 
		{250, 250}, 
		{100, 250}};

	std::vector<DLibV::Representacion_primitiva::punto> pt2{ {350, 100},
		{650, 100}, 
		{450, 250}, 
		{350, 250}};

	std::vector<DLibV::Representacion_primitiva::punto> pt3{ {350, 300},
		{650, 300}, 
		{450, 350}, 
		{350, 350}};

	GLfloat ptex[]={0.f, 0.f,
			1.f, 0.f, 
			1.f, 1.f, 
			0.f, 1.f};

	GLfloat ptex2[]={0.f, 0.f,
			1.f, 0.f, 
			1.f, 1.f, 
			0.f, 1.f};

	GLfloat ptex3[]={0.f, 0.f,
			1.f/8.f, 0.f, 
			1.f/8.f, 1.f/8.f, 
			0.f, 1.f/8.f};

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindTexture(GL_TEXTURE_2D, id_tex);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

		glVertexPointer(2, GL_INT, 0, pt.data());
		glTexCoordPointer(2, GL_FLOAT, 0, ptex);
		glDrawArrays(GL_QUADS, 0, 4);

	glBindTexture(GL_TEXTURE_2D, id_tex2);

		glVertexPointer(2, GL_INT, 0, pt2.data());
		glTexCoordPointer(2, GL_FLOAT, 0, ptex2);
		glDrawArrays(GL_QUADS, 0, 4);

	glBindTexture(GL_TEXTURE_2D, id_tex3);

		glVertexPointer(2, GL_INT, 0, pt3.data());
		glTexCoordPointer(2, GL_FLOAT, 0, ptex3);
		glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

	glDisable(GL_TEXTURE_2D);

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

void Controlador_principal::bmp_patron(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_bitmap_patron r(DLibV::Gestor_texturas::obtener(g_sprites));
	r.establecer_pincel({0, 0, 32, 32});
	r.establecer_recorte(32,0,32,32);
	r.establecer_posicion(x, 32, 64, 64);
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
	DLibV::Representacion_primitiva_caja_lineas r{{x, 32, 32, 32}, DLibV::rgba8(255, 64, 64, 255)};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::caja_rellena(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_caja r{{x, 32, 32, 32}, DLibV::rgba8(255, 64, 64, 255)};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::linea(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_linea r(x, 32, x+32, 64, DLibV::rgba8(255, 64, 64, 255));
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_poligono_lineas r{ {{x, 32},{x+16,40},{x+32,32},{x+16,64}}, DLibV::rgba8(255, 64, 64, 255)};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::poligono_relleno(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_poligono r{ {{x, 40},{x+16,32},{x+32,40},{x+16,64}}, DLibV::rgba8(255, 64, 64, 255)};
	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::puntos(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_primitiva_puntos r(DLibV::rgba8(255, 64, 64, 255));
	r.insertar(x, 32);
	r.insertar(x+32, 32);
	r.insertar(x+32, 64);
	r.insertar(x, 64);

	r.volcar(pantalla);
	r.volcar(pantalla, camara);
}

void Controlador_principal::compuesta(DLibV::Pantalla& pantalla, int x)
{
	DLibV::Representacion_agrupada r(true);

	DLibV::Representacion_bitmap * r1=new DLibV::Representacion_bitmap(DLibV::Gestor_texturas::obtener(g_sprites));
	r1->establecer_alpha(128);
	r1->transformar_rotar(45);
	r1->establecer_recorte(0,0,32,32);
	r1->establecer_posicion(0, 32, 32, 32);

	DLibV::Representacion_bitmap_patron * r2=new DLibV::Representacion_bitmap_patron(DLibV::Gestor_texturas::obtener(g_sprites));
	r2->establecer_pincel({0, 0, 32, 32});
	r2->establecer_recorte(32,0,32,32);
	r2->establecer_posicion(32, 32, 64, 64);

	DLibV::Representacion_TTF * r3=new DLibV::Representacion_TTF(fuente, SDL_Color{255, 255, 255, 255}, "Hola");
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
