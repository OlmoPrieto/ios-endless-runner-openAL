//
//  object.h
//  EndlessRunner
//
//  Created by Olmo on 21/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef object_h
#define object_h

class Object {
  public:
    Object();
    ~Object();

    void init(float x, float y, float width, float height);

    void set_x(float x);
    void set_y(float y);
    void set_width(float width);
    void set_height(float height);

    void set_position(float x, float y);

    float x() const;
    float y() const;
    float width() const;
    float height() const;
    
  protected:
    float x_;
    float y_;
    float width_;
    float height_;
};

#endif /* object_h */
