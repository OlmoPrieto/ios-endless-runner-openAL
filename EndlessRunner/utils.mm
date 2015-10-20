//
//  utils.mb
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include "utils.h"

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>


namespace utils {
	
double GetTimeSeconds() {
	double current_time = CACurrentMediaTime();
	return current_time;
}
	
	
void LogInfo(const char *tag, const char* format, ...) {
	va_list ap;
		
	va_start(ap, format);
	NSString *form =[NSString stringWithUTF8String:format];
	NSLogv(form, ap);
	va_end (ap);
}
	
void LogDebug(const char *tag, const char* format, ...) {
	va_list ap;
	
	va_start(ap, format);
	NSString *form =[NSString stringWithUTF8String:format];
	NSLogv(form, ap);
	va_end (ap);
}
	
void LogWarn(const char *tag, const char* format, ...) {
	va_list ap;
	
	va_start(ap, format);
	NSString *form =[NSString stringWithUTF8String:format];
	NSLogv(form, ap);
	va_end (ap);
}
	
void LogError(const char *tag, const char* format, ...) {
	va_list ap;
	
	va_start(ap, format);
	NSString *form =[NSString stringWithUTF8String:format];
	NSLogv(form, ap);
	va_end (ap);
}
	
	
}  // namespace utils