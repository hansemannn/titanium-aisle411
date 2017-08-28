/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2017 by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
#import "TiUIView.h"
#import <MapSDK/MapSDK.h>

@interface TiAisle411MapView : TiUIView <MapControllerDelegate, InformationBarDelegate, InformationBarDataSource, CalloutOverlayDelegate> {
  @private
  TiDimension _width;
  TiDimension _height;
  CGFloat _autoHeight;
  CGFloat _autoWidth;

  ProductCalloutOverlay *_productCallOutOverlay;
  NSString *_keywordText;
}

- (ProductCalloutOverlay *)overlay;

@property (nonatomic, strong) MapController *mapController;
@property (nonatomic, assign) BOOL shoppingModeEnabled;

@end
