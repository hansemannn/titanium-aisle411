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
#import "CalloutOverlay.h"

/*!
 @class CalloutOptionOverlay
 @abstract CalloutOptionOverlay class definition.
 */
@interface CalloutOptionOverlay : CalloutOverlay {
	BOOL optionsCheckable;
}

/*!
 @property optionsCheckable
 @abstract Set this property to YES if you want the callout items to be checkable. 
 Default value is YES.
 */
@property (nonatomic, assign) BOOL optionsCheckable; // defaults to YES


@end
