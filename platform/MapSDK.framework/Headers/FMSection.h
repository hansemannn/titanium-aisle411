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



/*!
 @class FMSection
 @abstract represents the section in the store.
 */
@interface FMSection : NSObject {
	long sublocation;
	long location;
    long maplocation;
	NSString *title;
    NSString *aisleTitle;
}

/*!
 @property title
 @abstract Section title.
 */
@property (nonatomic, copy) NSString *title;

/*!
 @property aisleTitle
 @abstract Aisle title.
 */
@property (nonatomic, copy) NSString *aisleTitle;

/*!
 @property sublocation
 @abstract Sublocation id.
*/
@property (nonatomic) long sublocation;

/*!
 @property location
 @abstract location id.
 */
@property (nonatomic) long location;

/*!
 @property maplocation
 @abstract map location id.
 */
@property (nonatomic) long maplocation;

/*!
 @method initWithSublocation:location:
 @abstract Initialize the FMSection object with provided sublocation and location.
 @param aSublocation - identifier of the sublocation
 @param aLocation - identifier of the location
 */
- (id)initWithSublocation:(long)aSublocation location:(long)aLocation;

@end
