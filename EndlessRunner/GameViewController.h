//
//  GameViewController.h
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@class GameWrapper;

@interface GameViewController : GLKViewController

@property (strong, nonatomic) GameWrapper *gameWrapper;

@end
