/**
 * titanium-aisle411
 *
 * Created by Hans Knöchel
 * Copyright (c) 2017 Axway Appcelerator. All rights reserved.
 */

#import "TiAisle411Module.h"
#import "TiBase.h"
#import "TiHost.h"
#import "TiUtils.h"

#import <AisleNetworking/AisleNetworking-Swift.h>
#import <MapSDK/MapSDK.h>

@implementation TiAisle411Module

#pragma mark Internal

- (id)moduleGUID
{
  return @"2bd872ef-d80a-4d07-be50-cbd424b67eb4";
}

- (NSString *)moduleId
{
  return @"ti.aisle411";
}

#pragma mark Lifecycle

- (void)startup
{
  [super startup];
  NSLog(@"[DEBUG] %@ loaded", self);
}

#pragma mark Public APIs

- (void)setup:(id)args
{
  NSLog(@"[WARN] Aisle411 setup() method is not supported on iOS. You must use a plist file instead.");
}

- (void)requestCachedRasterMap:(id)args
{
  ENSURE_SINGLE_ARG(args, NSDictionary);

  NSNumber *venueId = [args objectForKey:@"venueId"];
  KrollCallback *callback = [args objectForKey:@"callback"];

  AisleServer *server = [AisleServer shared];
  [server requestCachedRasterMapForVenueId:[venueId integerValue]
                         withResponseBlock:^(NSURL *url, NSArray<IVKError *> *errors) {
                           if (errors.count > 0) {
                             [callback call:@[ @{ @"error" : [[errors objectAtIndex:0] description] } ] thisObject:self];
                             return;
                           }

                           [callback call:@[ @{ @"path" : NULL_IF_NIL(url.path) } ]
                               thisObject:self];
                         }];
}

#pragma mark Constants

MAKE_SYSTEM_PROP(AISLE_LOGO_POSITION_RIGHT_BOTTOM, AisleLogoRightBottomPosition);
MAKE_SYSTEM_PROP(AISLE_LOGO_POSITION_LEFT_BOTTOM, AisleLogoLeftBottomPosition);

@end
