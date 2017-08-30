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

@class GraphContainer;
@class CMControlPoint;
/*!
 @class CustomPathOverlay
 @abstract Custom Path Overlay class definition - displays the point-to-point route.
 */

@interface CustomPathOverlay : Overlay {
	GraphContainer* _graphContainer;
	
	CMControlPoint*	startRoutePoint;
	CMControlPoint*	endRoutePoint;
	
	BOOL enabled;
}

/*!
 @property startRoutePoint:
 @abstract The first control point of the point-to-point route
 */
@property (nonatomic, assign) CMControlPoint* startRoutePoint;

/*!
 @property endRoutePoint:
 @abstract The last control point of the point-to-point route
 */
@property (nonatomic, assign) CMControlPoint* endRoutePoint;

/*!
 @property enabled:
 @abstract Set this value to NO if you don't want this overlay to receive touch events and draw routes, YES - otherwise
 */
@property (assign) BOOL enabled;

@end