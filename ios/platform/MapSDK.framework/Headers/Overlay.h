/***********************************************************************
 *
 * aisle411
 * Copyright (C) by aisle411
 * http://www.aisle411.com
 *
 * Developed by Mercury Development, LLC
 * http://www.mercdev.com
 *
 ***********************************************************************/

#import <Foundation/Foundation.h>

#import "OverlayItem.h"

@class MapView;

/*!
 @class Overlay
 @abstract Base class for all overlays.
 */
@interface Overlay : NSObject

@property (nonatomic, assign) MapView *mapView;

@end
