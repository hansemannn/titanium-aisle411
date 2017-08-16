/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2017 by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
#import "TiUIView.h"
#import <MapSDK/MapController.h>

@interface TiAisle411MapView : TiUIView {
@private
  TiDimension width;
  TiDimension height;
  CGFloat autoHeight;
  CGFloat autoWidth;

}

@property(nonatomic, strong) MapController *mapController;

@end
