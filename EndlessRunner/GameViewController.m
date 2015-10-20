//
//  GameViewController.m
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#import "GameViewController.h"

#import "AVFoundation/AVAudioSession.h"

#import "GameWrapper.h"

@interface GameViewController () {
}
@property (strong, nonatomic) EAGLContext *context;


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
    [[self gameWrapper] update];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // Render with ES2
    [[self gameWrapper] render];
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
