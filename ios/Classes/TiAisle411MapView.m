/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2017 by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#import "TiAisle411MapView.h"
#import "TiAisle411MapViewProxy.h"

#define PUBLIXCOLOR [UIColor colorWithRed:47 / 255 green:136 / 255 blue:15 / 255 alpha:1.0]

@implementation TiAisle411MapView

- (MapController *)mapController
{
  if (_mapController == nil) {
    NSString *url = [[self proxy] valueForKey:@"url"];
    UIImage *selected = [TiUtils toImage:[[self proxy] valueForKey:@"selectedPinImage"] proxy:self.proxy];
    UIImage *unselected = [TiUtils toImage:[[self proxy] valueForKey:@"unselectedPinImage"] proxy:self.proxy];

    _shoppingModeEnabled = [TiUtils boolValue:[[self proxy] valueForKey:@"shoppingListEnabled"] def:NO];

    // Create callout-overlay
    _productCallOutOverlay = [[ProductCalloutOverlay alloc] initWithInformationBarSupport];
    _productCallOutOverlay.delegate = self;
    _productCallOutOverlay.image = unselected;
    _productCallOutOverlay.selectedImage = selected;

    // Configure information-bar
    InformationBar *informationBar = _productCallOutOverlay.informationBar;
    informationBar.backgroundColor = UIColor.whiteColor;
    [informationBar setLocationLabelBackgroundColor:PUBLIXCOLOR];
    informationBar.dataSource = self;
    informationBar.delegate = self;
    [informationBar hideInstructionLine:!_shoppingModeEnabled];
    [informationBar hideInstructionLabel:!_shoppingModeEnabled];

    [[informationBar table] registerClass:[UITableViewCell class] forCellReuseIdentifier:@"Cell"];

    // Create map-controller
    _mapController = [[MapController alloc] init];
    _mapController.mapControllerDelegate = self;

    // Parse map-data
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
      MapBundleParser *parser = [[MapBundleParser alloc] initWithPathToArchive:url];
      MapBundle *mapBundle = [parser parse];

      // Parse bundle
      dispatch_async(dispatch_get_main_queue(), ^{
        _mapController.mapBundle = mapBundle;

        // Size view
        UIView *mapView = [_mapController view];
        mapView.frame = self.bounds;
        mapView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;

        // Set default values
        [_mapController setFloor:1];
        [_mapController setCompassEnabled:NO];

        [_mapController addOverlay:_productCallOutOverlay];

        [self addSubview:mapView];
      });
    });
  }

  return _mapController;
}

- (TiAisle411MapViewProxy *)mapViewProxy
{
  return (TiAisle411MapViewProxy *)[self proxy];
}

- (ProductCalloutOverlay *)overlay
{
  return _productCallOutOverlay;
}

#pragma mark Map Controller Delegate

- (void)mapControllerDidFinishLoading:(MapController *)aMapController
{
  // Fix incorrectly scaled maps
  if (aMapController.mapRotation != 0) {
    [aMapController resetMapRotation];
  }
  
  if ([[self proxy] _hasListeners:@"load"]) {
    [[self proxy] fireEvent:@"load"];
  }
}

#pragma mark Information Bar Data Source / Delegate

- (void)informationBar:(InformationBar *)informationBar didSelectRowAtIndex:(NSInteger)rowIndex forItem:(OverlayItem *)item
{
  ProductOverlayItem *spItem = (ProductOverlayItem *)item;
  FMProduct *selectedProduct = spItem.products[rowIndex];

  NSDictionary *publixProduct = nil;
  
  for (NSDictionary *product in [[self mapViewProxy] products]) {
    if ([TiUtils intValue:[product valueForKey:@"id"]] == selectedProduct.idn) {
      publixProduct = product;
      break;
    }
  }

  if (publixProduct == nil) {
    return;
  }

  BOOL isPickedUp = [TiUtils boolValue:[publixProduct valueForKey:@"isPickedUp"] def:NO];
  
  [publixProduct setValue:@(!isPickedUp) forKey:@"isPickedUp"];
  selectedProduct.checked = !isPickedUp;

  UITableViewCell *cell = informationBar.table.visibleCells[rowIndex];
  cell.textLabel.attributedText = [TiAisle411MapView cellTitleForProduct:selectedProduct andDictionary:publixProduct];

  if ([[self proxy] _hasListeners:@"informationBarItemClick"]) {
    [[self proxy] fireEvent:@"informationBarItemClick" withObject:@{ @"product": publixProduct }];
  }
}

- (NSInteger)informationBar:(InformationBar *)informationBar numberOfRowsForItem:(OverlayItem *)item
{
  if (_shoppingModeEnabled == NO) {
    return 0;
  }

  return [[(ProductOverlayItem *)item products] count];
}

- (UITableViewCell *)informationBar:(InformationBar *)informationBar cellForRowAtIndex:(NSInteger)rowIndex forItem:(OverlayItem *)item
{
  if (_shoppingModeEnabled == NO) {
    return nil;
  }

  UITableViewCell *cell = [informationBar dequeueReusableCellWithIdentifier:@"Cell"];

  if (cell == nil) {
    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:@"Cell"];
  }

  ProductOverlayItem *productItem = (ProductOverlayItem *)item;
  FMProduct *product = [productItem.products objectAtIndex:rowIndex];
  NSDictionary *proxyProduct = [[[self mapViewProxy] products] objectAtIndex:rowIndex];

  cell.textLabel.attributedText = [TiAisle411MapView cellTitleForProduct:product andDictionary:proxyProduct];
  cell.selectionStyle = UITableViewCellSelectionStyleNone;

  return cell;
}

- (NSString *)informationBar:(InformationBar *)informationBar keywordForItem:(OverlayItem *)item
{
  if (_shoppingModeEnabled == NO) {
    ProductOverlayItem *productItem = (ProductOverlayItem *)item;
    FMProduct *firstProduct = [[productItem products] firstObject];

    return [firstProduct name] ?: @"";
  }

  ProductOverlayItem *spItem = (ProductOverlayItem *)item;
  NSInteger itemCount = spItem.products.count;

  switch (itemCount) {
  case 1: {
    FMProduct *onlyProduct = [spItem.products objectAtIndex:0];
    _keywordText = onlyProduct.name;
  } break;
  case 2: {
    FMProduct *firstProduct = [spItem.products objectAtIndex:0];
    FMProduct *secondProduct = [spItem.products objectAtIndex:1];
    _keywordText = [NSString stringWithFormat:@"%@ and %@ ", firstProduct.name, secondProduct.name];
  } break;
  default: {
    FMProduct *firstProduct = [spItem.products objectAtIndex:0];
    _keywordText = [NSString stringWithFormat:@"%@ and %li other items", firstProduct.name, itemCount - 1];
  } break;
  }

  return _keywordText;
}

- (void)informationBarDidShowTable:(InformationBar *)informationBar
{
  [informationBar setKeyWordLabelText:@"Items in this section"];
}

- (void)informationBarDidHideTable:(InformationBar *)informationBar
{
  [informationBar setKeyWordLabelText:_keywordText];
}

- (BOOL)informationBar:(InformationBar *)informationBar fixedForItem:(OverlayItem *)item
{
  if (_shoppingModeEnabled == NO) {
    return YES;
  } else {
    return NO;
  }
}

- (NSString *)informationBar:(InformationBar *)informationBar collapsedInstructionsForItem:(OverlayItem *)item
{
  if (_shoppingModeEnabled == NO) {
    return @"";
  } else {
    return @"Slide up for items in this section";
  }
}

- (NSString *)informationBar:(InformationBar *)informationBar expandedInstructionsForItem:(OverlayItem *)item
{
  if (_shoppingModeEnabled == NO) {
    return @"";
  } else {
    return @"Slide down to close";
  }
}

- (NSString *)informationBar:(InformationBar *)informationBar locationForItem:(OverlayItem *)item
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

- (BOOL)calloutOverlay:(CalloutOverlay *)overlay shouldZoom:(OverlayItem *)item
{
  return [TiUtils boolValue:[[self proxy] valueForKey:@"zoomOnPinClickEnabled"] def:NO];
}

#pragma mark Callout Overlay Delegate

- (void)calloutOverlay:(CalloutOverlay *)overlay didItemSelected:(OverlayItem *)item
{
  UIImage *selected = [TiUtils toImage:[[self proxy] valueForKey:@"selectedPinImage"] proxy:self.proxy];
  [item setImage:selected];
  [_mapController setPosition:item animated:YES];

  if ([[self proxy] _hasListeners:@"didItemSelected"]) {
    [[self proxy] fireEvent:@"didItemSelected" withObject:@{ @"item" : item.title }];
  }
}

- (void)calloutOverlay:(CalloutOverlay *)overlay didItemDeselected:(OverlayItem *)item
{
  UIImage *unselected = [TiUtils toImage:[[self proxy] valueForKey:@"unselectedPinImage"] proxy:self.proxy];
  [item setImage:unselected];

  if ([[self proxy] _hasListeners:@"didItemDeselected"]) {
    [[self proxy] fireEvent:@"didItemDeselected" withObject:@{ @"item" : item.title }];
  }
}

- (void)calloutOverlay:(CalloutOverlay *)overlay didItemReselected:(OverlayItem *)oldItem withItem:(OverlayItem *)item
{
  UIImage *selected = [TiUtils toImage:[[self proxy] valueForKey:@"selectedPinImage"] proxy:self.proxy];
  UIImage *unselected = [TiUtils toImage:[[self proxy] valueForKey:@"unselectedPinImage"] proxy:self.proxy];

  [oldItem setImage:unselected];
  [item setImage:selected];
  [_mapController setPosition:item animated:YES];

  if ([[self proxy] _hasListeners:@"didItemReselected"]) {
    [[self proxy] fireEvent:@"didItemReselected" withObject:@{ @"item" : item.title }];
  }
}

#pragma mark Layout Helper

+ (NSAttributedString *)cellTitleForProduct:(FMProduct *)product andDictionary:(NSDictionary *)proxyProduct
{
  // Strike through or not
  if ([TiUtils boolValue:[proxyProduct valueForKey:@"isPickedUp"] def:NO]) {
    NSMutableAttributedString *attributedString = [[NSMutableAttributedString alloc] initWithString:product.name];
    [attributedString addAttribute:NSBaselineOffsetAttributeName value:@0 range:NSMakeRange(0, attributedString.length)];
    [attributedString addAttribute:NSStrikethroughStyleAttributeName value:@2 range:NSMakeRange(0, attributedString.length)];
    return attributedString;
  } else {
    return [[NSMutableAttributedString alloc] initWithString:product.name];
  }
}

- (void)updateContentMode
{
  if (self != nil) {
    [self setContentMode:[self contentModeForMapView]];
  }
}

- (UIViewContentMode)contentModeForMapView
{
  if (TiDimensionIsAuto(_width)
      || TiDimensionIsAutoSize(_width)
      || TiDimensionIsUndefined(_width)
      || TiDimensionIsAuto(_height)
      || TiDimensionIsAutoSize(_height)
      || TiDimensionIsUndefined(_height)) {
    return UIViewContentModeScaleAspectFit;
  } else {
    return UIViewContentModeScaleToFill;
  }
}

- (void)frameSizeChanged:(CGRect)frame bounds:(CGRect)bounds
{
  for (UIView *child in [[[self mapController] view] subviews]) {
    [TiUtils setView:child positionRect:bounds];
  }

  [super frameSizeChanged:frame bounds:bounds];
}

- (CGFloat)contentWidthForWidth:(CGFloat)suggestedWidth
{
  if (_autoWidth > 0) {
    //If height is DIP returned a scaled autowidth to maintain aspect ratio
    if (TiDimensionIsDip(_height) && _autoHeight > 0) {
      return roundf(_autoWidth * _height.value / _autoHeight);
    }
    return _autoWidth;
  }

  CGFloat calculatedWidth = TiDimensionCalculateValue(_width, _autoWidth);
  if (calculatedWidth > 0) {
    return calculatedWidth;
  }

  return 0;
}

- (CGFloat)contentHeightForWidth:(CGFloat)width_
{
  if (width_ != _autoWidth && _autoWidth > 0 && _autoHeight > 0) {
    return (width_ * _autoHeight / _autoWidth);
  }

  if (_autoHeight > 0) {
    return _autoHeight;
  }

  CGFloat calculatedHeight = TiDimensionCalculateValue(_height, _autoHeight);
  if (calculatedHeight > 0) {
    return calculatedHeight;
  }

  return 0;
}

- (UIViewContentMode)contentMode
{
  if (TiDimensionIsAuto(_width)
      || TiDimensionIsAutoSize(_width)
      || TiDimensionIsUndefined(_width)
      || TiDimensionIsAuto(_height)
      || TiDimensionIsAutoSize(_height)
      || TiDimensionIsUndefined(_height)) {
    return UIViewContentModeScaleAspectFit;
  } else {
    return UIViewContentModeScaleToFill;
  }
}

@end
