#include "audio.h"

using namespace dfw;

audio::audio(lda::audio_controller& a)
	:ac(a), queue(ac)
{

}

void audio::pause_active()
{
	unsigned int canales=ac.get_requested_channels();
	unsigned int i=0;

	while(i < canales)
	{
		auto c=ac.get_channel(i);
		if(c.is_playing())
		{
			c.pause();
			paused_channels.push_back(i);				
		}
		++i;
	}
}

/*Todos los sonidos vuelven a reproducirse...*/

void audio::resume()
{
	ac.resume_sound();
	paused_channels.clear();
}

/*Los sonidos en los canales activos que se pausaron vuelven a reproducirse. El
buffer de canales guardados se vacía para permitir usar el sistema de nuevo.*/

void audio::resume_active()
{
	for(unsigned int i : paused_channels)
	{
		ac.get_channel(i).resume();
	}
	
	paused_channels.clear();
}

/*Reanuda los sonidos pausados que no fueran guardados mediante pausar_activos.*/

void audio::resume_inactive()
{
	unsigned int canales=ac.get_requested_channels();
	unsigned int i=0;

	if(!paused_channels.size())
	{
		ac.resume_sound();
	}
	else while(i < canales)
	{
		if(std::find(paused_channels.begin(),
				paused_channels.end(),
				i)==paused_channels.end())
		{
			ac.get_channel(i).resume();
		}
		++i;
	}
}
