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
  affected_by_gravity_ = false;
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

bool Object::grounded()const{
  return is_grounded_;
}

void Object::update(){
  // Handle Gravity
  doGravity();
}

void Object::isAffectedByGravity(bool affected){
  affected_by_gravity_ = affected;
}

void Object::is_grounded(bool grounded){
  is_grounded_ = grounded;
}


void Object::doGravity(){
  
  if(affected_by_gravity_ && !grounded()){
    y_ -= 9.8f;
  }
  
}