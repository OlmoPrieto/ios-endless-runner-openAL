//
//  game.hpp
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef __GAME_H__
#define __GAME_H__ 1

#include "player.h"

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
        
	void pause();
	void resume();
        
	void update();
	void render();
        
 private:
	Game(const Game&);
	void operator=(const Game&);
        
	bool is_opengl_init_;
	bool is_paused_;
	double secs_last_update_;
	int width_;
	int height_;
  float speed_;
    
    
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
    
  // Character variables
  float char_x_;
  float char_y_;
  float char_width_;
  float char_height_;
  float jump_height_;
  bool is_jumping_;
  
  Player player;
    
  void drawCharacter();
    
  // boxes
  float box_width_;
  float box_height_;
  float box_speed_;
    
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
        
	// methods load shaders
	bool loadShaders();
	int loadProgram(const char* str_vert_source, const char* str_frag_source);
	int loadShader(const char* str_source, int id_type);
};

}  // namespace endlessrunner

#endif  // __GAME_H__
