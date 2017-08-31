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

#import "Overlay.h"
#import "OverlayItem.h"

/*!
 @class UserLocationOverlay
 @abstract UserLocationOverlay is used for displaying user position snapped to the route with a given threshold.
 */
@interface UserLocationOverlay : Overlay
{
    OverlayItem *item;
}

/*!
 @property item
 @abstract OverlayItem used for displaying as user position. Use point property of OverlayItem to specify user coordinates; use setImage: method of OverlayItem instance to specify user location image.
 */
@property (nonatomic, retain) OverlayItem *item;

/*!
 @property angle
 @abstract Can be used to rotate OverlayItem image by this angle in radians.
 */
@property (nonatomic, assign) CGFloat angle;

/*!
 @property threshold
 @abstract Threshold is used for snapping the user location point to a route grid of the map. Must be specified in relative coordinates of map from 0.0 to 1.0, where 1.0 is the whole map width/height. If set to 0, threshold is turn off.
 */
@property (nonatomic, assign) CGFloat threshold;

/*!
 @method configureScale
 @abstract Set default overlay radius
 */
- (void)configureScale;

/*!
 @method changeRadius:
 @abstract Changes overlay radius
 @param newRadius new radius value
 */
- (void)changeRadius:(double)newRadius;
@end
