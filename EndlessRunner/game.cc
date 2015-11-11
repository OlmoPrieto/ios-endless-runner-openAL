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
#include <stdlib.h>
#include <string>
#include <stdio.h>

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
    support::ReleaseMusic(background_music);
}

void Game::setupGL() {
	utils::LogInfo("EndlessRunner", "Game::setupGL()");

    glGenBuffers(1, &buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 50, NULL, GL_STATIC_DRAW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	loadShaders();

    // Textures
    glGenTextures(1, &Player::texture_id_);
    glBindTexture(GL_TEXTURE_2D, Player::texture_id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    bool has_alpha;
    player.set_texture(support::LoadImageFile("player_texture.jpg", &width, &height,
                                                         &has_alpha));
    if (player.texture_handler() == NULL) {
        utils::LogDebug("errors", "FAILED TO LOAD TEXTURE\n");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 player.texture_handler());
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenTextures(1, &Obstacle::texture_goomba_);
    glBindTexture(GL_TEXTURE_2D, Obstacle::texture_goomba_);
    unsigned char *texture_data = support::LoadImageFile("goomba.png", &width, &height,
                                                         &has_alpha);
    if (texture_data == NULL) {
        utils::LogDebug("errors", "FAILED TO LOAD OBSTACLE TEXTURE\n");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenTextures(1, &Obstacle::texture_koopa_);
    glBindTexture(GL_TEXTURE_2D, Obstacle::texture_koopa_);
    unsigned char *texture_data2 = support::LoadImageFile("koopa.png", &width, &height,
                                                         &has_alpha);
    if (texture_data2 == NULL) {
        utils::LogDebug("errors", "FAILED TO LOAD OBSTACLE TEXTURE\n");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 texture_data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenTextures(1, &floor_texture_id_);
    glBindTexture(GL_TEXTURE_2D,floor_texture_id_);
    unsigned char *texture_data3 = support::LoadImageFile("bricks.png", &width, &height,
                                                          &has_alpha);
    if (texture_data3 == NULL) {
        utils::LogDebug("errors", "FAILED TO LOAD OBSTACLE TEXTURE\n");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 texture_data3);
    glGenerateMipmap(GL_TEXTURE_2D);

    Obstacle::texture_id_ = Obstacle::texture_goomba_;
    free(texture_data);
    free(texture_data2);
    free(texture_data3);

	is_opengl_init_ = true;
}
	
void Game::teardownGL() {
	if (is_opengl_init_ == true) {
		is_opengl_init_ = false;
		
		if (program_handle_) {
			glDeleteProgram(program_handle_);
			program_handle_ = 0;
		}
        if (buffer_id_) {
            glDeleteBuffers(1, &buffer_id_);
        }
        if (Player::texture_id_) {
            glDeleteTextures(1, &Player::texture_id_);
        }
        if (Obstacle::texture_id_) {
            glDeleteTextures(1, &Obstacle::texture_goomba_);
            glDeleteTextures(1, &Obstacle::texture_koopa_);
        }
        if (floor_texture_id_) {
            glDeleteTextures(1, &floor_texture_id_);
        }
		utils::LogInfo("EndlessRunner", "Game::teardownGL()");
	}
}
	
void Game::initialize(int width, int height) {
    width_ = width;
    height_ = height;
    
    hi_score_ = 0;
    
    utils::LogInfo("EndlessRunner", "Game::initialize(%d, %d)", width_, height_);
    
    // Main Music
    background_music = support::LoadMusic("bgmusic.mp3");

    game_is_paused_ = false;
    
    current_scene_ = SCENE_MENU;

    max_obstacles_ = 6;
    obstacle_pool_.reserve(max_obstacles_);
    for (int i = 0; i < max_obstacles_; ++i) {
        Obstacle o;
        o.init(20000.0f, floor_height_ + 100.0f, 100.0f,
               100.0f, 75.0f);
        obstacle_pool_.push_back(o);
    }
    
    restartGameValues();
    
}
    
void Game::restartGameValues(){
    
    srand(time(NULL));

    score_ = 0;
    
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
    player.is_alive(true);
    
    for (int i = 0; i < max_obstacles_; ++i) {
        obstacle_pool_[i].set_position(20000.0f, floor_height_ + 100.0f);
        obstacle_pool_[i].set_availability(true);
    }
    
    // Restart music
    support::RestartMusic(background_music);
    
    // reset timer
    secs_last_update_ = 0.0;
}

void Game::touchDownAt(float x, float y) {
    player.is_jumping(true);
}
	
void Game::touchUpAt(float x, float y) {
    player.is_jumping(false);
}
  
void Game::screenTouched(){
    if(current_scene_ == SCENE_MENU){
        current_scene_ = SCENE_GAME;
    }else if(current_scene_ == SCENE_GAME){
        
    }else if(current_scene_ == SCENE_END){
        current_scene_ = SCENE_MENU;
        restartGameValues();
    }
}

void Game::pause() {
	is_paused_ = true;
	utils::LogInfo("EndlessRunner", "Game::pause()");
    support::PauseMusic(background_music);
    const char *path = support::PathToFileInDocuments("save.txt");
    FILE *f = fopen(path, "w");
    if (f != NULL) {
        fprintf(f, "%f\n", secs_last_update_);
        fprintf(f, "%u\n", max_obstacles_);
        fprintf(f, "%u\n", current_scene_);
        fprintf(f, "%u\n", hi_score_);
        fprintf(f, "%u\n", score_);
        fprintf(f, "%f\n", floor_x_pos_);
        fprintf(f, "%f\n", floor_y_pos_);
        fprintf(f, "%f\n", floor_width_);
        fprintf(f, "%f\n", floor_height_);
        fprintf(f, "%f\n", floor_one_x_position_);
        fprintf(f, "%f\n", floor_two_x_position_);
        fprintf(f, "%f\n", player.x());
        fprintf(f, "%f\n", player.y());
        fprintf(f, "%f\n", player.width());
        fprintf(f, "%f\n", player.height());
        fprintf(f, "%u\n", player.alive());
        fprintf(f, "%u\n", player.jumping());
        fprintf(f, "%f\n", Obstacle::speed_);
        for (int i = 0; i < max_obstacles_; ++i) {
            fprintf(f, "%f\n", obstacle_pool_[i].x());
            fprintf(f, "%f\n", obstacle_pool_[i].y());
            fprintf(f, "%f\n", obstacle_pool_[i].width());
            fprintf(f, "%f\n", obstacle_pool_[i].height());
            fprintf(f, "%u\n", obstacle_pool_[i].available());
        }
        fclose(f);
    } else {
        utils::LogDebug("Errors", "Failed to open save.txt to save data\n");
    }
}

void Game::resume() {
	//is_paused_ = false;
	utils::LogInfo("EndlessRunner", "Game::resume()");
    support::PlayMusic(background_music);
    const char *path = support::PathToFileInDocuments("save.txt");
    FILE *f = fopen(path, "r");
    if (f != NULL) {
        fscanf(f, "%lf", &secs_last_update_);
        fscanf(f, "%u", &max_obstacles_);
        unsigned int scene;
        fscanf(f, "%u", &scene);
        switch (scene) {
            case 0: {
                current_scene_ = SCENE_MENU;
                break;
            }
            case 1: {
                current_scene_ = SCENE_GAME;
                break;
            }
            case 2: {
                current_scene_ = SCENE_END;
                break;
            }
                
            default:
                break;
        }
        fscanf(f, "%u", &hi_score_);
        fscanf(f, "%u", &score_);
        fscanf(f, "%f", &floor_x_pos_);
        fscanf(f, "%f", &floor_y_pos_);
        fscanf(f, "%f", &floor_width_);
        fscanf(f, "%f", &floor_height_);
        fscanf(f, "%f", &floor_one_x_position_);
        fscanf(f, "%f", &floor_two_x_position_);
        float data;
        fscanf(f, "%f", &data);
        player.set_x(data);
        fscanf(f, "%f", &data);
        player.set_y(data);
        fscanf(f, "%f", &data);
        player.set_width(data);
        fscanf(f, "%f", &data);
        player.set_height(data);
        bool bool_data;
        fscanf(f, "%d", &bool_data);
        player.is_alive(bool_data);
        fscanf(f, "%d", &bool_data);
        player.is_jumping(bool_data);
        fscanf(f, "%f", &Obstacle::speed_);
        for (int i = 0; i < max_obstacles_; ++i) {
            fscanf(f, "%f", &data);
            obstacle_pool_[i].set_x(data);
            fscanf(f, "%f", &data);
            obstacle_pool_[i].set_y(data);
            fscanf(f, "%f", &data);
            obstacle_pool_[i].set_width(data);
            fscanf(f, "%f", &data);
            obstacle_pool_[i].set_height(data);
            fscanf(f, "%d", &bool_data);
            obstacle_pool_[i].set_availability(bool_data);
        }

        fclose(f);
    } else {
        utils::LogDebug("Errors", "Failed to open save.txt to load data\n");
    }

    if (current_scene_ != SCENE_GAME) {
        current_scene_ = SCENE_MENU;
        restartGameValues();
    } else {
        game_is_paused_ = true;
        update();
    }
}


void Game::update() {
	if (is_paused_ || !is_opengl_init_) return;
    
    if(current_scene_ == SCENE_MENU){
    
    }else if(current_scene_ == SCENE_GAME){


        if(!game_is_paused_){
        score_++;
        if (score_ % 300 == 0) {
            Obstacle::speed_ += 5.0f;
        }
        
        if(!support::IsMusicPlaying(background_music)){
            support::PlayMusic(background_music);
        }
        
        
        // utils::LogInfo("EndlessRunner", "Game update.");
	
        // Calculate time since last update
        if (secs_last_update_ == 0.0) {
            secs_last_update_ = utils::GetTimeSeconds();
        }
        double secs_current_time = utils::GetTimeSeconds();
        double secs_since_last_update = secs_current_time - secs_last_update_;
        secs_last_update_ = secs_current_time;
        static float accum_time = 0;
        accum_time += secs_since_last_update;
    
        // Dirty Floor Movement
        floor_one_x_position_ -= 5.0f;
        floor_two_x_position_ -= 5.0f;
    
        if(floor_one_x_position_ <= (-floor_width_)){
            floor_one_x_position_ = 0.0f + floor_width_;
        }
        if(floor_two_x_position_ <= (-floor_width_)){
            floor_two_x_position_ = 0.0f + floor_width_;
        }

        // obstacle spawn
        if (accum_time > 2.3f) {
            /*
             *  each 2.3 seconds, spawn an obstacle
             *  a spawned obstacle is not available.
             *  when an obstacle reaches the left side of the screen,
             *  it automatically sets its position off the screen and makes itself available
             */
            accum_time = 0.0f;
            float color[3];
            Obstacle *obs = &obstacle_pool_[0];
            for (int i = 0; i < max_obstacles_; ++i) {
                obs = &obstacle_pool_[i];
                if (obs->available()) {
                    obs->set_availability(false);
                    obs->set_x(1000.0f);
                    if (rand() % 20 < 5) {
                        obs->set_y(400.0f);
                        Obstacle::texture_id_ = Obstacle::texture_koopa_;
                    } else {
                        Obstacle::texture_id_ = Obstacle::texture_goomba_;
                    }
                    //color[0] = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
                    //color[1] = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
                    //color[2] = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
                    obs->set_color(color);
                    break;
                }
            }
        }

        for (int i = 0; i < max_obstacles_; ++i) {
            // only update not available obstacles (spawned obstacles)
            if (!obstacle_pool_[i].available()) {
                obstacle_pool_[i].update(secs_since_last_update);
            }
        }
  
        // Player gravity
        if(player.y() <= floor_y_pos_ + player.height()){
            player.is_grounded(true);
        }
  
        // Update objects
        player.update();
  
        // Check collision between the player and the obstacles
        {
            Obstacle *obs = NULL;
            for (int i = 0; i < max_obstacles_; ++i) {
                obs = &obstacle_pool_[i];
                if (Object::colliding(player, *obs)) {
                    utils::LogDebug("COLLISIONS", "Player is colliding!\n");
                    player.die();
                    endGame();
                }
            }
        }
    }

    }

    
    
}


void Game::render() {
	if (!is_opengl_init_) return;
	
	// utils::LogInfo("EndlessRunner", "Game render.");
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(program_handle_);
    
    if(current_scene_ == SCENE_MENU){
        
    }else if(current_scene_ == SCENE_GAME){
        // for now, only the player has a texture so each object
        // has to set an uniform in the shader indicating it has no texture
        drawFloor(floor_one_x_position_);
        drawFloor(floor_two_x_position_);
        drawCharacter();
        drawBoxes();
    }else if (current_scene_ == SCENE_END){
        
    }
    
}
    
unsigned char Game::current_scene()const{
    return current_scene_;
}
    
void Game::drawFloor(float pos_x){
    glUniform3f(glGetUniformLocation(program_handle_, "u_color"), 1.0f, 1.0f, 1.0f);
    glUniform1i(glGetUniformLocation(program_handle_, "has_texture"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, floor_texture_id_);
    glUniform1i(glGetUniformLocation(program_handle_, "u_texture"), 2);
    drawRect(pos_x, floor_y_pos_, floor_width_, floor_height_);
}

void Game::drawCharacter() {
    glUniform3f(glGetUniformLocation(program_handle_, "u_color"), 0.1f, 1.0f, 0.4f);
    glUniform1i(glGetUniformLocation(program_handle_, "has_texture"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Player::texture_id_);
    glUniform1i(glGetUniformLocation(program_handle_, "u_texture"), 0);
    drawRect(player.x(), player.y(), player.width(), player.height());
}
    
void Game::drawBoxes() {
    Obstacle *obs = NULL;
    for (int i = 0; i < max_obstacles_; ++i) {
        obs = &obstacle_pool_[i];
        // only draw available obstacles (spawned obstacles)
        if (!obs->available()) {
            glUniform3f(glGetUniformLocation(program_handle_, "u_color"),
                        obs->color()[0],
                        obs->color()[1],
                        obs->color()[2]);
            glUniform1i(glGetUniformLocation(program_handle_, "has_texture"), 1);
            glActiveTexture(GL_TEXTURE1);
            if (obs->y() < 400) {
                glBindTexture(GL_TEXTURE_2D, Obstacle::texture_goomba_);
            } else {
                glBindTexture(GL_TEXTURE_2D, Obstacle::texture_koopa_);
            }
            glUniform1i(glGetUniformLocation(program_handle_, "u_texture"), 1);
            drawRect(obs->x(), obs->y(), obs->width(), obs->height());
        }
    }
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

    float uv[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };
    //glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(uv), uv);

    unsigned int indices[] = {
        0, 1, 3,
        0, 3, 2
    };
    //glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(uv),
                    sizeof(indices), indices);

    //glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glVertexAttribPointer(glGetAttribLocation(program_handle_, "a_position"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(glGetAttribLocation(program_handle_, "a_position"));
    //glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glVertexAttribPointer(glGetAttribLocation(program_handle_, "a_uv"), 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertices));
    glEnableVertexAttribArray(glGetAttribLocation(program_handle_, "a_uv"));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(vertices) + sizeof(uv)));
}

bool Game::gameIsPaused()const{
    return game_is_paused_;
}

void Game::setGamePause(bool status){
    game_is_paused_ = status;
}
  
int Game::score(){
    return score_;
}
    
int Game::best_score(){
    return hi_score_;
}

  
void Game::endGame(){
    set_best_score(score_);
    support::PauseMusic(background_music);
    current_scene_ = SCENE_END;
}
    
void Game::set_best_score(int score){
    if(score >= best_score()){
        hi_score_ = score;
    }
}

bool Game::loadShaders() {
    
	glClearColor(126 / 255.0f, 192 / 255.0f, 238 / 255.0f , 1.0f);
	
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