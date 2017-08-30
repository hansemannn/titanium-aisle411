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
#import "CalloutOverlayDelegate.h"

@class TiledView;

/*!
 @class CalloutOverlay
 @abstract Callout overlay class definition.
 */

@interface CalloutOverlay : Overlay

/*!
 @method itemCountAndCurrentFloorNumber
 @abstract returns mapping with count POI per level
 */
- (NSMutableArray *)itemCountAndCurrentFloorNumber;

/*!
 @method itemAtIndex:
 @abstract Returns visual object at specified index
 @param index Index of an object
 @result Corresponding OverlayItem object
 */
- (OverlayItem*)itemAtIndex:(NSUInteger)index;

/*!
 @method addItem:
 @abstract Adds the item onto the map
 @param item Visual object
 */
- (void)addItem:(OverlayItem*)item;

/*!
 @method removeItem:
 @abstract Removes the item from the map
 @param item Visual object
 */
- (void)removeItem:(OverlayItem*)item;

/*!
 @method clearItems:
 @abstract Removes all the items from the map
 */
- (void) clearItems;

/*!
 @property delegate
 @abstract the object supports CalloutOverlayDelegate protocol
 */
@property (nonatomic, assign) id<CalloutOverlayDelegate> delegate;


/*!
 @property showChevronInCallout
 @abstract YES if chevron should be shown in callouts, NO otherwise
 */
@property (nonatomic, assign) BOOL showChevronInCallout;

/*!
 @property isCalloutFullyClickable [FOR REVIEW]
 @abstract YES if you can touch any place of callout to click chevron, NO otherwise
 */
@property (nonatomic, assign) BOOL isCalloutFullyClickable;

// array of overlay items
/*!
 @property items
 @abstract Array of visual objects displayed on the map
 */
@property (nonatomic, retain) NSMutableArray *items;

/*!
 @property image
 @abstract Image for default object on the map
 */
@property (nonatomic, retain) UIImage *image;

/*!
 @property selectedImage
 @abstract Image for currently selected object on the map
 */
@property (nonatomic, retain) UIImage *selectedImage;

/*!
 @property selectedItem
 @abstract Currently selected item on the overlay
 */
@property (nonatomic, retain) OverlayItem *selectedItem;

@end
