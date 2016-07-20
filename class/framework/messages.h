#ifndef DFRAMEWORK_MESSAGES_H
#define DFRAMEWORK_MESSAGES_H

#include <vector>
#include <memory>

namespace dfw
{

/**
* Un sistema de mensajería (no necesariamente eventos, pero es un buen nombre)
* para que los diferentes controladores puedan comunicarse con una central,
* que sería el bootstrap de la aplicación.
* De momento no hay nada pensado para que diferentes controladores se 
* comuniquen entre si fuera del código que ya tenemos en el bootstrap. Siempre
* sería posible añadir una cola de mensajes que pueda consumirse entre los
* propios controladores. 
*/

/**
* Base para un evento. Un evento es una estructura que tendrá sus propios 
* valores indicando lo que sea que haya ocurrido. Incluye un método virtual
* para indicar su tipo de evento de acuerdo con un entero. Es tarea del 
* intérprete "desmontar" cada evento para convertirlo en el tipo deseado.
*/

struct message_i
{
	virtual	int		get_type() const=0;
};


/**
* Esto es la clase base para un reader de eventos. La idea es que el 
* intérprete sea completamente opcional para la aplicación y que tenga estado.
* Por ejemplo, podría tener una referencia a la configuración para poder
* actualizar los valores de configuración en respuesta a eventos que vayan
* llegando.
*/

class message_reader_interface
{
	public:

	virtual void		consume(const message_i& ev)=0;
};

typedef std::unique_ptr<message_i> uptr_message;

/**
* La cola de eventos es una clase separada que contiene el vector. Será propiedad
* del director de estados pero se inyectará en los controladores al registrarlos.
*/

class message_queue
{
	public:

	size_t			size() const {return data.size();}

	//Encola un evento para ser procesado al final del loop actual.
	void			insert(uptr_message& ev) {data.push_back(std::move(ev));}

	//Envia un evento que será interpretado inmediatamente.
	void			consume(uptr_message& ev) {reader->consume(*ev);}

	//Realiza el proceso de la cola de eventos... La idea es que esto lo llame sólo el director de estados.
	void			process(message_reader_interface& i)
	{
		for(auto& ev : data) i.consume(*ev);
		data.clear();
	}

	//Establece un intérprete de eventos para el "enviar eventos". Lo hace el director al registrar el intérprete.
	void			set_reader(message_reader_interface& e){	reader=&e;}

	private:

	std::vector<std::unique_ptr<message_i>>		data;
	message_reader_interface *			reader;
	
};

}
#endif
