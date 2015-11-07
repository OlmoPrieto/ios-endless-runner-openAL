//
//  player.hpp
//  EndlessRunner
//
//  Created by Jose Luis Jimenez on 25/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include <stdio.h>

#include "object.h"

#endif /* player_hpp */

class Player : public Object{
public:
  Player();
  ~Player(){};
  
  void init();
  void update();
  void is_jumping(bool jumping);
  bool jumping()const;
    void is_grounded(bool grounded);
    
private:
    float is_jumping_;
   
    bool played_jump_sound_ = false;
    
};