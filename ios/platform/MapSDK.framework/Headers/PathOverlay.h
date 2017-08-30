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
#import "Overlay.h"

/*!
 @class PathOverlay
 @abstract Draws the specified route on the map.
 */
@interface PathOverlay : Overlay {
	NSArray *route;
}

/*!
 @property route
 @abstract Array of CMPoints
 */
@property (nonatomic, retain) NSArray *route;

@end
