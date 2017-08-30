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
#import "FMProduct.h"

typedef enum {
    kSponsoredPinLink = 0,
    kSponsoredPinProduct
} SponsoredPinType;

typedef enum {
    kDrawingPositionCenter = 0,
    kDrawingPositionLeft,
    kDrawingPositionRight
} DrawingPosition;

/*!
 @class ProductCalloutOverlay
 @abstract Displays sponsored pin item
 */
@interface OverlaySponsoredItem : OverlayItem {
    NSArray *products;
    NSString *url;
    UIImage *calloutImage;
    UIImage *pinCircleImage;
    SponsoredPinType type;
    DrawingPosition drawingPosition;
    NSString *webViewTitle;
    BOOL isCoupon;
    int floor;
}

/*!
 @property products
 @abstract Array with FMFeaturedProducts
 */
@property(nonatomic, retain) NSArray *products;

/*!
 @property url
 @abstract The url that should be opened on chevron click
 */
@property(nonatomic, retain) NSString *url;

/*!
 @property calloutImage
 @abstract Image inside callout
 */
@property(nonatomic, retain) UIImage *calloutImage;

/*!
 @property calloutImage
 @abstract Circle image inside pin
 */
@property(nonatomic, retain) UIImage *pinCircleImage;

/*!
 @property webViewTitle
 @abstract Title for web view that should be opened on chevron click
 */
@property(nonatomic, retain) NSString *webViewTitle;

/*!
 @property type
 @abstract Type of sponsored pin
 */
@property(assign) SponsoredPinType type;

/*!
 @property drawingPosition
 @abstract Position for callout (angled, not angled)
 */
@property(assign) DrawingPosition drawingPosition;

/*!
 @property isCoupon
 @abstract If YES, default circle pin image should be default coupon image
 */
@property(assign) BOOL isCoupon;

/*!
 @property floor
 @abstract Floor of the pin
 */
@property(assign) int floor;

@end
