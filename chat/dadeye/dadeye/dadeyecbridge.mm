//
//  dadeyecbridge.m
//  dadeye
//
//  Created by Elias Sun on 12/24/22.
//
#import "dadeyewrapper.h"
#import "main.hpp"

@implementation DadEye
- (void) dadmode:(NSString*) sweetid {
    DadEyeWrapper().dadmode([sweetid cStringUsingEncoding:NSUTF8StringEncoding]);
}
- (void) sweetmode:(NSString*) dadid {
    DadEyeWrapper().sweetmode([dadid cStringUsingEncoding:NSUTF8StringEncoding], [[self picpath] cStringUsingEncoding:NSUTF8StringEncoding]);
}
- (void) generateid {
    DadEyeWrapper().generateid();
}
- (void) getscreen {
    DadEyeWrapper().getscreen([[self picpath] cStringUsingEncoding:NSUTF8StringEncoding]);
}
- (void) display {
    DadEyeWrapper().display([[self picpath] cStringUsingEncoding:NSUTF8StringEncoding]);
}
- (NSString*) picpath {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return [paths objectAtIndex:0];
}
@end
