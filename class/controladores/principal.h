#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

//Dependencias...
#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

//Específicos de la aplicación...
#include <class/gestor_fuentes_ttf.h>
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
	void					bmp_alpha(DLibV::Pantalla& pantalla, int x);
	void					bmp_rotar(DLibV::Pantalla& pantalla, int x);
	void					ttf(DLibV::Pantalla& pantalla, int x);
	void					caja(DLibV::Pantalla& pantalla, int x);
	void					caja_rellena(DLibV::Pantalla& pantalla, int x);
	void					linea(DLibV::Pantalla& pantalla, int x);
	void					poligono(DLibV::Pantalla& pantalla, int x);
	void					poligono_relleno(DLibV::Pantalla& pantalla, int x);
	void					puntos(DLibV::Pantalla& pantalla, int x);

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&		fuente;

	DLibV::Camara				camara;
	float					t;
	int					direccion;
};

}

#endif
