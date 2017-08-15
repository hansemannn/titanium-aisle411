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
#import "PathOverlay.h"
#import "ProductCalloutOverlay.h"

#import "FMShoppingList.h"

/*!
 @class ShoppingListRouteOverlay
 @abstract Displays shopping list route with product pins.
 */
@interface ShoppingListRouteOverlay : ProductCalloutOverlay {
	PathOverlay *_pathOverlay;
	FMShoppingList *shoppingList;
	NSArray *_routedSublocations;
	BOOL _mapValidated;
	BOOL showRoute;
}

/*!
 @abstract Method should be used to initialize overlay for information bar usage
 */
-(instancetype) initWithInformationBarSupport;

/*!
 @property shoppingList:
 @abstract The FMShoppingList instance to be displayed
 */
@property (nonatomic, retain) FMShoppingList *shoppingList;

/*!
 @property showRoute:
 @abstract set this value to YES if you want the ShoppingListRouteOverlay to draw optimal route, NO - otherwise
 */
@property (assign) BOOL showRoute;

@end
