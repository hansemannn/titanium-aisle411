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

static const NSUInteger kFirstFloorNumber = 1;

@class CMapMetaData;
@protocol MapBundleTilesProvider
- (void)loadTileForStoreMap:(int)storeID level:(int)mapLevel zoom:(float)mapZoom xTilePosition:(int)xPos yTilePosition:(int)yPos withTopPriority:(BOOL)topPriority;
@end

@protocol MapBundleTilesUpdateDelegate
-(void) tilesWereUpdated;
-(void) tilesWereUpdatedWithXPos:(int)x andYPos:(int)y;
@end

@interface MapBundle : NSObject {
	CMapMetaData* _mapMetadata;
	NSString* _path;
	BOOL _deleteOnRecycle;
    id<MapBundleTilesProvider> tilesProvider;
    NSMutableSet *allowedTiles;
}

@property (nonatomic, readonly) BOOL dummy;
@property (nonatomic, readonly) BOOL validated;
@property (nonatomic, assign) id<MapBundleTilesProvider> tilesProvider;
@property (nonatomic, assign) id<MapBundleTilesUpdateDelegate> tilesUpdateDelegate;

/*!
 @method getFloorCount:
 @abstract Returns number of the floors on this map
 @result Floor count
 */
- (int)getFloorCount;

/*!
 @method getFloorNames
 @abstract Returns array of names of all floors
 @result An array of NSString objects
 */
- (NSArray *)getFloorNames;

/*!
 @method controlPointWithId:
 @abstract Returns the control point on the floor map with specified id
 @param pointId The point's id
 @result Control point
 */
- (CMControlPoint*)controlPointWithId:(int)pointId;

/*!
 @method controlPointsForFloor:
 @abstract Returns all the points on specified level
 @param level 
 @result An array of CMControlPoint objects
 */
- (NSArray *)controlPointsForFloor:(int)level;

/*!
 @method getStoreID:
 @abstract Returns the id of the store
 @result Store ID
 */
- (int)getStoreID;

/*!
 @method getMapMetadataVersion:
 @abstract returns version of the map meta data
 @result version: 1.0, 1.1, etc
 */
- (float)getMapMetadataVersion;

/*!
 @method entranceControlPointForSubLocationId:
 @abstract returns the nearest sublocation entrance for provided sublocation
 @param subLocationId 
 @result CMControlPoint object or nil
 */
- (CMControlPoint*)entranceControlPointForSubLocationId:(NSNumber *)subLocationId;

/*!
 @method entranceControlPointForLocationId:
 @abstract returns the nearest location entrance for provided location
 @param locationId 
 @result CMControlPoint object or nil
 */
- (CMControlPoint*)entranceControlPointForLocationId:(NSNumber *)locationId;

/*!
 @method entranceControlPointForMapLocationId: [FOR PREVIEW]
 @abstract returns the nearest location entrance for provided map location
 @param mapLocationId 
 @result CMControlPoint object or nil
 */
- (CMControlPoint*)entranceControlPointForMapLocationId:(NSNumber *)mapLocationId;

/*!
 @method entranceControlPointForPoint:floorLevel
 @abstract returns the nearest location entrance for provided point and level
 @param locationPoint
 @param floorLevel 
 @result CMControlPoint object
 */
- (CMControlPoint*)entranceControlPointForPoint:(CGPoint) locationPoint floorLevel: (int) floorLevel;


/*!
 @method firstMallEntrance:
 @abstract returns the mall entrance for the first floor
 @result CMControlPoint object 
 */
- (CMControlPoint*)firstMallEntrance;

/*!
 @method addTile:forLevel:zoom:xPos:yPos:
 @abstract writes tile data to its destination
 @return YES if we successfully saved tiledData, NO otherwise
 */
- (BOOL) addTile:(NSData *) tileData forLevel:(int)level zoom:(float)zoom xPos:(int)xPos yPos:(int)yPos;

/*!
 @method requestAllMissingTiles
 @abstract request missing tiles from MapBundleTilesProvider
 */
- (void) requestAllMissingTiles;


/*!
 @method needRedrawForScale:floorLevel:
 @return YES if we switched to/from custom zoom level and need to redraw tiles layer
 */
- (BOOL) needRedrawForScale:(float) scale floorLevel:(int) floorLevel;


/*!
 @method isValidFloorLevel:
 @return YES if floor level is valid, NO otherwise
*/
-(BOOL) isValidFloorLevel: (int) floorLevel;


/*!
 @method isValidFloorLevel
 @return first valid floor level
 */
-(int) firstValidFloorLevel;


/*!
 @method mapZoomForScale:floorLevel:
 @return map zoom level for current scale and floor level
 */
- (float) mapZoomForScale:(float) scale floorLevel:(int) floorLevel;


/*!
 @method mapScaleForZoom:floorLevel:
 @return map scale for current map zoom level and floor level
 */
- (float) mapScaleForZoom:(float) zoom floorLevel:(int) floorLevel;

@end
