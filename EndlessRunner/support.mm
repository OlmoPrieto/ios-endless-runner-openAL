//
//  support.mm
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "support.h"

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIImage.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVAudioPlayer.h>

#include "utils.h"

namespace support {

const unsigned int kMaxTextureSize = 1024;
    


typedef enum {
	kGLTexturePixelFormat_Automatic = 0,
	kGLTexturePixelFormat_RGBA8888,
	kGLTexturePixelFormat_RGB565,
	kGLTexturePixelFormat_A8,
} GLTexturePixelFormat;

const char* PathForResource(const char* resource) {
	NSString* file =[NSString stringWithUTF8String:resource];
	
	NSString* fileName = [[file lastPathComponent] stringByDeletingPathExtension];
	NSString* fileType = [file pathExtension];
	
	NSString* path = [[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
	if (path == nil) {
		utils::LogDebug("EndlessRunner", "Could not find file resource '%@.%@'", fileName, fileType);
		return NULL;
	}
	
	return [path UTF8String];
}

const char* ContentsOfFile(const char* file) {
	NSString *fileNS =[NSString stringWithUTF8String:file];
	NSString *sourceNS = [NSString stringWithContentsOfFile:fileNS encoding:NSUTF8StringEncoding error:nil];
	char* tmp = (char *)[sourceNS UTF8String];
	
	char *source = (char*)calloc([sourceNS length]+1, 1);
	strncpy(source, tmp, [sourceNS length]);
	
	return source;
}

const char* LoadVertexShader() {
	const char *file = PathForResource("Shader.vsh");
	return ContentsOfFile(file);
}

const char* LoadFragmentShader() {
	const char *file = PathForResource("Shader.fsh");
	return ContentsOfFile(file);
}


unsigned char* LoadImageFile(const char* file, int* image_width, int* image_height, bool* has_alpha)
{
	NSUInteger				width, height, i;
	CGContextRef			cgContext = nil;
	void*					data = NULL;
	CGColorSpaceRef			colorSpace;
	void*					tempData;
	unsigned int*			inPixel32;
	unsigned short*			outPixel16;
	BOOL					hasAlpha;
	CGImageAlphaInfo		info;
	CGAffineTransform		transform;
	CGSize					imageSize;
	GLTexturePixelFormat    pixelFormat;
	CGImageRef				image;
	BOOL					sizeToFit = NO;
	
	NSString *fileNS =[NSString stringWithUTF8String:file];
	NSString *fileName = [[fileNS lastPathComponent] stringByDeletingPathExtension];
	NSString *fileType = [fileNS pathExtension];
	NSString *path = [[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
	if (path == nil) {
		utils::LogDebug("EndlessRunner", "ERROR - Could not find image file '%@.%@'", fileName, fileType);
		return NULL;
	}
	
	UIImage *uiImage = [UIImage imageWithContentsOfFile:path];
	image = [uiImage CGImage];
	if(image == NULL) {
		utils::LogDebug("EndlessRunner", "Image is Null");
		return NULL;
	}
	
	
	info = CGImageGetAlphaInfo(image);
	
	hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);
	
	if(CGImageGetColorSpace(image)) {
		if(hasAlpha) {
			pixelFormat = kGLTexturePixelFormat_RGBA8888;
		} else {
			pixelFormat = kGLTexturePixelFormat_RGB565;
		}
	} else {  //NOTE: No colorspace means a mask image
		pixelFormat = kGLTexturePixelFormat_A8;
	}
	
	
	imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	transform = CGAffineTransformIdentity;
	
	width = imageSize.width;
	if((width != 1) && (width & (width - 1))) {
		i = 1;
		while((sizeToFit ? 2 * i : i) < width) {
			i *= 2;
		}
		width = i;
	}
	
	height = imageSize.height;
	if((height != 1) && (height & (height - 1))) {
		i = 1;
		while((sizeToFit ? 2 * i : i) < height)
		{
			i *= 2;
		}
		height = i;
	}
	
	while((width > kMaxTextureSize) || (height > kMaxTextureSize)) {
		width /= 2;
		height /= 2;
		transform = CGAffineTransformScale(transform, 0.5, 0.5);
		imageSize.width *= 0.5;
		imageSize.height *= 0.5;
	}
	
	switch(pixelFormat) {
		case kGLTexturePixelFormat_RGBA8888:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = malloc(height * width * 4);
			cgContext = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
			break;
		case kGLTexturePixelFormat_RGB565:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = malloc(height * width * 4);
			cgContext = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
			break;
			
		case kGLTexturePixelFormat_A8:
			data = malloc(height * width);
			cgContext = CGBitmapContextCreate(data, width, height, 8, width, NULL, kCGImageAlphaOnly);
			break;
		default:
			[NSException raise:NSInternalInconsistencyException format:@"Invalid pixel format"];
	}
	
	
	CGContextClearRect(cgContext, CGRectMake(0, 0, width, height));
	CGContextTranslateCTM(cgContext, 0, height - imageSize.height);
	
	if(!CGAffineTransformIsIdentity(transform)) {
		CGContextConcatCTM(cgContext, transform);
	}
	
	CGContextDrawImage(cgContext, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
	
	CGContextRelease(cgContext);
	
	*image_width = (int)width;
	*image_height = (int)height;
	*has_alpha = true;
	
	return (unsigned char*)data;
}


void* LoadOpenALAudioData(const char* file, ALsizei *out_data_size, ALenum *out_data_format, ALsizei* out_sample_rate) {
	OSStatus                        err = noErr;
	UInt64                          fileDataSize = 0;
	AudioStreamBasicDescription     theFileFormat;
	UInt32                          thePropertySize = sizeof(theFileFormat);
	AudioFileID                     afid = 0;
	void*                           theData = NULL;
	
	NSString *fileNS =[NSString stringWithUTF8String:file];
	NSString *fileName = [[fileNS lastPathComponent] stringByDeletingPathExtension];
	NSString *fileType = [fileNS pathExtension];
	NSString *path = [[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
	if (path == nil) {
		utils::LogDebug("EndlessRunner", "ERROR - SoundManager: Could not find file '%@.%@'", fileName, fileType);
		return NULL;
	}
	CFURLRef fileURL = (__bridge CFURLRef)[NSURL fileURLWithPath:path];
	
	// Open a file with ExtAudioFileOpen()
	err = AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &afid);
	if(err) {
		utils::LogDebug("EndlessRunner", "LoadOpenALAudioData: AudioFileOpenURL FAILED, Error = %ld\n", err);
		if (afid) AudioFileClose(afid);
		return NULL;
	}
	
	// Get the audio data format
	err = AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &thePropertySize, &theFileFormat);
	if(err) {
		utils::LogDebug("EndlessRunner", "LoadOpenALAudioData: AudioFileGetProperty(kAudioFileProperty_DataFormat) FAILED, Error = %ld\n", err);
		if (afid) AudioFileClose(afid);
		return NULL;
	}
	
	if (theFileFormat.mChannelsPerFrame > 2)  {
		utils::LogDebug("EndlessRunner", "LoadOpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
		if (afid) AudioFileClose(afid);
		return NULL;
	}
	
	if ((theFileFormat.mFormatID != kAudioFormatLinearPCM) || (!TestAudioFormatNativeEndian(theFileFormat))) {
		utils::LogDebug("EndlessRunner", "LoadOpenALAudioData - Unsupported Format, must be little-endian PCM\n");
		if (afid) AudioFileClose(afid);
		return NULL;
	}
	
	if ((theFileFormat.mBitsPerChannel != 8) && (theFileFormat.mBitsPerChannel != 16)) {
		utils::LogDebug("EndlessRunner", "LoadOpenALAudioData - Unsupported Format, must be 8 or 16 bit PCM\n");
		if (afid) AudioFileClose(afid);
		return NULL;
	}
	
	
	thePropertySize = sizeof(fileDataSize);
	err = AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize);
	if(err) {
		utils::LogDebug("EndlessRunner", "LoadOpenALAudioData: AudioFileGetProperty(kAudioFilePropertyAudioDataByteCount) FAILED, Error = %ld\n", err);
		if (afid) AudioFileClose(afid);
		return NULL;
	}
	
	// Read all the data into memory
	UInt32 dataSize = (UInt32)fileDataSize;
	theData = malloc(dataSize);
	if (theData)
	{
		AudioFileReadBytes(afid, false, 0, &dataSize, theData);
		if(err == noErr)
		{
			// success
			*out_data_size = (ALsizei)dataSize;
			*out_data_format = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			*out_sample_rate = (ALsizei)theFileFormat.mSampleRate;
		}
		else
		{
			// failure
			free (theData);
			theData = NULL; // make sure to return NULL
			utils::LogDebug("EndlessRunner", "LoadOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err);
			if (afid) AudioFileClose(afid);
			return NULL;
		}
	}
	
	// Dispose the ExtAudioFileRef, it is no longer needed
	if (afid) AudioFileClose(afid);
	return theData;
}

const char* PathToFileInDocuments(const char* file) {
    NSString *_file = [NSString stringWithUTF8String:file];

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path_to_documents = [paths objectAtIndex:0];

    NSString *full_path = [NSString stringWithFormat:@"%@/%@", path_to_documents, _file];

    if (full_path == nil) {
        utils::LogDebug("Errors", "Could not find file resource '%@'", _file);
        return NULL;
    }

    return [full_path UTF8String];
}


void* LoadMusic(const char* file)
{
    AVAudioPlayer* musicPlayer;
    
    NSError *error = nil;
    musicPlayer = [AVAudioPlayer alloc];
    
    NSString *fileNS =[NSString stringWithUTF8String:file];
    NSString *fileName = [[fileNS lastPathComponent] stringByDeletingPathExtension];
    NSString *fileType = [fileNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
    NSURL *fileUrl = [[NSURL alloc] initFileURLWithPath:path];

    [musicPlayer initWithContentsOfURL:fileUrl error:&error];
    
    if (error) {
        NSLog(@"Error loading Background music");
    }
	return (__bridge_retained void*)musicPlayer;
}

void ReleaseMusic(void* music_player)
{
	AVAudioPlayer* mp = (__bridge_transfer AVAudioPlayer*)music_player;
}

bool PlayMusic(void* music_player){
    AVAudioPlayer* mp = (__bridge AVAudioPlayer*)music_player;
    
    return [mp play];
}

void RestartMusic(void* music_player){
    AVAudioPlayer* mp = (__bridge AVAudioPlayer*)music_player;
    [mp stop];
    mp.currentTime = 0;
}

void PauseMusic(void* music_player)
{
    AVAudioPlayer* mp = (__bridge AVAudioPlayer*)music_player;
    
    return [mp pause];
}

bool IsMusicPlaying(void* music_player)
{
    
    AVAudioPlayer* mp = (__bridge AVAudioPlayer*)music_player;
    
    return [mp isPlaying];
}
	
void* LoadSound(const char* file)
{
    AVAudioPlayer* musicPlayer;
    
    NSError *error = nil;
    musicPlayer = [AVAudioPlayer alloc];
    
    NSString *fileNS =[NSString stringWithUTF8String:file];
    NSString *fileName = [[fileNS lastPathComponent] stringByDeletingPathExtension];
    NSString *fileType = [fileNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
    NSURL *fileUrl = [[NSURL alloc] initFileURLWithPath:path];
    
    [musicPlayer initWithContentsOfURL:fileUrl error:&error];
    
    if (error) {
        NSLog(@"Error loading sound");
    }
    return (__bridge_retained void*)musicPlayer;
}
	
void ReleaseSound(void* music_player)
{
   AVAudioPlayer* mp = (__bridge_transfer AVAudioPlayer*)music_player;
}
	
bool PlaySound(void* music_player)
{
    AVAudioPlayer* mp = (__bridge AVAudioPlayer*)music_player;
    
    return [mp play];
}

}  // namespace support
