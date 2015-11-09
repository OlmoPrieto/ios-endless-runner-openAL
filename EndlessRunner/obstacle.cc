//
//  obstacle.cpp
//  EndlessRunner
//
//  Created by Olmo on 25/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "obstacle.h"

unsigned int Obstacle::texture_id_ = 0;
unsigned int Obstacle::texture_goomba_ = 0;
unsigned int Obstacle::texture_koopa_ = 0;
float Obstacle::speed_ = 0.0f;

Obstacle::Obstacle() {

}

Obstacle::~Obstacle() {

}

void Obstacle::init(float x, float y, float width, float height, float speed) {
    Object::init(x, y, width, height);
    speed_ = speed;
    available_ = true;
    color_[0] = 1.0f;
    color_[1] = 1.0f;
    color_[2] = 1.0f;
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

void Obstacle::set_color(float color[3]) {
    color_[0] = color[0];
    color_[1] = color[1];
    color_[2] = color[2];
}

const float* Obstacle::color() const {
    return color_;
}

void Obstacle::update(float dt) {
    x_ -= speed_ * dt * 4;
    if (x_ < 0.0f - (width_ + width_ / 2.0f)) {
        x_ = 20000.0f;
        y_ = 50.f + height_;
        available_ = true;
    }
}