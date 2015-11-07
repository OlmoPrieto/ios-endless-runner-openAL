//
//  utils.h
//  EndlessRunner
//
//  Created by Marcos Martí Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef __UTILS_H__
#define __UTILS_H__ 1

namespace utils {
	
double GetTimeSeconds();

void LogInfo(const char *tag, const char *format, ...);
void LogDebug(const char *tag, const char *format, ...);
void LogWarn(const char *tag, const char *format, ...);
void LogError(const char *tag, const char *format, ...);
	
}  // namespace utils

#endif  // __UTILS_H__
