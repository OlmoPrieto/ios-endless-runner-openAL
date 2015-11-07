//
//  support.h
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef __SUPPORT_H__
#define __SUPPORT_H__ 1

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace support {

// Functions load shaders
const char* LoadVertexShader();
const char* LoadFragmentShader();
	
// Function for load images
unsigned char* LoadImageFile(const char* file, int* image_width,
								 int* image_height, bool* has_alpha);
	
// Function for load audio
void* LoadOpenALAudioData(const char* file, ALsizei *out_data_size,
							  ALenum *out_data_format, ALsizei* out_sample_rate);
	
// Function for save state
const char* PathToFileInDocuments(const char* file);
	
// Functions for music
void* LoadMusic(const char* file);
void ReleaseMusic(void* music_player);
bool PlayMusic(void* music_player);
void PauseMusic(void* music_player);
void RestartMusic(void* music_player);
bool IsMusicPlaying(void* music_player);

// Functions for sounds
void* LoadSound(const char* file);
void ReleaseSound(void* music_player);
bool PlaySound(void* music_player);
	
}  // namespace support

#endif  // __SUPPORT_H__
