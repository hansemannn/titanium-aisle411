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
#import "CalloutOptionOverlay.h"
#import "ProductCalloutOverlayDelegate.h"
#import "ProductOverlayItem.h"
#import "InformationBar.h"

#import "FMProduct.h"
#import "FMSponsoredPin.h"

/*!
 @class ProductCalloutOverlay
 @abstract Draws the pins and callouts on the floor map mapped to the FMProduct.
 */
@interface ProductCalloutOverlay : CalloutOptionOverlay {
	NSArray *products;
    NSMutableArray *ads;
	NSArray *selectedProducts;
	UIImage *checkedPinImage;
	id<ProductCalloutOverlayDelegate> productCalloutDelegate;
	
	BOOL shouldHideCalloutOnTimeout;
}

/*!
 @abstract Method should be used to initialize overlay for information bar usage
 */
-(instancetype) initWithInformationBarSupport;

/*!
 @property informationBar
 @abstract information bar
 */
@property (nonatomic, readonly) InformationBar *informationBar;

/*!
 @property hidePinsForBoughtProducts:
 @abstract hides pins if Yes in case pin has only bought products  
 */
@property (nonatomic, assign) BOOL hidePinsForBoughtProducts;

/*!
 @property products
 @abstract array of CMPoint-s
 */
@property (nonatomic, retain) NSArray *products;

/*!
 @property selectedProducts
 @abstract Selected products array
 */
@property (nonatomic, retain) NSArray *selectedProducts;

/*!
 @property selectedSections
 @abstract Selected sections array
 */
@property (nonatomic, retain) NSArray * selectedSections;

/*!
 @property sponsoredCalloutDisplayTime
 @abstract Sponsored callout display time. The default value is 2 seconds.
 */
@property (nonatomic, assign) NSTimeInterval sponsoredCalloutDisplayTime;


/*!
 @property productCalloutDelegate:
 @abstract this objects should return the callout properties such as callout title, product name, their state, etc. this object also receives the callout actions
 */
@property (nonatomic, assign) id<ProductCalloutOverlayDelegate> productCalloutDelegate;


/*!
 @property updatePinImages:
 @abstract updates images for evert overlay item according it state
 */
- (void)updatePinImages;

/*!
 @property addAd:
 @abstract add pin with advertising information to overlay
 @param ad pin with advertising
 */
- (void)addAd:(FMSponsoredPin*)ad;

@end
