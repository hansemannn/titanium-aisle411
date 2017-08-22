/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-Present by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
#import "TiViewProxy.h"
#import <MapSDK/MapSDK.h>

@interface TiAisle411MapViewProxy : TiViewProxy <InformationBarDelegate, InformationBarDataSource> {
  ProductCalloutOverlay *_productCallOutOverlay;
  NSArray<NSDictionary<NSString *, id> *> *_products;
}

- (void)setFloor:(id)floor;

- (void)setLogoPosition:(id)logoPosition;

- (void)setCompassEnabled:(id)compassEnabled;

- (void)deselectAll:(id)unused;

- (void)clear:(id)unused;

- (void)zoomIn:(id)unused;

- (void)zoomOut:(id)unused;

- (void)addOverlay:(id)args;

- (void)redraw:(id)unused;

- (void)redrawOverlay:(id)unused;

- (void)reloadTiles:(id)unused;

- (void)fadeIn:(id)args;

@end
