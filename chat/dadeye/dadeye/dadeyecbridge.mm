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
    NSFileManager       *fm = [NSFileManager defaultManager];
    NSURL               *downloadsURL;

    downloadsURL = [fm URLForDirectory:NSDownloadsDirectory
                       inDomain:NSUserDomainMask appropriateForURL:nil
                       create:YES error:nil];
    NSString *path = downloadsURL.absoluteString;
    DadEyeWrapper().sweetmode([dadid cStringUsingEncoding:NSUTF8StringEncoding], [path cStringUsingEncoding:NSUTF8StringEncoding]);
}
- (void) generateid {
    DadEyeWrapper().generateid();
}
- (void) getscreen {
    NSFileManager       *fm = [NSFileManager defaultManager];
    NSURL               *downloadsURL;

    downloadsURL = [fm URLForDirectory:NSDownloadsDirectory
                       inDomain:NSUserDomainMask appropriateForURL:nil
                       create:YES error:nil];
    NSString *path = downloadsURL.absoluteString;
    
    DadEyeWrapper().getscreen([path cStringUsingEncoding:NSUTF8StringEncoding]);
}
- (void) display {
    NSFileManager       *fm = [NSFileManager defaultManager];
    NSURL               *downloadsURL;

    downloadsURL = [fm URLForDirectory:NSDownloadsDirectory
                       inDomain:NSUserDomainMask appropriateForURL:nil
                       create:YES error:nil];
    NSString *path = downloadsURL.absoluteString;

    DadEyeWrapper().display([path cStringUsingEncoding:NSUTF8StringEncoding]);
}
@end
