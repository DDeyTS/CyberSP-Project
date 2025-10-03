//**************************************************************************
//**
//** File: sound.c (CyberSP Project)
//** Purpose: Soudntrack stuff
//** Last Update: 03-10-2025 00:19
//** Author: DDeyTS
//**
//**************************************************************************

#include "sound.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/blender.h>

ALLEGRO_AUDIO_STREAM *LoadMusic(const char *file)
{
    ALLEGRO_AUDIO_STREAM *stream = al_load_audio_stream(file, 4, 2048);
    if (!stream) return NULL;

    al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_LOOP);

    return stream;
}

void PlayMusic(ALLEGRO_AUDIO_STREAM *stream)
{
    if (stream) {
        al_attach_audio_stream_to_mixer(stream, al_get_default_mixer());
        al_set_audio_stream_playing(stream, true);
    }
}

// FIXME: it isn't reseting the song.
void PauseMusic(ALLEGRO_AUDIO_STREAM *stream)
{
    if (stream) al_set_audio_stream_playing(stream, false);
}

void StopMusic(ALLEGRO_AUDIO_STREAM *stream)
{
    if (stream) {
        al_set_audio_stream_playing(stream, false);
        al_detach_audio_stream(stream);
    }
}

void DestroyMusic(ALLEGRO_AUDIO_STREAM *stream)
{
    if (stream) al_destroy_audio_stream(stream);
}
