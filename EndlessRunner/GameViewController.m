//
//  GameViewController.m
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#import "GameViewController.h"

#import "AVFoundation/AVAudioSession.h"
#import "AVFoundation/AVAudioPlayer.h"

#import "GameWrapper.h"

@interface GameViewController () {
}
@property (strong, nonatomic) EAGLContext *context;

@property (strong, nonatomic) UILabel *score_text;
@property (strong, nonatomic) UILabel *main_menu_text;
@property (strong, nonatomic) UILabel *current_score;
@property (strong, nonatomic) UILabel *best_score;


- (void)setupGL;
- (void)tearDownGL;
@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	
	// Initialize Audio Session
	[self initAudioSession];
	
    // Create GameWrapper
    self.gameWrapper = [[GameWrapper alloc] init];
    
    [self setupGL];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    NSLog(@"viewWillAppear");
    float width = self.view.bounds.size.width;
    float height = self.view.bounds.size.height;
    float scale = [[UIScreen mainScreen] scale];
  
    self.main_menu_text = [[UILabel alloc] initWithFrame:CGRectMake(10, 10, 20, 15)];
    self.score_text = [[UILabel alloc] initWithFrame:CGRectMake(10, 10, 20, 15)];
    self.current_score = [[UILabel alloc] initWithFrame:CGRectMake(10, 10, 20, 15)];
    self.best_score = [[UILabel alloc] initWithFrame:CGRectMake(10, 10, 20, 15)];
  
    [self.gameWrapper initializeWithWidth:(width*scale) andHeight:(height*scale)];
}

- (void)dealloc
{
    NSLog(@"dealloc");
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    [[self gameWrapper] setupGL];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    [[self gameWrapper] tearDownGL];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    unsigned char current_scene = [[self gameWrapper] current_scene];

    if(current_scene == 0){ // Main Menu
        [self.score_text removeFromSuperview];
        [self.current_score removeFromSuperview];
        [self.best_score removeFromSuperview];
        
        self.main_menu_text.text = @"Press anywhere to play:";
        self.main_menu_text.frame = CGRectMake(90, 50, 400, 100);
        self.main_menu_text.font = [UIFont boldSystemFontOfSize:25.0f];
        self.main_menu_text.textColor = [UIColor whiteColor];
        self.main_menu_text.backgroundColor = [UIColor clearColor];
        [self.view addSubview:self.main_menu_text];
    }else if(current_scene == 1){ // Game
        
        // Remove other labels
        [self.main_menu_text removeFromSuperview];
        [self.current_score removeFromSuperview];
        [self.best_score removeFromSuperview];
        
        // Score
        self.score_text.text = [NSString stringWithFormat:@"Score:  %d", [[self gameWrapper] score]];
        self.score_text.frame = CGRectMake(75, 35, 160, 20);
        self.score_text.font=[UIFont boldSystemFontOfSize:25.0];
        self.score_text.textColor=[UIColor whiteColor];
        self.score_text.backgroundColor=[UIColor clearColor];
        [self.view addSubview:self.score_text];
    }else if(current_scene == 2){ // End menu
        [self.score_text removeFromSuperview];
        [self.main_menu_text removeFromSuperview];
        
        self.current_score.text = [NSString stringWithFormat:@"Game Score  %d", [[self gameWrapper] score]];
        self.current_score.frame = CGRectMake(90, 50, 200, 100);
        self.current_score.font=[UIFont boldSystemFontOfSize:25.0];
        self.current_score.textColor=[UIColor whiteColor];
        self.current_score.backgroundColor=[UIColor clearColor];
        
        self.best_score.text = [NSString stringWithFormat:@"Best Score  %d", [[self gameWrapper] best_score]];
        self.best_score.frame = CGRectMake(90, 100, 200, 100);
        self.best_score.font=[UIFont boldSystemFontOfSize:25.0];
        self.best_score.textColor=[UIColor whiteColor];
        self.best_score.backgroundColor=[UIColor clearColor];
        
        [self.view addSubview:self.current_score];
        [self.view addSubview:self.best_score];
    }

  
  [[self gameWrapper] update];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // Render with ES2
    [[self gameWrapper] render];
}



// Get Touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
  
  for(UITouch *touch in touches) {
    CGPoint origTouchPos = [touch locationInView: touch.view];
      unsigned char current_scene = [[self gameWrapper] current_scene];
      [[self gameWrapper] screenTouched];
  }
}

#pragma mark - AVAudioSession and AVAudioSessionDelegate delegate methods
- (void)initAudioSession
{
	NSError *error = nil;
	AVAudioSession *session = [AVAudioSession sharedInstance];
	
	NSString *soundCategory = AVAudioSessionCategoryAmbient;
	[session setCategory:soundCategory error:&error];
	if(error) {
		NSLog(@"WARNING - AVAudioSession error setting session category: %@", [error localizedDescription]);
	}
	
	error = nil;
	[session setActive:YES error:&error];
	if(error) {
		NSLog(@"WARNING - AVAudioSession error activating: %@", [error localizedDescription]);
	}
	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(interruption:) name:AVAudioSessionInterruptionNotification object:nil];
}

- (void) interruption:(NSNotification*)notification
{
	NSDictionary *interuptionDict = notification.userInfo;
	NSUInteger interuptionType = (NSUInteger)[interuptionDict valueForKey:AVAudioSessionInterruptionTypeKey];
	
	if (interuptionType == AVAudioSessionInterruptionTypeBegan)
		[self beginInterruption];
	else if (interuptionType == AVAudioSessionInterruptionTypeEnded)
		[self endInterruption];
}

- (void)beginInterruption
{
	NSLog(@"AVAudioSession begin interruption");
}

- (void)endInterruption
{
	NSError *error = nil;
	AVAudioSession *session = [AVAudioSession sharedInstance];
	[session setActive:YES error:&error];
	if(error) {
		NSLog(@"WARNING - AVAudioSession error activating: %@", [error localizedDescription]);
	}
	
	NSLog(@"AVAudioSession end interruption");
}


@end
