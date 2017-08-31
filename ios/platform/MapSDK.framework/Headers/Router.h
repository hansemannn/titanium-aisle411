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
#import "CMControlPoint.h"
#import "MapBundle.h"

@class FMShoppingList;
@class GraphContainer;

/*!
 @class Router
 @abstract Router class definition.
 */

@interface Router : NSObject {
	MapBundle *_mapBundle;
	GraphContainer *_graphContainer;
	
	NSMutableArray *_route;	
	CMControlPoint *_startPoint;
	CMControlPoint *_endPoint;
	NSSet *points;
	
	FMShoppingList *shoppingList;
	NSMutableArray *_routedSublocations;
}

/*!
 @method initWithMapBundle:
 @abstract Designated initializer for the Router
 @param mapBundle MapBundle instance
 @result Router object
 */
- (id) initWithMapBundle: (MapBundle *)mapBundle;

/*!
 @property points
 @abstract An array of points which the route should consist of
 */
@property (nonatomic, retain) NSSet *points;

/*!
 @method setStart:
 @abstract Sets the start point of the route
 @param point Start point of the route
 */
- (void) setStart:(CMControlPoint *)point;

/*!
 @method setFinish:
 @abstract Sets the end point of the route
 @param point End point of the route
 */
- (void) setFinish:(CMControlPoint *)point;

/*!
 @method calculate
 @abstract Calculates the route
 */
- (void) calculate;

/*!
 @method getRoute
 @abstract Returns an array of points in the order in which they should be placed on the route
 @result An array of points
 */
- (NSArray *)getRoute;

@end
