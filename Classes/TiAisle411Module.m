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

#import <MapSDK/MapSDK.h>
#import <AisleNetworking/AisleNetworking-Swift.h>

@implementation TiAisle411Module

#pragma mark Internal

// This is generated for your module, please do not change it
- (id)moduleGUID
{
	return @"2bd872ef-d80a-4d07-be50-cbd424b67eb4";
}

// This is generated for your module, please do not change it
- (NSString *)moduleId
{
	return @"ti.aisle411";
}

#pragma mark Lifecycle

- (void)startup
{
	// This method is called when the module is first loaded
	// You *must* call the superclass
	[super startup];
	NSLog(@"[DEBUG] %@ loaded",self);
}

#pragma mark Public APIs

- (void)requestCachedRasterMap:(id)args
{
  ENSURE_SINGLE_ARG(args, NSDictionary);
  
  NSNumber *venueId = [args objectForKey:@"venueId"];
  KrollCallback *callback = [args objectForKey:@"callback"];
  
  AisleServer *server = [AisleServer shared];
  [server requestCachedRasterMapForVenueId:[venueId integerValue]
                         withResponseBlock:^(NSURL *url, NSArray<IVKError *> *errors) {
                           if (errors.count > 0) {
                             [callback call:@[@{@"error": [[errors objectAtIndex:0] description]}] thisObject:self];
                             return;
                           }

                           [callback call:@[@{@"url": NULL_IF_NIL(url.absoluteString), @"path": NULL_IF_NIL(url.path)}] thisObject:self];
                         }];
}

- (void)search:(id)args
{
  ENSURE_SINGLE_ARG(args, NSDictionary);
  
  NSNumber *venueId = [args objectForKey:@"venueId"];
  NSString *name = [args objectForKey:@"name"];
  NSArray *products = [args objectForKey:@"products"]; // { name: '', id: '', section: '', ... }
  KrollCallback *callback = [args objectForKey:@"callback"];
  
  AisleServer *server = [AisleServer shared];
  
  NSMutableArray *items = [NSMutableArray arrayWithCapacity:products.count];
  
  for (NSDictionary *item in products) {
    [items addObject:@{@"name": [item valueForKey:@"sectionCode"] ?: @""}];
  }
  
  [server searchWithVenueWithId:venueId.integerValue
         forItemsFromDictionary:@{@"name": name, @"items": items}
              withResponseBlock:^(NSArray<IVKVenueItem *> *venueItems, NSArray<IVKError *> *errors) {
                if (errors.count > 0) {
                  [callback call:@[@{@"error": [[errors objectAtIndex:0] description]}] thisObject:self];
                  return;
                }
                
                NSMutableArray *dictVenues = [NSMutableArray arrayWithCapacity:venueItems.count];
                
                for (IVKVenueItem *venueItem in venueItems) {
                  [dictVenues addObject:@{
                    @"id": NUMINTEGER(venueItem.id),
                    @"name": venueItem.name,
                    @"price": NUMDOUBLE(venueItem.price),
                    @"discountedPrice": NUMDOUBLE(venueItem.discountedPrice),
                    @"section": venueItem.sectionName,
                    @"sections": [TiAisle411Module arrayFromSections:venueItem.sections],
                    @"venueItemTypeName": venueItem.venueItemTypeName, // More to be exposed here
                  }];
                }
                [callback call:@[@{@"venueItems": dictVenues}] thisObject:self];
              }];
}

+ (NSArray *)arrayFromSections:(NSArray<IVKSection *> *)sections
{
  NSMutableArray *result = [NSMutableArray arrayWithCapacity:sections.count];
  
  for (IVKSection *section in sections) {
    [result addObject:@{
      @"aisle": section.aisle,
      @"mapPointId": NUMINTEGER(section.mapPointId),
      @"section": section.section
    }];
  }
  
  return result;
}

#pragma mark Constants

MAKE_SYSTEM_PROP(AISLE_LOGO_POSITION_RIGHT_BOTTOM, AisleLogoRightBottomPosition);
MAKE_SYSTEM_PROP(AISLE_LOGO_POSITION_LEFT_BOTTOM, AisleLogoLeftBottomPosition);

@end
