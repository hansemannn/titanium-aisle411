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
 @class FMSponsoredPin
 @abstract Represents the sponsored ad pin 
 */
@interface FMSponsoredPin : NSObject {
    FMSection *section;
    NSString *name;
    NSString *url;
    NSString *webViewTitle;
    UIImage *calloutImage;
    UIImage *pinCircleImage;
    NSArray *products;
    BOOL isCoupon;
}

/*!
 @property section
 @abstract Pin section
 */
@property(nonatomic, retain) FMSection *section;

/*!
 @property name
 @abstract Pin name
 */
@property(nonatomic, copy) NSString *name;

/*!
 @property url
 @abstract Ad url that should be opened on chevron click
 */
@property(nonatomic, copy) NSString *url;

/*!
 @property webViewTitle
 @abstract The title of web view that should be opened on chevron click
 */
@property(nonatomic, copy) NSString *webViewTitle;

/*!
 @property image
 @abstract Image in the callout
 */
@property(nonatomic, retain) UIImage *calloutImage;

/*!
 @property calloutImage
 @abstract Circle image inside pin
 */
@property(nonatomic, retain) UIImage *pinCircleImage;

/*!
 @property products
 @abstract Array with FMFeaturedProducts
 */
@property(nonatomic, retain) NSArray *products;

/*!
 @property isCoupon
 @abstract YES if circle inside pin should show default coupon image, NO if circle inside pin should show a different image
 */
@property(nonatomic, assign) BOOL isCoupon;

@end
