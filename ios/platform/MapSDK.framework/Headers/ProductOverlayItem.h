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
#import "FMProduct.h"

typedef enum {
	POIStateInitial,
	POIStateSelected,
	POIStateChecked
} ProductOverlayItemState;

/*!
 @class ProductOverlayItem
 @abstract the object on the floor map represents the product location.
 */
@interface ProductOverlayItem : OverlayItem {
	NSMutableArray *products;
	ProductOverlayItemState state;
}

/*!
 @property products
 @abstract Array of FMProducts displayed on this overlay
 */

@property (nonatomic, retain) NSMutableArray *products;

/*!
 @property state
 @abstract state of product overlay
 */
@property (assign) ProductOverlayItemState state;
@end
