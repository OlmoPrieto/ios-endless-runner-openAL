//
//  object.cpp
//  EndlessRunner
//
//  Created by Olmo on 21/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "object.h"

Object::Object() {

}

Object::~Object() {

}

void Object::init(float x, float y, float width, float height) {
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;
}

void Object::set_x(float x) {
    x_ = x;
}

void Object::set_y(float y) {
    y_ = y;
}

void Object::set_width(float width) {
    width_ = width;
}

void Object::set_height(float height) {
    height_ = height;
}

void Object::set_position(float x, float y) {
    x_ = x;
    y_ = y;
}

float Object::x() const {
    return x_;
}

float Object::y() const {
    return y_;
}

float Object::width() const {
    return width_;
}

float Object::height() const {
    return height_;
}