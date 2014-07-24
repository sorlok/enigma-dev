/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/
#include <stdio.h>
#include <vector>
#include <map>
using std::vector;
using std::map;

#include "ALsystem.h"
#include "SoundChannel.h"
#include "SoundResource.h"
#include "SoundEmitter.h"

#include <time.h>
clock_t starttime;
clock_t elapsedtime;
clock_t lasttime;

bool load_al_dll();
size_t channel_num = 128;

ALfloat listenerPos[] = {0.0f,0.0f,0.0f};
ALfloat listenerVel[] = {0.0f,0.0f,0.0f};
ALfloat listenerOri[] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};

vector<SoundChannel*> sound_channels(0);
map<int, SoundResource*> sound_resources;
vector<SoundEmitter*> sound_emitters(0);

#include "Widget_Systems/widgets_mandatory.h" // show_error

namespace {
int next_sound_id = 0; //ID of the next sound to allocate (GM does not actually re-use sound IDs).
}


namespace enigma {

  int get_free_channel(double priority)
  {
    // test for channels not playing anything
    for (size_t i = 0; i < sound_channels.size(); i++) {
      ALint state;
      alGetSourcei(sound_channels[i]->source, AL_SOURCE_STATE, &state);
      if (state != AL_PLAYING) {
        return i;
      }
    }
    // finally if you still couldnt find an empty channel, and we have a few more we can generate
    // go ahead and generate a new one
    if (sound_channels.size() < channel_num) {
      int i = sound_channels.size();
      ALuint src;
      alGenSources(1, &src);
      SoundChannel* sndsrc = new SoundChannel(src,-1);
      sound_channels.push_back(sndsrc);
      return i;
    }
    // test for channels playing a lower priority sound, and take their spot if they are
    for (size_t i = 0; i < sound_channels.size(); i++) {
      if (sound_channels[i]->priority < priority) {
        return i;
      }
    }

    return -1;
  }

  void eos_callback(void *soundID, ALuint src)
  {
    get_sound(snd, (ptrdiff_t)soundID, );
    snd->playing = false;
    snd->idle = true;
  }

  int audiosystem_initialize()
  {
    starttime = clock();
    elapsedtime = starttime;
    lasttime = elapsedtime;
    printf("Initializing audio system...\n");

    /*#ifdef _WIN32
    if (!load_al_dll())
      return 1;
	printf("Starting ALURE (Windows thing).\n");
	init_alure();
    #endif*/

    printf("Opening ALURE devices.\n");
    if(!alureInitDevice(NULL, NULL)) {
      fprintf(stderr, "Failed to open OpenAL device: %s\n", alureGetErrorString());
      return 1;
    }

    return 0;
  }

  SoundResource* sound_new_with_source() {
    SoundResource *res = new SoundResource();
    alGetError();
    int a = alGetError();
    if(a != AL_NO_ERROR) {
      fprintf(stderr, "Failed to create OpenAL source! Error %d: %s\n",a,alGetString(a));
      printf("%d %d %d %d %d\n",AL_INVALID_NAME,AL_INVALID_ENUM,AL_INVALID_VALUE,AL_INVALID_OPERATION,AL_OUT_OF_MEMORY);
      delete res;
      return NULL;
    }
    res->loaded = LOADSTATE_SOURCED;
    return res;
  }

  int sound_add_from_buffer(int id, void* buffer, size_t bufsize)
  {
    if (sound_resources.find(id)!=sound_resources.end() && sound_resources[id]) {
      fprintf(stderr, "Sound id %d collides with existing id (for internally-managed ids).\n", id);
      return 3;
    }

    SoundResource *snd = sound_new_with_source();
    sound_resources[id] = snd;
    if (id>=next_sound_id) { next_sound_id=id+1; }

    if (snd->loaded != LOADSTATE_SOURCED) {
      fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
      return 1;
    }
    ALuint& buf = snd->buf[0];
    buf = alureCreateBufferFromMemory((ALubyte*)buffer, bufsize);

    if(!buf) {
      fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
      return 2;
    }

    snd->loaded = LOADSTATE_COMPLETE;
    return 0;
  }

  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void *userdata)
  {
    if (sound_resources.find(id)!=sound_resources.end() && sound_resources[id]) {
      fprintf(stderr, "Sound id %d collides with existing id (for internally-managed ids).\n", id);
      return 3;
    }

    SoundResource *snd = sound_new_with_source();
    sound_resources[id] = snd;
    if (id>=next_sound_id) { next_sound_id=id+1; }

    if (snd->loaded != LOADSTATE_SOURCED)
      return 1;

    snd->stream = alureCreateStreamFromCallback((ALuint (*)(void*, ALubyte*, ALuint))callback, userdata, AL_FORMAT_STEREO16, 44100, 4096, 0, NULL);
    if (!snd->stream) {
      fprintf(stderr, "Could not create stream %d: %s\n", id, alureGetErrorString());
      return 1;
    }
    snd->cleanup = cleanup;
    snd->userdata = userdata;
    snd->seek = seek;

    snd->loaded = LOADSTATE_COMPLETE;
    return 0;
  }

  int sound_allocate()
  {
    int id = next_sound_id++;
    sound_resources[id] = NULL;
    return id;
  }

  void audiosystem_update(void)
  {
    alureUpdate();
  }

  void audiosystem_cleanup()
  {
    for (std::map<int, SoundResource*>::iterator it=sound_resources.begin(); it!=sound_resources.end(); it++) 
    {
      SoundResource* sr = it->second;
      if (!sr) { continue; }
      switch (sr->loaded)
      {
        case LOADSTATE_COMPLETE:
          alDeleteBuffers(sr->stream ? 3 : 1, sr->buf);
          if (sr->stream) {
            alureDestroyStream(sr->stream, 0, 0);
            if (sr->cleanup) sr->cleanup(sr->userdata);
          }
          // fallthrough
        case LOADSTATE_SOURCED:
          for (size_t j = 0; j < sound_channels.size(); j++) {
            alureStopSource(sound_channels[j]->source, true);
            alDeleteSources(1, &sound_channels[j]->source);
            delete sound_channels[j];
          }
          sound_channels.clear();
          break;
  
        case LOADSTATE_INDICATED:
        case LOADSTATE_NONE:
        default: ;
      }
    }

    alureShutdownDevice();
  }

}
