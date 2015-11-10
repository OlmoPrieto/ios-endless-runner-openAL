//
//  player.cpp
//  EndlessRunner
//
//  Created by Jose Luis Jimenez on 25/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "player.h"

unsigned int Player::texture_id_ = 0;

Player::Player(){
  init();
}
Player::~Player(){
    support::ReleaseSound(die_sound);
    support::ReleaseSound(jump_sound);
}

void Player::init(){
    is_jumping_ = false;
    die_sound = support::LoadSound("player_dead_sound.mp3");
    jump_sound = support::LoadSound("jump.mp3");
}

void Player::is_jumping(bool jumping){
  is_jumping_ = jumping;
}

bool Player::jumping()const{
  return is_jumping_;
}

void Player::is_grounded(bool grounded){
    Object::is_grounded(grounded);
    if(grounded){
        played_jump_sound_ = false;
    }
}

void Player::is_alive(bool alive){
    is_alive_ = alive;
}

bool Player::alive()const{
    return is_alive_;
}

void Player::die(){
    is_alive(false);
    support::PlaySound(die_sound);
}

void Player::update(){
    Object::update();
    
    if(jumping()){
        if(!played_jump_sound_){
            support::PlaySound(jump_sound);
            played_jump_sound_ = true;
        }
        is_grounded(false);
        set_y( y() + 20.0f );
    }
    
}