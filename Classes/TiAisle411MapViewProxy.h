/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2017 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
#import "TiViewProxy.h"
#import <MapSDK/MapSDK.h>

@interface TiAisle411MapViewProxy : TiViewProxy <InformationBarDelegate, InformationBarDataSource> {

}

- (void)setFloor:(id)floor;

- (void)setLogoPosition:(id)logoPosition;

- (void)setCompassEnabled:(id)compassEnabled;

- (void)deselectAll:(id)unused;

- (void)clear:(id)unused;

- (void)zoomIn:(id)unused;

- (void)zoomOut:(id)unused;

- (void)redraw:(id)args;

@end
