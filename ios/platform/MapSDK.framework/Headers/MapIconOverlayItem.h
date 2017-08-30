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

#import "OverlayItem.h"

/*!
 @class ZOOM
 @abstract class to represent zoom levels range
 */
@interface ZOOM: NSObject {
    CGFloat start;
    CGFloat end;
}

+ (ZOOM*) zoomWithStart:(CGFloat)start end:(CGFloat)end;

@property(nonatomic, assign) CGFloat start;
@property(nonatomic, assign) CGFloat end;

@end


/*!
 @class MapIconOverlayItem
 @abstract represents icon over map that can change visibility depending on zoom level
 */
@interface MapIconOverlayItem : OverlayItem {
    NSArray *zoomLevels;
}

/*!
 @property zoomLevels
 @abstract array of ZOOM objects. Overlay is visble for every zoom level in this array
 */
@property (nonatomic, retain) NSArray *zoomLevels;

/*!
 @method fadeIn
 @abstract show overlay with animation
 */
- (void)fadeIn;

/*!
 @method fadeIn
 @abstract hide overlay with animation
 */
- (void)fadeOut;

/*!
 @method visibleAtZoomLevel:
 @abstract checks visibility for zoom level
 @param zoomLevel zoom level to check
 */
- (BOOL)visibleAtZoomLevel:(CGFloat)zoomLevel;

@end
