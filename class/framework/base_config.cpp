#include "base_config.h"
#include <map>
#include <source/string_utils.h>

/*
Al crear el objeto aún no podemos cargar la información de los valores por
defecto (métodos virtuales) así que no podremos intentar acceder a ningún
valor hasta haber cargado la configuración.
*/

using namespace dfw;

base_config::base_config(const std::string& ppath)
try
	:token(tools::dnot_parse(ppath)),
	path(ppath)
{

}
catch(std::runtime_error& e)
{
	throw std::runtime_error(std::string("error starting configuration ")+ppath+" : "+e.what());
}

void base_config::save()
{
	using namespace tools;
	dnot_token_serialize_options os;
	os.tab_depth=true;
	std::ofstream f(path);
	f<<token.serialize(os);
}

/**
* Localiza un token nombrado siguiendo una cadena sencilla: config:video:tam_pantalla
* localizaría el token en el objeto "config" de la raiz, seguido del objeto 
* "video" contenido en "config" y "tam_pantalla" contenido en "video".
*/

const tools::dnot_token& base_config::token_from_path(const std::string& c) const
{
	using namespace tools;
	const dnot_token * p=&token;
	auto v=explode(c, ':');
	for(const auto& clave : v) 
	{
		try
		{
			p=&p->get_map().at(clave);
		}
		catch(std::exception& e)
		{
			throw std::runtime_error("unable to locate key "+clave+" in file "+c);
		}
	}
	return *p;
}

tools::dnot_token& base_config::token_from_path(const std::string& c)
{
	using namespace tools;
	dnot_token * p=&token;
	auto v=explode(c, ':');
	for(const auto& clave : v) p=&p->get_map()[clave];
	return *p;
}
