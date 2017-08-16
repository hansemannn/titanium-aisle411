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

#import "CMControlPoint.h"

/*!
 @class DirectionOverlay
 @abstract Draws the arrow that points to the specified control point.
 */
@interface DirectionOverlay : Overlay {
	CMControlPoint *point;
	UIImageView *_imageView;
}


/*!
 @property point
 @abstract The control point which the arrow should point to
 */
@property (nonatomic, retain) CMControlPoint *point;

/*!
 @property image
 @abstract The arrow image
 */
@property (nonatomic, retain) UIImage *image;

@end
