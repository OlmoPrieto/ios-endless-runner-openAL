//
//  obstacle.h
//  EndlessRunner
//
//  Created by Olmo on 25/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef obstacle_h
#define obstacle_h

#include "object.h"

class Obstacle : public Object {
public:
    Obstacle();
    ~Obstacle();

    void init(float x, float y, float width, float height, float speed);

    void set_speed(float speed);
    float speed() const;

    void set_availability(bool state);
    bool available() const;

    void update(float dt);

private:
    float speed_;
    bool available_;
};

#endif /* obstacle_h */
