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

#import "FMProduct.h"

/*!
 @class FMFeaturedProduct 
 @abstract Represents the product in sponsored pin
 */
@interface FMFeaturedProduct : FMProduct {
    NSString *upc;
    NSString *description;
    NSString *brandName;
}

/*!
 @property upc
 @abstract The product's upc
 */
@property(nonatomic, copy) NSString *upc;

/*!
 @property decription
 @abstract The product's description
 */
@property(nonatomic, copy) NSString *description;

/*!
 @property brandName
 @abstract The product's brand name
 */
@property(nonatomic, copy) NSString *brandName;

@end
