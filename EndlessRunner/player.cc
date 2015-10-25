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


void Player::jump(){
  
  if(grounded() && !is_jumping_){
    
  }
  
}