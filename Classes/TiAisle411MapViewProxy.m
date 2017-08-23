/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-Present by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#import "TiAisle411MapViewProxy.h"
#import "TiUtils.h"
#import "TiAisle411MapView.h"

#import <MapSDK/FMProduct.h>
#import <MapSDK/ProductCalloutOverlay.h>

#define PUBLIXCOLOR [UIColor colorWithRed:47/255 green:136/255 blue:15/255 alpha:1.0]

@implementation TiAisle411MapViewProxy

- (NSArray *)keySequence
{
  return @[@"url"];
}

- (id)_initWithPageContext:(id<TiEvaluator>)context
{
  if (self = [super _initWithPageContext:context]) {
    _productCallOutOverlay = [[ProductCalloutOverlay alloc] initWithInformationBarSupport];
    
    BOOL shoppingModeEnabled = [TiUtils boolValue:[self valueForKey:@"shoppingModeEnabled"] def:NO];
    
    InformationBar *informationBar = _productCallOutOverlay.informationBar;
    informationBar.backgroundColor = UIColor.whiteColor;
    [informationBar setLocationLabelBackgroundColor:PUBLIXCOLOR];
    informationBar.delegate = self;
    informationBar.dataSource = self;
    [informationBar hideInstructionLine:!shoppingModeEnabled];
    [informationBar hideInstructionLabel:!shoppingModeEnabled];
    
    [[informationBar table] registerClass:[UITableViewCell class] forCellReuseIdentifier:@"Cell"];    
  }
  
  return self;
}

- (TiAisle411MapView *)mapView
{
  return (TiAisle411MapView *)[self view];
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
  
  _productCallOutOverlay.products = nativeProducts;
  
  TiThreadPerformOnMainThread(^{
    [[[self mapView] mapController] redrawOverlay:_productCallOutOverlay];
  }, NO);
}

- (void)addOverlay:(id)args
{
  ENSURE_SINGLE_ARG(args, NSDictionary);
  [[[self mapView] mapController] addOverlay:_productCallOutOverlay];
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

#pragma mark Information Bar Data Source / Delegate

- (void)informationBar:(InformationBar *)informationBar didSelectRowAtIndex:(NSInteger)rowIndex forItem:(OverlayItem *)item
{
  if ([self _hasListeners:@"overlayitemclick"]) {
    [self fireEvent:@"overlayitemclick" withObject:@{@"title": item.title}]; // Return more if desired
  }
}

- (NSInteger)informationBar:(InformationBar*)informationBar numberOfRowsForItem:(OverlayItem*)item
{
  return [[(ProductOverlayItem *)item products] count];
}

- (UITableViewCell *)informationBar:(InformationBar*)informationBar cellForRowAtIndex:(NSInteger) rowIndex forItem:(OverlayItem*)item
{
  UITableViewCell *cell = [informationBar dequeueReusableCellWithIdentifier:@"Cell"];
  
  if (cell == nil) {
    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:@"Cell"];
  }
  
  ProductOverlayItem *productItem = (ProductOverlayItem *)item;
  FMProduct *product = [productItem.products objectAtIndex:rowIndex];
  NSDictionary *proxyProduct = [_products objectAtIndex:rowIndex];
  
  NSMutableAttributedString *attributedString = nil;
  
  // Strike through or not
  if ([proxyProduct valueForKey:@"isPickedUp"]) {
    attributedString = [[NSMutableAttributedString alloc] initWithString:product.name];
    [attributedString addAttribute:NSStrikethroughStyleAttributeName value:@2 range:NSMakeRange(0, attributedString.length)];
  } else {
    attributedString = [[NSMutableAttributedString alloc] initWithString:product.name];
  }
  
  cell.textLabel.attributedText = attributedString;
  cell.selectionStyle = UITableViewCellSelectionStyleNone;
  
  return cell;
}

- (NSString *)informationBar:(InformationBar*)informationBar keywordForItem:(OverlayItem*)item
{
  ProductOverlayItem *productItem = (ProductOverlayItem *)item;
  FMProduct *firstProduct = [[productItem products] firstObject];
  
  return [firstProduct name] ?: @"";
}


- (BOOL)informationBar:(InformationBar*)informationBar fixedForItem:(OverlayItem*)item
{
  [item setDisabled:YES]; // Disable items
  return YES;
 
}

- (NSString*)informationBar:(InformationBar*)informationBar collapsedInstructionsForItem:(OverlayItem*)item
{
  return @"";
}

- (NSString*)informationBar:(InformationBar*)informationBar expandedInstructionsForItem:(OverlayItem*)item
{
  return @"";
}

- (NSString*)informationBar:(InformationBar*)informationBar locationForItem:(OverlayItem*)item
{
  ProductOverlayItem *productItem = (ProductOverlayItem *)item;
  FMProduct *product = productItem.products.firstObject;
  FMSection *sectionForOverlay = [[FMSection alloc] init];
  
  for (FMSection *section in product.sections) {
    if (section.maplocation == item.maplocation) {
      sectionForOverlay = section;
    }
  }
  
  if (sectionForOverlay == nil) {
    NSLog(@"[ERROR] Logic-error: sectionForOverlay should be non-nil at this point!");
  }
  
  return sectionForOverlay.aisleTitle;
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
