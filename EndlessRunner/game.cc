//
//  game.cc
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "game.h"

#include <OpenGLES/ES2/gl.h>
#include <stddef.h>

#include "support.h"
#include "utils.h"

namespace endlessrunner {
	
Game::Game()
	: is_opengl_init_(false),
	is_paused_(false),
	secs_last_update_(0.0),
	width_(0),
	height_(0),
	program_handle_(0),
	attribute_position_(0),
	uniform_mvp_matrix_(0) {
    utils::LogInfo("EndlessRunner", "Game::Game()");
}

Game::~Game() {
	utils::LogInfo("EndlessRunner", "Game::~Game()");
}

void Game::setupGL() {
	utils::LogInfo("EndlessRunner", "Game::setupGL()");

    glGenBuffers(1, &buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, NULL, GL_STATIC_DRAW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	loadShaders();
	
	is_opengl_init_ = true;
}
	
void Game::teardownGL() {
	if (is_opengl_init_ == true) {
		is_opengl_init_ = false;
		
		if (program_handle_) {
			glDeleteProgram(program_handle_);
			program_handle_ = 0;
		}
		utils::LogInfo("EndlessRunner", "Game::teardownGL()");
	}
}
	
void Game::initialize(int width, int height) {
	width_ = width;
	height_ = height;
	utils::LogInfo("EndlessRunner", "Game::initialize(%d, %d)", width_, height_);
    
    // Screen variables
  
    // Initialize map variables
    floor_height_ = 50.0f;
    floor_width_ = 1000.0f; // Hardcode now -> next, get screen width
    floor_x_pos_ = 0.0f;
    floor_y_pos_ = 0.0f + floor_height_;
	
    floor_one_x_position_ = floor_x_pos_;
    floor_two_x_position_ = floor_one_x_position_ + floor_width_;

  
    player.set_width(100.0f);
    player.set_height(200.0f);
    player.set_position(100.0f, floor_height_ + player.height() + 100.0f);
    player.isAffectedByGravity(true);
    player.is_grounded(false);

    max_obstacles_ = 6;
    obstacle_pool_.reserve(max_obstacles_);
    for (int i = 0; i < max_obstacles_; ++i) {
        Obstacle o;
        o.init(800.0f + i * 100.0f, floor_height_ + 100.0f, 100.0f,
               100.0f, 75.0f);
        obstacle_pool_.push_back(o);
    }
    
	// reset timer
	secs_last_update_ = 0.0;
}

void Game::touchDownAt(float x, float y) {

}
	
void Game::touchUpAt(float x, float y) {
}
  
void Game::screenTouched(){
  player.jump();
}

void Game::pause() {
	is_paused_ = true;
	utils::LogInfo("EndlessRunner", "Game::pause()");
}

void Game::resume() {
	is_paused_ = false;
	utils::LogInfo("EndlessRunner", "Game::resume()");
}


void Game::update() {
	if (is_paused_ || !is_opengl_init_) return;
  
  
	
	// utils::LogInfo("EndlessRunner", "Game update.");
	
	// Calculate time since last update
	if (secs_last_update_ == 0.0) {
		secs_last_update_ = utils::GetTimeSeconds();
	}
	double secs_current_time = utils::GetTimeSeconds();
	double secs_since_last_update = secs_current_time - secs_last_update_;
	secs_last_update_ = secs_current_time;
    
    // Dirty Floor Movement
    
    floor_one_x_position_ -= 5.0f;
    floor_two_x_position_ -= 5.0f;
    
    if(floor_one_x_position_ <= (-floor_width_)){
        floor_one_x_position_ = 0.0f + floor_width_;
    }
    if(floor_two_x_position_ <= (-floor_width_)){
        floor_two_x_position_ = 0.0f + floor_width_;
    }
    
    for (int i = 0; i < max_obstacles_; ++i) {
        obstacle_pool_[i].update(secs_since_last_update);
    }

    utils::LogDebug("obstacle x: ", "\n\n%f\n\n", obstacle_pool_[0].x());
  
  // Player gravity
  if(player.y() <= floor_y_pos_ + player.height()){
    player.is_grounded(true);
    player.is_jumping(false);
  }
  
  // Update objects
  player.update();
  
}


void Game::render() {
	if (!is_opengl_init_) return;
	
	// utils::LogInfo("EndlessRunner", "Game render.");
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
    
  glUseProgram(program_handle_);
    
  //drawRectBad(300.0f, 400.0f, 100.0f, 200.0f);
  drawFloor(floor_one_x_position_);
  drawFloor(floor_two_x_position_);
  drawCharacter();
  drawBoxes();
}
    
void Game::drawFloor(float pos_x){
    glUniform3f(glGetUniformLocation(program_handle_, "u_color"), 1.0f, 1.0f, 1.0f);
    drawRect(pos_x, floor_y_pos_, floor_width_, floor_height_);
}

void Game::drawCharacter() {
    glUniform3f(glGetUniformLocation(program_handle_, "u_color"), 0.1f, 1.0f, 0.4f);
    drawRect(player.x(), player.y(), player.width(), player.height());
}
    
void Game::drawBoxes() {
    glUniform3f(glGetUniformLocation(program_handle_, "u_color"), 1.0f, 1.0f, 0.0f);
    //drawRectBad(box1_x_, box1_y_, box_width_, box_height_);
    drawRect(obstacle_pool_[0].x(), obstacle_pool_[0].y(),
             obstacle_pool_[0].width(), obstacle_pool_[0].height());
}
	
// TODO(Students) Bad implemented, reimplement!!!!
void Game::drawRectBad(float x, float y, float width, float height) const {
	// Ortho matrix
	GLfloat matrix_ortho[] = {2.0f/width_, 0.0f, 0.0f, 0.0f,
							  0.0f, 2.0f/height_, 0.0f, 0.0f,
							  0.0f, 0.0f, -1.f, 0.0f,
							 -1.0f, -1.0f, -0.0f, 1.0f};
	
	// Set model view projection matrix
	glUniformMatrix4fv(uniform_mvp_matrix_, 1, 0, matrix_ortho);

	
	// Vertices
	GLfloat vertices[] = {x, y-height, 0.0f,
						  x+width, y-height, 0.0f,
						  x, y, 0.0f,
						  x+width, y, 0.0f};
	
	// Load the vertex data
	glEnableVertexAttribArray(attribute_position_);
	glVertexAttribPointer(attribute_position_, 3, GL_FLOAT, GL_FALSE, 0,
						  vertices);
		
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Game::drawRect(float x, float y, float width, float height) const {
    // Ortho matrix
    GLfloat matrix_ortho[] = {2.0f/width_, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f/height_, 0.0f, 0.0f,
        0.0f, 0.0f, -1.f, 0.0f,
							 -1.0f, -1.0f, -0.0f, 1.0f};

    // Set model view projection matrix
    glUniformMatrix4fv(uniform_mvp_matrix_, 1, 0, matrix_ortho);


    float vertices[] = {
        x, y, 0.0f,
        x, y - height, 0.0f,
        x + width, y, 0.0f,
        x + width, y - height, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    unsigned int indices[] = {
        0, 1, 3,
        0, 3, 2
    };
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(indices), indices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)sizeof(vertices));
}

bool Game::loadShaders() {
	glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
	
	// shaders
	const char *vert_shader_str = support::LoadVertexShader();
	const char *frag_shader_str = support::LoadFragmentShader();
	
	program_handle_ = loadProgram(vert_shader_str, frag_shader_str);
	if (program_handle_ == 0)
		return false;
	
	// Get the attribute locations
	attribute_position_ = glGetAttribLocation(program_handle_, "a_position");
	
	// Get uniform locations.
	uniform_mvp_matrix_ = glGetUniformLocation(program_handle_,
												"u_modelViewProjectionMatrix");
	
	delete vert_shader_str;
	delete frag_shader_str;
	
	return true;
}
	
int Game::loadProgram(const char* str_vert_source,
					  const char* str_frag_source) {
	int id_vert_shader;
	int id_frag_shader;
	int id_program;
	int link[1];
	
	id_vert_shader = loadShader(str_vert_source, GL_VERTEX_SHADER);
	if (id_vert_shader == 0) {
		utils::LogDebug("EndlessRunner", "Load vertex shader failed.");
		return 0;
	}
	
	id_frag_shader = loadShader(str_frag_source, GL_FRAGMENT_SHADER);
	if (id_frag_shader == 0) {
		utils::LogDebug("EndlessRunner", "Load fragment shader failed.");
		return 0;
	}
	
	id_program = glCreateProgram();
	
	// attach shaders to program
	glAttachShader(id_program, id_vert_shader);
	glAttachShader(id_program, id_frag_shader);
	
	// link program
	glLinkProgram(id_program);
	
	// get the link status
	glGetProgramiv(id_program, GL_LINK_STATUS, link);
	if (link[0] <= 0) {
		GLint log_length;
		glGetProgramiv(id_program, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			GLchar *log = new GLchar[log_length];
			glGetProgramInfoLog(id_program, log_length, &log_length, log);
			utils::LogDebug("EndlessRunner", "Failed load program: %s", log);
			delete log;
		}
		utils::LogDebug("EndlessRunner", "Load program failed.");
		return 0;
	}
		
	// delete the shaders, since we have already loaded
	glDeleteShader(id_vert_shader);
	glDeleteShader(id_frag_shader);
	return id_program;
}
	
int Game::loadShader(const char* str_source, int id_type) {
	int compiled[1];
	int id_shader = glCreateShader(id_type);
	glShaderSource(id_shader, 1, &str_source, 0);
	glCompileShader(id_shader);
	glGetShaderiv(id_shader, GL_COMPILE_STATUS, compiled);
	if (compiled[0] == 0) {
		GLint log_length;
		glGetShaderiv(id_shader, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			GLchar *log = new GLchar[log_length];
			glGetShaderInfoLog(id_shader, log_length, &log_length, log);
			utils::LogDebug("EndlessRunner", "Failed loadShader: %s", log);
			delete log;
		}
		return 0;
	}
	return id_shader;
}
	
}  // namespace endlessrunner