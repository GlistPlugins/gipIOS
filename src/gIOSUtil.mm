#import <Foundation/Foundation.h>

const char* getResourceDirectory()
{
    NSString* path = [NSBundle mainBundle].resourcePath;
    return [path UTF8String];
}
