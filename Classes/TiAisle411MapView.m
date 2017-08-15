/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2017 by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#import "TiAisle411MapView.h"

@implementation TiAisle411MapView

- (UIView *)mapView
{
  if (_mapView == nil) {
    MapController *mapController = [[MapController alloc] init];
    
    _mapView = [mapController view];
    _mapView.frame = self.bounds;
    _mapView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    
    [self addSubview:_mapView];
  }
  
  return _mapView;
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
  for (UIView *child in [[self mapView] subviews]) {
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
