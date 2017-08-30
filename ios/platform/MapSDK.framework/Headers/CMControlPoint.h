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

typedef enum {
	kMPointUnknown,
	kMPointTypeOneWay,
	kMPointTypeMidPoint,
	kMPointTypeMallEntrance,
	kMPointTypeStairs,
	kMPointTypeSubLocEntrance
} MPointType;

/*!
 @class CMPoint
 @abstract Represents some point on the map
 */
@interface CMPoint : NSObject <NSCopying> {
@private
	float _x;
	float _y;
	int _floorLevel;
}

@property (assign) int floorLevel;
@property (assign) float x;
@property (assign) float y;

+ (CMPoint*)pointWithX:(float)X Y:(float)Y;
+ (CMPoint*)pointWithX:(float)X Y:(float)Y floor:(int)level;

@end

/*!
 @class CMControlPoint
 @abstract Represents the control point on the map
 */
@interface CMControlPoint : CMPoint {
@private
	int _id;
	MPointType _pointType;
	NSMutableArray* _subLocIDs; //this value is valid only for sub-loc-entrance control points
	NSMutableArray* _locIDs; //this value is valid for sub-loc-entrance control points only
    NSMutableArray* _mapLocIDs;
	
	NSMutableArray* _siblings; //integer sibling's identificators (NSNumber)
	NSMutableArray* _counterparts; //integer counterpart's identificators (NSNumber)
//+	NSMutableArray* _sbEntrances;
}
/* 
 @property ident 
 @abstract Point identifier
 */

@property (assign) int ident;

/* 
 @property pointType
 @abstract Point type
 */

@property (assign) MPointType pointType;

/*
 @property locIDs
 @abstract An array of NSNumbers containing the list of location ids assigned to this control point
 */

@property (readonly) NSArray* locIDs;

/*
 @property subLocIDs
 @abstract An array of NSNumbers containing the list of sub-location ids assigned to this control point
 */

@property (readonly) NSArray* subLocIDs;


/*
 @property mapLocIDs 
 @abstract An array of NSNumbers containing the list of map location ids assigned to this control point
 */
@property (readonly) NSArray* mapLocIDs;

/*
 @property subLocIDs
 @abstract An array of NSNumbers containing the list of counterpart points. 
 
 Counterpart point is only defined for stairs/elevators and represent the stairs/elevator points 
 placed on another floors and connected with this point. 
 */

@property (readonly) NSArray* counterParts;

/*
 @property siblings
 @abstract An array of sibling points
 */

@property (readonly) NSArray* siblings;


@end

