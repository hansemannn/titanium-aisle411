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
#import "SVPulsingAnnotationView.h"

/*!
 @class OverlayItem
 @abstract Some visual object on the floor map.
 */
@interface OverlayItem : NSObject {
	NSString *title;	
	UIImageView *imageView;
	CMControlPoint *point;
    
    long sublocation;
	long location;
    long maplocation;
}

/*!
 @property title
 @abstract The title of the on-map object
 */
@property (nonatomic, retain) NSString *title;

/*!
 @property imageView
 @abstract The image for the on-map object
 */
@property (nonatomic, readonly) UIImageView *imageView;

/*!
 @property point
 @abstract Represents the object's location on the floor map
 */
@property (nonatomic, retain) CMControlPoint *point;


/*!
 @property disabled:
 @abstract set this value to YES if you don't want pins to be touchable  
 */
@property (assign) BOOL disabled;

/*!
 @method setImage:
 @abstract set image to image view
 */
- (void) setImage: (UIImage *)img;

/*!
 @property userDot
 @abstract marker to represent user location
 */
@property (nonatomic, retain) SVPulsingAnnotationView *userDot;

@property (nonatomic, assign) long sublocation;
@property (nonatomic, assign) long location;
@property (nonatomic, assign) long maplocation;

@end
