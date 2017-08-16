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
#import "FMSection.h"

/*!
 @class FMProduct
 @abstract Represents the product mapped to the some location(s) in the store.
 */
@interface FMProduct : NSObject {
	long idn;
	NSArray* sections;
	NSString *name;
	BOOL checked;
}

/*!
 @property name
 @abstract Name of the product
 */
@property (nonatomic, copy) NSString *name;

/*!
 @property idn
 @abstract The product id
 */
@property (assign) long idn;

/*!
 @property sections
 @abstract An array of sections in which this product is available
 */
@property (nonatomic, retain) NSArray* sections;

/*!
 @property checked
 @abstract YES if the product has already been purchased, NO otherwise
 */
@property (assign) BOOL checked;

@end
