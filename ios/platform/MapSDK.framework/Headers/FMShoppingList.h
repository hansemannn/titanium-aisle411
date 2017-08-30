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
#import "FMProduct.h"

/*!
 @class FMShoppingList
 @abstract Represents the shopping list with several products.
 */
@interface FMShoppingList : NSObject {
	NSMutableArray *_items;
}

/*!
 @property products
 @abstract An array of the FMProducts
 */
@property (nonatomic, readonly) NSArray *products;


/*!
 @method addProduct:
 @abstract Adds product to the shopping list
 @param product The product to be added
 */
- (void) addProduct: (FMProduct *)product;

/*!
 @method buyProduct:
 @abstract Purchase the product: set the "checked" value of the product to YES
 @param product The product to be added
 */
- (void) buyProduct: (FMProduct *)product;

/*!
 @method returnProduct:
 @abstract Set the "checked" value of the product to NO
 @param product The product instance
 */
- (void) returnProduct: (FMProduct *)product;

/*!
 @method isProductBought:
 @abstract Returns YES if the product has already been purchased, NO - otherwise
 @param product The product instance
 @result Boolean value indicating whether a product has been purchased
 */
- (BOOL) isProductBought: (FMProduct *)product;

/*!
 @method getProductsToBuy:
 @abstract Returns the list of products that have not yet been purchased
 @result An array of FMProduct objects
 */
- (NSArray *)getProductsToBuy;

@end