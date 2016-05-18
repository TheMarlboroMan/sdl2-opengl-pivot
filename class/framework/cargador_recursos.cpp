#include "cargador_recursos.h"
#include "kernel_driver_interface.h"

using namespace DFramework;

void Cargador_recursos::procesar(const std::vector<std::string>& entradas, void (Cargador_recursos::*procesar_valores)(const std::vector<std::string>&))
{
	const char separador='\t';
	for(auto& linea : entradas)
	{
		(this->*procesar_valores)(DLibH::Herramientas::explotar(linea, separador));
	}
}

void Cargador_recursos::generar_recursos_texturas(const std::vector<std::string>& recursos, DLibV::Pantalla &p)
{
	DLibV::Gestor_texturas::vaciar();
	pantalla=&p;
	try
	{
		procesar(recursos, &Cargador_recursos::procesar_entrada_textura);
	}
	catch(std::exception& e)
	{
		LOG<<"ERROR: No se ha cargado el archivo de texturas: "<<e.what()<<std::endl;
	}
	pantalla=nullptr;
}

void Cargador_recursos::generar_recursos_superficies(const std::vector<std::string>& recursos, DLibV::Pantalla &p)
{
	pantalla=&p;
	DLibV::Gestor_superficies::vaciar();
	try
	{
		procesar(recursos, &Cargador_recursos::procesar_entrada_superficie);
	}
	catch(std::exception& e)
	{
		LOG<<"ERROR: No se ha cargado el archivo de superficies: "<<e.what()<<std::endl;
	}
}

void Cargador_recursos::generar_recursos_audio(const std::vector<std::string>& recursos)
{
	//OJO: Vacia sonido y música.
	DLibA::Gestor_recursos_audio::liberar();
	try
	{
		procesar(recursos, &Cargador_recursos::procesar_entrada_audio);
	}
	catch(std::exception& e)
	{
		LOG<<"ERROR: No se ha localizado el archivo de recursos de audio: "<<e.what()<<std::endl;
	}
	pantalla=nullptr;
}

void Cargador_recursos::generar_recursos_musica(const std::vector<std::string>& recursos)
{
	try
	{
		procesar(recursos, &Cargador_recursos::procesar_entrada_musica);
	}
	catch(std::exception& e)
	{
		LOG<<"ERROR: No se ha localizado el archivo de recursos de música: "<<e.what()<<std::endl;
	}
}

void Cargador_recursos::procesar_entrada_textura(const std::vector<std::string>& valores)
{
	//TODO: Colorkey may not always be a thing...
	if(valores.size()!=6) LOG<<"ERROR: No hay 6 parametros para recursos textura, en su lugar "<<valores.size()<<std::endl;
	else 
	{
		unsigned int indice=std::atoi(valores[0].c_str());
		std::string ruta=valores[1];
//		unsigned int transparencia=std::atoi(valores[2].c_str());


/*
		DLibV::Imagen img(ruta);
		if(transparencia)
		{
			unsigned int r=std::atoi(valores[3].c_str());
			unsigned int g=std::atoi(valores[4].c_str());
			unsigned int b=std::atoi(valores[5].c_str());

			if(!SDL_SetColorKey(superficie, SDL_TRUE, SDL_MapRGB(superficie->format, r, g, b)))
			{
				LOG<<"ERROR: Imposible establecer colorkey "<<SDL_GetError()<<std::endl;
			}

		}
*/
		DLibV::Textura * t=new DLibV::Textura(ruta);
		if(DLibV::Gestor_texturas::insertar(indice, t)==-1)
		{
			LOG<<"ERROR: No se ha podido insertar textura "<<indice<<" en "<<ruta<<std::endl;
		}	
	}			
}

void Cargador_recursos::procesar_entrada_superficie(const std::vector<std::string>& valores)
{
	if(valores.size()!=6) LOG<<"ERROR: No hay 6 parametros para recursos superficie, en su lugar "<<valores.size()<<std::endl;
	else 
	{
		unsigned int indice=std::atoi(valores[0].c_str());
		std::string ruta=valores[1];
		unsigned int transparencia=std::atoi(valores[2].c_str());

		SDL_Surface * superficie=DLibV::Utilidades_graficas_SDL::cargar_imagen(ruta.c_str(), pantalla->acc_ventana());

		if(!superficie)
		{
			LOG<<"ERROR: Cargador recursos base no se ha podido cargar superficie para textura en "<<ruta<<std::endl;
		}
		else
		{
			if(transparencia)
			{
				unsigned int r=std::atoi(valores[3].c_str());
				unsigned int g=std::atoi(valores[4].c_str());
				unsigned int b=std::atoi(valores[5].c_str());

				SDL_SetColorKey(superficie, SDL_TRUE, SDL_MapRGB(superficie->format, r, g, b));
			}
			

			DLibV::Imagen * t=new DLibV::Imagen(superficie);

			if(DLibV::Gestor_superficies::insertar(indice, t)==-1)
			{
				LOG<<"ERROR: No se ha podido insertar superficie "<<indice<<" en "<<ruta<<std::endl;
			}	
		}
	}			
}


void Cargador_recursos::procesar_entrada_audio(const std::vector<std::string>& valores)
{
	if(valores.size()!=2) LOG<<"ERROR: No hay 2 parametros para recursos audio, en su lugar "<<valores.size()<<std::endl;
	else 
	{
		unsigned int indice=std::atoi(valores[0].c_str());
		std::string ruta=valores[1];

		if(DLibA::Gestor_recursos_audio::insertar_sonido(indice, ruta.c_str())==-1)
		{
			LOG<<"ERROR: No se ha podido insertar recurso audio "<<indice<<" en "<<ruta<<std::endl;
		}	
	}			
}

void Cargador_recursos::procesar_entrada_musica(const std::vector<std::string>& valores)
{
	if(valores.size()!=2) LOG<<"ERROR: No hay 2 parametros para recursos musica, en su lugar "<<valores.size()<<std::endl;
	else 
	{
		unsigned int indice=std::atoi(valores[0].c_str());
		std::string ruta=valores[1];

		if(DLibA::Gestor_recursos_audio::insertar_musica(indice, ruta.c_str())==-1)
		{
			LOG<<"ERROR: No se ha podido insertar recurso musica "<<indice<<" en "<<ruta<<std::endl;
		}	
	}			
}
