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

@class OverlayItem;

/*!
 @protocol ProductCalloutOverlayDelegate
 @abstract The data source for ProductCalloutOverlay.
 */
@protocol ProductCalloutOverlayDelegate<NSObject>
@optional

/*!
 @method titleForItem:
 @abstract Should return the callout's title
 @param item Overlay item object
 */
- (NSString*)titleForItem:(OverlayItem*)item;

/*!
 @method optionCheckedStateAtIndex:forItem:
 @abstract Should return YES if the overlay's item at index 'ind' should be checked, NO - otherwise
 @param ind Index of the item
 @param item The overlay item instance
 */
- (BOOL)optionCheckedStateAtIndex:(NSInteger)ind forItem:(OverlayItem*)item;

/*!
 @method checkedOptionAtIndex:forItem:
 @abstract Called when the overlay item is checked
 @param ind Index of the item
 @param item The overlay item instance
 */
- (void)checkedOptionAtIndex:(NSUInteger)ind forItem: (OverlayItem *)item;

/*!
 @method uncheckedOptionAtIndex:forItem:
 @abstract Called when the overlay item is unchecked
 @param ind Index of the item
 @param item The overlay item instance
 */
- (void)uncheckedOptionAtIndex:(NSUInteger)ind forItem: (OverlayItem *)item;

/*!
 @method chevronClickedForItem:
 @abstract Chevron button action. Do not use this methods directly, use ProductCalloutOverlayDelegate methods.
 */
- (void)chevronClickedForItem:(OverlayItem *)item;

@end
