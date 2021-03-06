//
//  GameWrapper.mm
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#import "GameWrapper.h"

#include "game.h"

@interface GameWrapper () {
    void* _game;
}
@end


@implementation GameWrapper

- (id) init
{
    self = [super init];
    if(self)
    {
        _game = new endlessrunner::Game();
    }
    return self;
}

- (void) dealloc
{
    delete (endlessrunner::Game*)_game;
}

- (void)setupGL
{
    ((endlessrunner::Game*)_game)->setupGL();
}

- (void)tearDownGL
{
    ((endlessrunner::Game*)_game)->teardownGL();
}

- (void)initializeWithWidth:(float)width andHeight:(float)height
{
    ((endlessrunner::Game*)_game)->initialize(width, height);
}

- (void)touchDownAtPosX:(float)x posY:(float)y
{
	((endlessrunner::Game*)_game)->touchDownAt(x, y);
}

- (void)touchUpAtPosX:(float)x posY:(float)y
{
    ((endlessrunner::Game*)_game)->touchUpAt(x, y);
}

-(void)screenTouched{
  ((endlessrunner::Game*)_game)->screenTouched();
}

-(int)score{
  return ((endlessrunner::Game*)_game)->score();
}
-(int)best_score{
    return ((endlessrunner::Game*)_game)->best_score();
}
-(unsigned char)current_scene{
    return ((endlessrunner::Game*)_game)->current_scene();
}
-(BOOL)game_is_paused{
    return ((endlessrunner::Game*)_game)->gameIsPaused();
}
-(void)set_game_paused:(bool)status{
    ((endlessrunner::Game*)_game)->setGamePause(status);
}

- (void)pause
{
    ((endlessrunner::Game*)_game)->pause();
}

- (void)resume
{
    ((endlessrunner::Game*)_game)->resume();
}

- (void)update
{
    ((endlessrunner::Game*)_game)->update();
}

- (void)render
{
    ((endlessrunner::Game*)_game)->render();
}


@end

