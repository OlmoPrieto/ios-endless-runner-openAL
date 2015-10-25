//
//  player.cpp
//  EndlessRunner
//
//  Created by Jose Luis Jimenez on 25/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "player.h"

Player::Player(){
  init();
}

void Player::init(){
  is_jumping_ = false;
}

void Player::is_jumping(bool jumping){
  is_jumping_ = jumping;
}

bool Player::jumping()const{
  return is_jumping_;
}

void Player::jump(){
  
  if(grounded() && !jumping()){
    is_jumping(true);
    is_grounded(false);
    
    // Change this
    set_y(400.0f);
    
  }
  
}