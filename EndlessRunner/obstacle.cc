//
//  obstacle.cpp
//  EndlessRunner
//
//  Created by Olmo on 25/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "obstacle.h"

Obstacle::Obstacle() {
    speed_ = 0.0f;
}

Obstacle::~Obstacle() {

}

void Obstacle::init(float x, float y, float width, float height, float speed) {
    Object::init(x, y, width, height);
    speed_ = speed;
    available_ = true;
}

void Obstacle::set_speed(float speed) {
    speed_ = speed;
}

float Obstacle::speed() const {
    return speed_;
}

void Obstacle::set_availability(bool state) {
    available_ = state;
}

bool Obstacle::available() const {
    return available_;
}

void Obstacle::update(float dt) {
    x_ -= speed_ * dt;
    if (x_ < 0.0f - (width_ + width_ / 2.0f)) {
        x_ = 1000.0f;
    }
}