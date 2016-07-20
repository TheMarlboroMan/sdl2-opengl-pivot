#ifndef DFW_RESOURCE_LOADER_H
#define DFW_RESOURCE_LOADER_H

/*Un cargador de recursos gráficos y de audio. Para cargar los recursos usa
ficheros en disco, localizados en las rutas de turno. Tiene un rollo autoload,
bien para proyectos pequeños, mal para proyectos grandes, puesto que lo que
cargas se queda para siempre cargado.

Estos ficheros tienen una estructura concreta, N valores separados por 
tabuladores.

Aunque se pueden generar texturas a partir de superficies, lo vamos a separar
todo por aquí. Para proyectos más grandes haríamos gestores custom :).

Para graficos (texturas y superficies).

#indice	ruta	transparencia	rtrans	gtrans	btrans
1	data/imagenes/fuente_defecto.png	1	0	0	0

Donde "transparencia" indica si hay transparencia y r, g y btrans indican el 
color de pixel transparente.

Para audio:
#indice	#ruta
1	data/sonidos/defecto.ogg

Al usar este cargador se cargan todos los recursos directamente en memoria. Esto
puede estar bien para aplicaciones pequeñas, pero si hay muchos gráficos mejor
que usemos métodos alternativos para cargarlos y descargarlos según sea 
necesario.

Otra cosa más, si vamos a usarlo no deberíamos almacenar nunca referencias
ni punteros a los recursos, puesto que pueden vaciarse. En su lugar, los 
obtenemos siempre mediante los gestores de turno.
*/

#include <stdexcept>
#include <libDan2.h>
#include <class/text_reader.h>

namespace dfw
{

class resource_loader
{
	public:
	
	resource_loader(ldv::resource_manager&, lda::resource_manager&);

	void generate_textures(const std::vector<std::string>&);
	void generate_surfaces(const std::vector<std::string>&, ldv::screen &pantalla);
	void generate_sounds(const std::vector<std::string>&);
	void generate_music(const std::vector<std::string>&);

	private:

	void process_music(const std::vector<std::string>&);
	void process_sound(const std::vector<std::string>&);
	void process_texture(const std::vector<std::string>&);
	void process_surface(const std::vector<std::string>&);
	void process(const std::vector<std::string>& entradas, void (resource_loader::*)(const std::vector<std::string>&));

	ldv::resource_manager&	video_m;
	lda::resource_manager&	audio_m;

	
	ldv::screen * screen;	//Un hack para poder usar métodos más genéricos en "procesar".
};

}
#endif
