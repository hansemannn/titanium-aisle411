/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2017 by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#import "TiAisle411MapView.h"
#import "TiAisle411MapViewProxy.h"

#define PUBLIXCOLOR [UIColor colorWithRed:47/255 green:136/255 blue:15/255 alpha:1.0]

@implementation TiAisle411MapView

- (MapController *)mapController
{
  if (_mapController == nil) {
    NSString *url = [[self proxy] valueForKey:@"url"];
    BOOL shoppingModeEnabled = [TiUtils boolValue:[[self proxy] valueForKey:@"shoppingModeEnabled"] def:NO];
    UIImage *selected = [TiUtils toImage:[[self proxy] valueForKey:@"selectedPinImage"] proxy:self.proxy];
    UIImage *unselected = [TiUtils toImage:[[self proxy] valueForKey:@"unselectedPinImage"] proxy:self.proxy];
    
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
    [informationBar hideInstructionLine:!shoppingModeEnabled];
    [informationBar hideInstructionLabel:!shoppingModeEnabled];
    
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
  if ([[self proxy] _hasListeners:@"load"]) {
    [[self proxy] fireEvent:@"load"];
  }
}

#pragma mark Information Bar Data Source / Delegate

- (void)informationBar:(InformationBar *)informationBar didSelectRowAtIndex:(NSInteger)rowIndex forItem:(OverlayItem *)item
{
  if ([[self proxy] _hasListeners:@"informationBarItemClick"]) {
    [[self proxy] fireEvent:@"informationBarItemClick" withObject:@{@"title": item.title}]; // Return more if desired
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
  NSDictionary *proxyProduct = [[[self mapViewProxy] products] objectAtIndex:rowIndex];
  
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

#pragma mark Callout Overlay Delegate

- (void)calloutOverlay:(CalloutOverlay*)overlay didItemSelected:(OverlayItem*)item
{
  UIImage *selected = [TiUtils toImage:[[self proxy] valueForKey:@"selectedPinImage"] proxy:self.proxy];
  [item setImage:selected];
  
  if ([[self proxy] _hasListeners:@"didItemSelected"]) {
    [[self proxy] fireEvent:@"didItemSelected" withObject:@{@"item": item.title}];
  }
}

- (void)calloutOverlay:(CalloutOverlay*)overlay didItemReselected:(OverlayItem*)oldItem withItem:(OverlayItem*)item
{
  UIImage *selected = [TiUtils toImage:[[self proxy] valueForKey:@"selectedPinImage"] proxy:self.proxy];
  UIImage *unselected = [TiUtils toImage:[[self proxy] valueForKey:@"unselectedPinImage"] proxy:self.proxy];

  [oldItem setImage:unselected];
  [item setImage:selected];

  if ([[self proxy] _hasListeners:@"didItemReselected"]) {
    [[self proxy] fireEvent:@"didItemReselected" withObject:@{@"item": item.title}];
  }
}

- (void)calloutOverlay:(CalloutOverlay*)overlay didItemDeselected:(OverlayItem*)item
{
  UIImage *unselected = [TiUtils toImage:[[self proxy] valueForKey:@"unselectedPinImage"] proxy:self.proxy];

  if ([[self proxy] _hasListeners:@"didItemDeselected"]) {
    [[self proxy] fireEvent:@"didItemDeselected" withObject:@{@"item": item.title}];
  }
}

- (BOOL)calloutOverlay:(CalloutOverlay*)overlay shouldZoom:(OverlayItem*)item
{
  return NO;
}

#pragma mark Layout Helper

- (void)updateContentMode
{
  if (self != nil) {
    [self setContentMode:[self contentModeForMapView]];
  }
}

- (UIViewContentMode)contentModeForMapView
{
  if (TiDimensionIsAuto(width) || TiDimensionIsAutoSize(width) || TiDimensionIsUndefined(width) || TiDimensionIsAuto(height) || TiDimensionIsAutoSize(height) || TiDimensionIsUndefined(height)) {
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
  if (autoWidth > 0) {
    //If height is DIP returned a scaled autowidth to maintain aspect ratio
    if (TiDimensionIsDip(height) && autoHeight > 0) {
      return roundf(autoWidth * height.value / autoHeight);
    }
    return autoWidth;
  }
  
  CGFloat calculatedWidth = TiDimensionCalculateValue(width, autoWidth);
  if (calculatedWidth > 0) {
    return calculatedWidth;
  }
  
  return 0;
}

- (CGFloat)contentHeightForWidth:(CGFloat)width_
{
  if (width_ != autoWidth && autoWidth > 0 && autoHeight > 0) {
    return (width_ * autoHeight / autoWidth);
  }
  
  if (autoHeight > 0) {
    return autoHeight;
  }
  
  CGFloat calculatedHeight = TiDimensionCalculateValue(height, autoHeight);
  if (calculatedHeight > 0) {
    return calculatedHeight;
  }
  
  return 0;
}

- (UIViewContentMode)contentMode
{
  if (TiDimensionIsAuto(width) || TiDimensionIsAutoSize(width) || TiDimensionIsUndefined(width) || TiDimensionIsAuto(height) || TiDimensionIsAutoSize(height) || TiDimensionIsUndefined(height)) {
    return UIViewContentModeScaleAspectFit;
  } else {
    return UIViewContentModeScaleToFill;
  }
}

@end
