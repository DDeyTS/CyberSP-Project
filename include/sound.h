#pragma once

#include "main.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

ALLEGRO_AUDIO_STREAM *LoadMusic(const char *file);
void StopMusic(ALLEGRO_AUDIO_STREAM *stream);
void DestroyMusic(ALLEGRO_AUDIO_STREAM *stream);
void PlayMusic(ALLEGRO_AUDIO_STREAM *stream);
void PauseMusic(ALLEGRO_AUDIO_STREAM *stream);




