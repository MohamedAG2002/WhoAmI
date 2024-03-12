#include "audio_manager.h"
#include "entities/trigger.h"
#include "collision/collision_space.h"

#include <raylib.h>

#include <vector>

namespace audio {

// Audio manager type
////////////////////////////////////////////////
struct audio_manager_t {
  std::vector<Sound> sounds;
  std::vector<Music> music;

  float master_volume, music_volume;
  int total_sounds, total_music;
};

static audio_manager_t s_man;
////////////////////////////////////////////////

// Callbacks
////////////////////////////////////////////////
void trigger_audio_play_callback(const entities::trigger_t& trigger) {
  if(trigger.type == entities::TRIGG_TYPE_SOUND) {
    PlaySound(s_man.sounds[s_man.total_sounds++]);
  }
  else if(trigger.type == entities::TRIGG_TYPE_MUSIC) {
     s_man.total_music++;

    // Stop the previous music if there is one 
    if(s_man.total_music >= 1) {
      StopMusicStream(s_man.music[s_man.total_music - 1]); 
    }

    PlayMusicStream(s_man.music[s_man.total_music]);
  }
  else if(trigger.type == entities::TRIGG_TYPE_MUSIC_STOP) {
    StopMusicStream(s_man.music[s_man.total_music]); // Stop the currently playing music 
  }
} 
////////////////////////////////////////////////

// Audio manager functions
////////////////////////////////////////////////
void init() {
  s_man.master_volume = 25.0f; 
  s_man.music_volume = 50.0f; 
  s_man.total_sounds = 0;
  s_man.total_music = -1;
  
  InitAudioDevice();
  SetMasterVolume(s_man.master_volume);

  collision::add_trigger_callback(trigger_audio_play_callback);

  // Adding sounds
  s_man.sounds.push_back(LoadSound("assets/audio/kids_playing-s1.mp3"));
  s_man.sounds.push_back(LoadSound("assets/audio/birds_chirps-s2.mp3"));
  s_man.sounds.push_back(LoadSound("assets/audio/child_laugh-s3.mp3"));
  s_man.sounds.push_back(LoadSound("assets/audio/water_plop-s4.ogg"));
  s_man.sounds.push_back(LoadSound("assets/audio/woman_sob-s5.wav"));
  s_man.sounds.push_back(LoadSound("assets/audio/woman_scream-s6.wav"));
  s_man.sounds.push_back(LoadSound("assets/audio/fire-s7.wav"));
  s_man.sounds.push_back(LoadSound("assets/audio/birds_chirps_sad-s8.mp3"));
  s_man.sounds.push_back(LoadSound("assets/audio/fire_whoosh-s9.wav"));

  // Adding music 
  s_man.music.push_back(LoadMusicStream("assets/audio/nocturne-m1.mp3"));
  s_man.music.push_back(LoadMusicStream("assets/audio/rain-m2.wav"));
  s_man.music.push_back(LoadMusicStream("assets/audio/gymnopedie-m3.mp3"));
}

void shutdown() {
  for(auto& sound : s_man.sounds) {
    UnloadSound(sound);
  }
  s_man.sounds.clear();

  for(auto& music : s_man.music) {
    UnloadMusicStream(music);
  }
  s_man.music.clear();

  CloseAudioDevice();
}

void update() {
  for(auto& music : s_man.music) {
    UpdateMusicStream(music);
  }
}

void reset() {
  for(auto& music : s_man.music) {
    StopMusicStream(music);
  }
  
  for(auto& sound : s_man.sounds) {
    StopSound(sound);
  }

  s_man.total_music = -1;
  s_man.total_sounds = 0;
}
////////////////////////////////////////////////

}
