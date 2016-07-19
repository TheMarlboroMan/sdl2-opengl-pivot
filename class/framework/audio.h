#ifndef PROYECTO_SDL2_BASE_CONTROLADOR_AUDIO_H
#define PROYECTO_SDL2_BASE_CONTROLADOR_AUDIO_H

/*Clase proxy con el controlador de audio. Añadimos alguna utilidad para el 
tema de pausar y despausar sonidos.*/

#include <libDan2.h>

namespace dfw
{

class audio
{
	public:

				audio(lda::audio_controller&);

	lda::audio_controller& 	operator()() {return ac;}

	void 			pause() {ac.pause_sound();}
	void 			pause_active();

	void 			resume();
	void 			resume_active();
	void 			resume_inactive();

	void 			queue_sound(lda::sound_struct es) {queue.insert(es);}
	void 			do_queue() {queue.process();}

	private:

	lda::audio_controller&			ac;
	lda::sound_queue			queue;
	std::vector<unsigned int> 		paused_channels;


};

}
#endif
