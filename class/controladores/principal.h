#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

//Dependencias...
#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

//Específicos de la aplicación...
#include <class/gestor_fuentes_ttf.h>
#include <templates/parches_compat.h>
#include <def_video.h>


namespace App
{

class Controlador_principal:
	public DFramework::Controlador_interface
{
	public:

						Controlador_principal(DLibH::Log_base&, const Herramientas_proyecto::Gestor_fuentes_TTF&);

	virtual void 				preloop(DFramework::Input& input, float delta);
	virtual void 				loop(DFramework::Input& input, float delta);
	virtual void 				postloop(DFramework::Input& input, float delta);
	virtual void 				dibujar(DLibV::Pantalla& pantalla);
	virtual void 				despertar();
	virtual void 				dormir();
	virtual bool				es_posible_abandonar_estado() const;

	private:
	
	void					bmp(DLibV::Pantalla& pantalla, int x);
	void					bmp_escalado(DLibV::Pantalla& pantalla, int x);
	void					bmp_alpha(DLibV::Pantalla& pantalla, int x);
	void					bmp_flip(DLibV::Pantalla& pantalla, int x, int t);
	void					bmp_rotar(DLibV::Pantalla& pantalla, int x);
	void					bmp_patron(DLibV::Pantalla& pantalla, int x);
	void					ttf(DLibV::Pantalla& pantalla, int x, const std::string&);
	void					caja(DLibV::Pantalla& pantalla, int x, int alpha);
	void					caja_rellena(DLibV::Pantalla& pantalla, int x, int alpha);
	void					linea(DLibV::Pantalla& pantalla, int x, int alpha);
	void					linea_rotar(DLibV::Pantalla& pantalla, int x, int alpha);
	void					poligono(DLibV::Pantalla& pantalla, int x, int alpha);
	void					poligono_rotado(DLibV::Pantalla& pantalla, int x, int alpha);
	void					poligono_relleno(DLibV::Pantalla& pantalla, int x, int alpha);
	void					puntos(DLibV::Pantalla& pantalla, int x, int alpha);
	void					puntos_rotar(DLibV::Pantalla& pantalla, int x, int alpha);
	void					compuesta(DLibV::Pantalla& pantalla, int x);

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&		fuente;

	bool					interruptor;
	int					angulo,
						alpha;

	DLibV::Camara				camara;
	DLibV::Representacion_primitiva_caja	caja_movil;
	DLibV::Representacion_primitiva_puntos 	puntos_movil;
	DLibV::Representacion_primitiva_linea 	linea_movil;

	class Texto_ogl
	{
		public:

		Texto_ogl(const DLibV::Fuente_TTF& fuente, int x, int y)
			:texto(fuente, DLibV::rgba8(255,255,255,255))
		{
			texto.ir_a(60, 60);
			configurar(x, y);
		}

						void volcar(DLibV::Pantalla& p)
		{
			texto.volcar(p);
		}

		void				configurar(int x, int y)
		{
			std::string nueva=compat::to_string(x)+","+compat::to_string(y);
			if(nueva!=texto.acc_cadena()) texto.asignar(nueva);
		}

		private:
	

		DLibV::Representacion_TTF 	texto;
	}texto_ogl;
};

}

#endif
