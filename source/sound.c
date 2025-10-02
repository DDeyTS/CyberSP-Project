
#include "sound.h"

ALLEGRO_AUDIO_STREAM *LoadMusic(const char *file)
{
    ALLEGRO_AUDIO_STREAM *stream = al_load_audio_stream(file, 4, 2048);
    if (!stream) return NULL;

    al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(stream, al_get_default_mixer());

    return stream;
}

void StopMusic(ALLEGRO_AUDIO_STREAM *stream)
{
    if (stream) al_drain_audio_stream(stream);
}

void DestroyMusic(ALLEGRO_AUDIO_STREAM *stream)
{
    if (stream) al_destroy_audio_stream(stream);
}
