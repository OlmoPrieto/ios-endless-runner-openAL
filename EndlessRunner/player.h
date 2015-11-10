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
    ~Player();
  
    void init();
    void update();
    void is_jumping(bool jumping);
    bool jumping()const;
    void die();
    void is_alive(bool alive);
    bool alive()const;
    void is_grounded(bool grounded);

    static unsigned int texture_id_;
    
private:
    bool is_alive_ = true;
    float is_jumping_;

    void* jump_sound;
    void* die_sound;
    bool played_jump_sound_ = false;
    
};