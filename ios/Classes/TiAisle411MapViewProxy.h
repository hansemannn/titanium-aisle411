/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-Present by Axway Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
#import "TiViewProxy.h"

@interface TiAisle411MapViewProxy : TiViewProxy {
}

@property (nonatomic, strong) NSArray<NSDictionary<NSString *, id> *> *products;

- (void)setFloor:(id)floor;

- (void)setZoomButtonsHidden:(id)floor;

- (void)setLogoPosition:(id)logoPosition;

- (void)setCompassEnabled:(id)compassEnabled;

- (void)setMapBackgroundColor:(id)backgroundColor;

- (void)setKeyWordLabelText:(id)keyWordLabelText;

- (void)deselectAll:(id)unused;

- (void)clear:(id)unused;

- (void)zoomIn:(id)unused;

- (void)zoomOut:(id)unused;

- (void)redraw:(id)unused;

- (void)redrawOverlay:(id)unused;

- (void)reloadTiles:(id)unused;

- (void)fadeIn:(id)args;

- (void)search:(id)args;

@end
