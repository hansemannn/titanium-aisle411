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

@interface ZOOM:NSObject {
    CGFloat start;
    CGFloat end;
}

+ (ZOOM*) zoomWithStart:(CGFloat)start end:(CGFloat)end;

@property(nonatomic, assign) CGFloat start;
@property(nonatomic, assign) CGFloat end;

@end



@interface MapIconOverlayItem : OverlayItem {
    NSArray *zoomLevels;
}

@property (nonatomic, retain) NSArray *zoomLevels;

- (void)fadeIn;
- (void)fadeOut;
- (BOOL)visibleAtZoomLevel:(CGFloat)zoomLevel;

@end
