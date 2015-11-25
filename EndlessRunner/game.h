//
//  game.hpp
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef __GAME_H__
#define __GAME_H__ 1

#include "obstacle.h"
#include "player.h"
#include "audio_source.h"

#include <vector>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace endlessrunner {
    
class Game {
 public:
	Game();
	~Game();
        
	void setupGL();
	void initialize(int width, int height);
	void teardownGL();
	
	void touchDownAt(float x, float y);
	void touchUpAt(float x, float y);
    void screenTouched();
    unsigned char current_scene()const;
    void set_best_score(int score);
        
	void pause();
	void resume();
        
	void update();
	void render();
  
    int score();
    int best_score();

    bool gameIsPaused()const;
    void setGamePause(bool status);

 private:
	Game(const Game&);
	void operator=(const Game&);
        
	bool is_opengl_init_;
	bool is_paused_;
	double secs_last_update_;
	int width_;
	int height_;
    float speed_;
    unsigned int max_obstacles_;
    enum Scenes { SCENE_MENU, SCENE_GAME, SCENE_END, SCENES_NUMBER };
    unsigned char current_scene_; // 0 : menu, 1 : game, 2 : end

    bool game_is_paused_;
  
    // Score
    unsigned int hi_score_;
    unsigned int score_;
  
	// GL ES
	int program_handle_;
	int attribute_position_;
	int uniform_mvp_matrix_;
    
    // Map Variables
    float floor_x_pos_;
    float floor_y_pos_;
    float floor_height_;
    float floor_width_;
    
    float floor_one_x_position_;
    float floor_two_x_position_;
    unsigned int floor_texture_id_;
    
    // Character variables
    float char_x_;
    float char_y_;
    float char_width_;
    float char_height_;
    float jump_height_;
    bool is_jumping_;
  
    Player player;
    
    void drawCharacter();
    
    void restartGameValues();
    
    // Music
    ALCdevice *device_;
    ALCcontext *context_;
    void* background_music;
    AudioSource background_music_;
    AudioListener listener_;
    
    // boxes
    std::vector<Obstacle> obstacle_pool_;

    void drawBoxes();
    
    // box1
    float box1_x_;
    float box1_y_;
    
    // box2
    float box2_x_;
    float box2_y_;
    
    void drawFloor(float pos_x);
    
	// draw a rectangle
	void drawRectBad(float x, float y, float width, float height) const;
    unsigned int buffer_id_;
    void drawRect(float x, float y, float width, float height) const;
    
    // Game finish
    void endGame();

  
	// methods load shaders
	bool loadShaders();
	int loadProgram(const char* str_vert_source, const char* str_frag_source);
	int loadShader(const char* str_source, int id_type);
};

}  // namespace endlessrunner

#endif  // __GAME_H__
