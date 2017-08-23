/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-Present by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#import "TiAisle411MapViewProxy.h"
#import "TiUtils.h"
#import "TiAisle411MapView.h"

#import <MapSDK/MapSDK.h>

@implementation TiAisle411MapViewProxy

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

- (void)setZoomButtonsHidden:(id)floor
{
  ENSURE_TYPE(floor, NSNumber);
  [[[self mapView] mapController] setZoomButtonsHidden:[TiUtils boolValue:floor]];
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

- (void)setBackgroundColor:(id)backgroundColor
{
  ENSURE_TYPE(backgroundColor, NSString);
  [[[self mapView] mapController] setMapBackgroundColor:[TiUtils colorValue:backgroundColor].color];
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
  NSMutableArray<FMProduct *> *nativeProducts = [NSMutableArray array];
  
  if (!inputProducts) {
    NSLog(@"[ERROR] Missing required parameter 'products!'");
    return;
  }
  
  _products = inputProducts;
  
  assert(venueItems.count == _products.count);
  
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
    [nativeProducts addObject:product];
  }
  
  [[self overlay] setProducts:nativeProducts];
  
  TiThreadPerformOnMainThread(^{
    [[[self mapView] mapController] redrawOverlay:[self overlay]];
  }, NO);
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
