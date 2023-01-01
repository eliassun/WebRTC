//
//  dadeyewrapper.h
//  dadeye
//
//  Created by Elias Sun on 12/24/22.
//

#import <Foundation/Foundation.h>
@interface DadEye : NSObject
- (void) dadmode: (NSString*) sweetid;
- (void) sweetmode:(NSString*) dadid;
- (void) generateid;
- (void) getscreen;
- (void) display;
- (NSString*) picpath;
@end


