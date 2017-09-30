/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-Present by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#import "TiAisle411MapViewProxy.h"
#import "TiAisle411MapView.h"
#import "TiUtils.h"

#import <AisleNetworking/AisleNetworking-Swift.h>
#import <MapSDK/MapSDK.h>

@implementation TiAisle411MapViewProxy

- (NSArray *)keySequence
{
  return @[@"url", @"shoppingListEnabled", @"selectedPinImage", @"unselectedPinImage"];
}

- (TiAisle411MapView *)mapView
{
  return (TiAisle411MapView *)[self view];
}

- (ProductCalloutOverlay *)overlay
{
  return [[self mapView] overlay];
}

#pragma mark Public API's

- (void)setFloor:(id)floor
{
  ENSURE_TYPE(floor, NSNumber);
  [[[self mapView] mapController] setFloor:[TiUtils intValue:floor]];
}

- (void)setZoomButtonsHidden:(id)zoomButtonsHidden
{
  ENSURE_TYPE(zoomButtonsHidden, NSNumber);
  [[[self mapView] mapController] setZoomButtonsHidden:[TiUtils boolValue:zoomButtonsHidden]];
}

- (void)setLogoPosition:(id)logoPosition
{
  ENSURE_TYPE(logoPosition, NSNumber);
  [[[self mapView] mapController] setLogoPosition:[TiUtils intValue:logoPosition def:AisleLogoRightBottomPosition]];
}

- (void)setCompassEnabled:(id)compassEnabled
{
  ENSURE_TYPE(compassEnabled, NSNumber);
  [[[self mapView] mapController] setCompassEnabled:[TiUtils boolValue:compassEnabled]];
}

- (void)setMapBackgroundColor:(id)backgroundColor
{
  ENSURE_TYPE(backgroundColor, NSString);
  [[[self mapView] mapController] setMapBackgroundColor:[TiUtils colorValue:backgroundColor].color];
}

- (void)setKeyWordLabelText:(id)keyWordLabelText
{
  ENSURE_TYPE(keyWordLabelText, NSString);

  NSArray *overlays = [[[self mapView] mapController] overlays];

  if (overlays == nil || [overlays count] != 1) {
    NSLog(@"[ERROR] No overlay added or too many overlays existing. Skipping ...");
    return;
  }

  [[(ProductCalloutOverlay *)[overlays objectAtIndex:0] informationBar] setKeyWordLabelText:keyWordLabelText];
}

- (void)deselectAll:(id)unused
{
  [[[self mapView] mapController] deselectAll];
}

- (void)clear:(id)unused
{
  [[[self mapView] mapController] clear];
}

- (void)zoomIn:(id)unused
{
  [[[self mapView] mapController] zoomIn];
}

- (void)zoomOut:(id)unused
{
  [[[self mapView] mapController] zoomOut];
}

- (void)redraw:(id)unused
{
  [[[self mapView] mapController] redraw];
}

- (void)redrawOverlay:(id)args
{
  ENSURE_SINGLE_ARG_OR_NIL(args, NSDictionary);

  NSArray *venueItems = [args objectForKey:@"venueItems"];
  NSArray *inputProducts = [args objectForKey:@"products"];
  NSMutableArray<FMProduct *> *sdkProducts = [NSMutableArray array];

  _products = inputProducts;

  if (inputProducts && venueItems && venueItems.count == inputProducts.count) {
    for (NSDictionary *inputProduct in _products) {
      FMProduct *product = [[FMProduct alloc] init];
      NSDictionary *venueItem = [venueItems objectAtIndex:[_products indexOfObject:inputProduct]];

      product.name = [inputProduct objectForKey:@"name"];
      product.idn = [TiUtils intValue:[inputProduct objectForKey:@"id"]];
      product.checked = [TiUtils boolValue:[inputProduct objectForKey:@"isPickedUp"] def:NO];

      NSArray *sections = (NSArray *)[venueItem objectForKey:@"sections"];
      NSMutableArray<FMSection *> *productSectionArray = [NSMutableArray arrayWithCapacity:[sections count]];

      for (NSDictionary *section in sections) {
        FMSection *newSection = [[FMSection alloc] init];
        newSection.maplocation = [(NSNumber *)[section valueForKey:@"mapPointId"] integerValue];

        if (newSection.maplocation == 0) {
          continue;
        }

        newSection.aisleTitle = [section valueForKey:@"aisle"];
        newSection.title = [section valueForKey:@"section"];

        [productSectionArray addObject:newSection];
      }

      product.sections = productSectionArray;
      [sdkProducts addObject:product];
    }
  }

  [[self overlay] setProducts:sdkProducts];

  TiThreadPerformOnMainThread(^{
    [[[self mapView] mapController] redrawOverlay:[self overlay]];
  },
      NO);
  
  // Justin Boswell
  //  - Due to the crash issue being a result of a dead information bar begin shown,
  //      we can remove it from the superview and have it regenerated when the pin is clicked.
  TiThreadPerformOnMainThread(^{
    [[[self overlay] informationBar] removeFromSuperview];
  },
      NO);
  
}

- (void)reloadTiles:(id)unused
{
  [[[self mapView] mapController] reloadTiles];
}

- (void)fadeIn:(id)args
{
  ENSURE_SINGLE_ARG(args, NSDictionary);

  NSNumber *zoom = [args objectForKey:@"zoom"];
  NSNumber *relativeToScreen = [args objectForKey:@"relativeToScreen"];

  [[[self mapView] mapController] fadeInWithZoom:zoom.floatValue
                                relativeToScreen:relativeToScreen.boolValue];
}

- (void)search:(id)args
{
  ENSURE_SINGLE_ARG(args, NSDictionary);

  if ([[self mapView] shoppingModeEnabled]) {
    [self _searchInShoppingList:args];
  } else {
    [self _searchWithFulltext:args];
  }
}

#pragma mark Utilities

- (void)_searchInShoppingList:(NSDictionary *)args
{

  NSNumber *venueId = [args objectForKey:@"venueId"];
  NSString *name = [args objectForKey:@"name"];
  NSArray *products = [args objectForKey:@"products"]; // { name: '', id: '', section: '', ... }
  KrollCallback *callback = [args objectForKey:@"callback"];

  AisleServer *server = [AisleServer shared];

  NSMutableArray *items = [NSMutableArray arrayWithCapacity:products.count];

  for (NSDictionary *item in products) {
    [items addObject:@{ @"name" : [item valueForKey:@"sectionCode"] ?: @"" }];
  }

  [server searchWithVenueWithId:venueId.integerValue
      forItemsFromDictionary:@{ @"name" : name,
        @"items" : items }
      withResponseBlock:^(NSArray<IVKVenueItem *> *venueItems, NSArray<IVKError *> *errors) {
        if (errors.count > 0) {
          [callback call:@[ @{ @"error" : [[errors objectAtIndex:0] description] } ] thisObject:self];
          return;
        }

        NSMutableArray *dictVenueItems = [NSMutableArray arrayWithCapacity:venueItems.count];

        for (IVKVenueItem *venueItem in venueItems) {
          [dictVenueItems addObject:[self dictionaryFromVenueItem:venueItem]];
        }
        [callback call:@[ @{ @"venueItems" : dictVenueItems } ] thisObject:self];
      }];
}

- (void)_searchWithFulltext:(NSDictionary *)args
{

  NSNumber *venueId = [args objectForKey:@"venueId"];
  NSString *searchTerm = [args objectForKey:@"searchTerm"];
  NSNumber *startingIndex = [args objectForKey:@"startingIndex"];
  NSNumber *endingIndex = [args objectForKey:@"endingIndex"];
  NSNumber *maxCount = [args objectForKey:@"maxCount"];
  KrollCallback *callback = [args objectForKey:@"callback"];

  AisleServer *server = [AisleServer shared];

  [server searchWithVenueWithId:venueId.integerValue
                        forTerm:searchTerm
              withStartingIndex:startingIndex.integerValue
                 andEndingIndex:endingIndex.integerValue
                   withMaxCount:maxCount.integerValue
              withResponseBlock:^(NSArray<IVKVenueItem *> *venueItems, NSArray<IVKError *> *errors) {
                if (errors.count > 0) {
                  [callback call:@[ @{ @"error" : [[errors objectAtIndex:0] description] } ] thisObject:self];
                  return;
                }

                NSMutableArray *dictVenueItems = [NSMutableArray arrayWithCapacity:venueItems.count];

                for (IVKVenueItem *venueItem in venueItems) {
                  [dictVenueItems addObject:[self dictionaryFromVenueItem:venueItem]];
                }
                [callback call:@[ @{ @"venueItems" : dictVenueItems } ] thisObject:self];
              }];
}

- (NSDictionary *)dictionaryFromVenueItem:(IVKVenueItem *)venueItem
{
  return @{
    @"id" : NUMINTEGER(venueItem.id),
    @"name" : venueItem.name,
    @"price" : NUMDOUBLE(venueItem.price),
    @"discountedPrice" : NUMDOUBLE(venueItem.discountedPrice),
    @"section" : venueItem.sectionName,
    @"sections" : [self arrayFromSections:venueItem.sections],
    @"venueItemTypeName" : venueItem.venueItemTypeName
  };
}

- (NSArray *)arrayFromSections:(NSArray<IVKSection *> *)sections
{
  NSMutableArray *result = [NSMutableArray arrayWithCapacity:sections.count];

  for (IVKSection *section in sections) {
    [result addObject:@{
      @"aisle" : section.aisle,
      @"mapPointId" : NUMINTEGER(section.mapPointId),
      @"section" : section.section
    }];
  }

  return result;
}

#pragma mark Layout Helper

USE_VIEW_FOR_CONTENT_WIDTH

USE_VIEW_FOR_CONTENT_HEIGHT

- (TiDimension)defaultAutoWidthBehavior:(id)unused
{
  return TiDimensionAutoFill;
}

- (TiDimension)defaultAutoHeightBehavior:(id)unused
{
  return TiDimensionAutoFill;
}

@end
