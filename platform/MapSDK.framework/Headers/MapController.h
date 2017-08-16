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

#import <QuartzCore/QuartzCore.h>

#import "MapControllerDelegate.h"
#import "MapBundle.h"
#import "Overlay.h"

@class MapView;
@class TiledView;
@class CMapMetadataParser;
@class CMFloor;
@class Responder;

/*!
 @header MapController.h
 @abstract Map view controller class definition.
 */

static const float edgeSize = 150.0f;//60.0f;

/*!
 @enum AisleLogoPosition
 @abstract Position of Aisle411 logo
 @constant AisleLogoRightBottomPosition Aisle411 logo in the right bottom corner
 @constant AisleLogoLeftBottomPosition Aisle411 logo in the left bottom corner
 */
typedef enum {
    AisleLogoRightBottomPosition,
    AisleLogoLeftBottomPosition
} AisleLogoPosition;

/*!
 @class MapController
 @abstract Map view controller class interface.
 */

@interface MapController : UIViewController <UIScrollViewDelegate> {
    MapBundle*	_mapBundle;
    
    MapView* _scView;
    TiledView* _tiledView;
    Responder* _responder;
    
    CGRect _mapRect;
    CGSize _edgeSize;
    CMFloor* _currentFloor;
    
    id<MapControllerDelegate> _mapControllerDelegate;
    
@private
    
    UIImageView* _hlView;
    
    CATiledLayer*	_tiledLayer;
    float _currentFactor;
    
    BOOL _isCompassEnabled;
    
    NSMutableArray *overlays;
    float yShift;
    
    AisleLogoPosition _logoPosition;
    BOOL didCallFinishDelegate;
}

@property (nonatomic, readonly) NSMutableArray *overlays;
@property (readonly) MapView* mapView;
@property (readonly) TiledView* tiledView;
@property (nonatomic, assign) id<MapControllerDelegate> mapControllerDelegate;
@property (nonatomic, retain) MapBundle* mapBundle;
@property (assign, getter = getFloorLevel, setter = setFloor:) int floorLevel;
@property (nonatomic, assign) AisleLogoPosition logoPosition;


- (void)resetMapRotation;/////////////
- (void)setRotation:(CGFloat)rotation;//newly added for map rotation
- (CGSize)getMapSize;

- (void)setPosition:(OverlayItem*)item animated:(BOOL)animated;

/****************************************************************************************************/

/*!
 method enableMapRotation
 @abstract Enable map rotation or not
 @isRotatable Allow map to rotate or not
 */
- (void)enableMapRotation:(BOOL)isRotatable;

/*!
 method customFloorLevels
 @abstract This method enable user to custom floor number
 @paramfloorAscendingOrder Note,must in ascending order, and must be consistent with the actual floor numbers count.
 use this after setting mapBundle: e.g map.bundle = mapBundle; [map customFloorLevels:xxx];
 */
- (void)customFloorLevels:(NSArray *)floorAscendingOrder;

- (CGFloat)returnScaleLevel;
/****************************************************************************************************/

/*!
 @method setFloor:
 @abstract Changes the current floor
 @param floor Floor number
 */
- (void)setFloor:(int)floor;

/*!
 @method setupMapView
 @abstract Adjusts the map controller to represent the current floor using the map bundle (property mapBundle)
 */
- (void)setupMapView;

/*!
 @method compassAction:
 @abstract Deprecated
 */
- (void)compassAction:(id)sender;

/*!
 @method setCompassEnabled:
 @abstract Enables or disables compass
 @param en Boolean value indicating whether the compass should be enabled or disabled
 */
- (void)setCompassEnabled:(BOOL)en;

/*!
 @method scaleToRect:animated:
 @abstract Scales the floor map to the specified rectangle
 @param zoomRect The rectangle which the map should be zoomed to
 */
- (void)scaleToRect:(CGRect)zoomRect animated:(BOOL)animated;


/*!
 @method scaleToInitialRectAnimated:
 @abstract Scales the floor map to the initial rectangle
 @param animated YES if the map should be scaled animated, NO otherwise
 */
- (void)scaleToInitialRectAnimated:(BOOL)animated;

// ***
/*!
 @method addOverlay:
 @abstract Adds the specified overlay onto the map
 @param overlay The overlay instance
 */
- (void) addOverlay: (Overlay *)overlay;
/*!
 @method removeOverlay:
 @abstract Removes the specified overlay from the map
 @param overlay The overlay instance
 */
- (void) removeOverlay: (Overlay *)overlay;



// additions
- (void) deselectAll;
- (void) redraw;
- (void) redrawOverlay: (Overlay *) overlay;
- (UIView*) logoView;
/*!
 @method setLogoBottomOffset:
 @abstract Sets aisle411 logo offset relative the bottom of the screen
 @param offset Offset value
 */
- (void) setLogoBottomOffset:(CGFloat)offset;

/*!
 @method setYShift:
 @abstract Deprecated
 */
- (void)setYShift:(float)shift;

/*!
 @method clear
 @abstract Clear map view.
 */
- (void)clear;

/*!
 @method zoomIn
 @abstract Zoom in the floor map.
 */
- (void)zoomIn;

/*!
 @method zoomOut
 @abstract Zoom out the floor map.
 */
- (void)zoomOut;

/*!
 @method canZoomIn
 @abstract YES if the floor map can be zoomed in, NO otherwise.
 */
- (BOOL)canZoomIn;

/*!
 @method canZoomOut
 @abstract YES if the floor map can be zoomed out, NO otherwise.
 */
- (BOOL)canZoomOut;

/*!
 @method reloadTiles
 @abstract Reloads map tiles.
 */
- (void)reloadTiles;

/*!
 @method reloadTilesOnXPos:andYPos:
 @abstract Reloads map tiles.
 */
- (void)reloadTilesOnXPos:(int)x andYPos:(int)y;

/*!
 @method fadeInWithZoom:
 @abstract Fade in the floor map with specific zoom value.
 @param zoom Zoom
 @param YES if zoom is given relative to scroll view size, NO if it is given in terms of map bundle
 */
- (void)fadeInWithZoom:(CGFloat)zoom relativeToScreen:(BOOL)relativeToScreen;


/*!
 @method setScrollViewTransformIdentity
 @abstract Set scroll view transform to identity.
 @discussion Should be called when interface orientation changes before view sizes are adjusted.
 */
- (void)setScrollViewTransformIdentity;

/*!
 @method resetScrollViewTransform
 @abstract Restore scroll view rotation angle.
 @discussion Should be called when interface orientation changes after view sizes are adjusted.
 */
- (void)resetScrollViewTransform;

/*!
 @property mapRotation
 @abstract Map rotation angle.
 */
@property (nonatomic, assign) CGFloat mapRotation;

/*!
 @property mapAzimuth
 @abstract Map azimuth angle.
 */
@property (nonatomic, assign) CGFloat mapAzimuth;


/*!
 @property mapCoordinatesDefined
 @abstract YES if map coordinates are deined in map bundle.
 */
@property (nonatomic, readonly) BOOL mapCoordinatesDefined;

/*!
 @method recalculateInsets
 @abstract Recalculates map insets. Should be called after orientation change.
 */
-(void) recalculateInsets;

/*!
 @method recalculateMinScale
 @abstract Recalculates min scale. Should be called after orientation change.
 */
-(void) recalculateMinScale;

/*!
 @method floorSelectorTextColorForState:
 @abstract Returns the text color used for a floor selector buttons state.
 @param state The state that uses the text color. Possible values are described in UIControlState.
 @return The text color used for the specified state.
 */
- (UIColor *)floorSelectorTextColorForState:(UIControlState)state;

/*!
 @method setFloorSelectorTextColor:
 @abstract Sets the text color to use for the specified state. In general, if a property is not specified for a state, the default is to use the UIControlStateNormal value. If the UIControlStateNormal value is not set, then the property defaults to a SDK value.
 @param image The text color to use for the specified state.
 @param state The state that uses the specified text color. The values are described in UIControlState.
 */
- (void)setFloorSelectorTextColor:(UIColor *)color forState:(UIControlState)state;

/*!
 @method floorSelectorBackgroundColorForState:
 @abstract Returns the background color used for a floor selector buttons state.
 @param state The state that uses the background color. Possible values are described in UIControlState.
 @return The background color used for the specified state.
 */
- (UIColor *)floorSelectorBackgroundColorForState:(UIControlState)state;

/*!
 @method setFloorSelectorBackgroundColor:
 @abstract Sets the background color to use for the specified state. In general, if a property is not specified for a state, the default is to use the UIControlStateNormal value. If the UIControlStateNormal value is not set, then the property defaults to a SDK value.
 @param image The background color to use for the specified state.
 @param state The state that uses the specified background color. The values are described in UIControlState.
 */
- (void)setFloorSelectorBackgroundColor:(UIColor *)color forState:(UIControlState)state;

/*!
 @method setMapBackgroundColor:
 @abstract Sets the background color to the map.
 @param color The background color for the map.
 */
- (void)setMapBackgroundColor:(UIColor *)color;

/*!
 @method zoomInButtonImageForState:
 @abstract Returns the image used for a zoomIn button state.
 @param state The state that uses the image. Possible values are described in UIControlState.
 @return The image used for the specified state.
 */
- (UIImage *)zoomInButtonImageForState:(UIControlState)state;

/*!
 @method setZoomInButtonImage:forState:
 @abstract Sets the image to use for the specified state. In general, if a property is not specified for a state, the default is to use the UIControlStateNormal value. If the UIControlStateNormal value is not set, then the property defaults to a SDK value.
 @param image The image to use for the specified state.
 @param state The state that uses the specified image. The values are described in UIControlState.
 */
- (void)setZoomInButtonImage:(UIImage *)image forState:(UIControlState)state;

/*!
 @method zoomOutButtonImageForState:
 @abstract Returns the image used for a zoomOut button state.
 @param state The state that uses the image. Possible values are described in UIControlState.
 @return The image used for the specified state.
 */
- (UIImage *)zoomOutButtonImageForState:(UIControlState)state;

/*!
 @method setZoomOutButtonImage:forState:
 @abstract Sets the image to use for the specified state. In general, if a property is not specified for a state, the default is to use the UIControlStateNormal value. If the UIControlStateNormal value is not set, then the property defaults to a SDK value.
 @param image The image to use for the specified state.
 @param state The state that uses the specified image. The values are described in UIControlState.
 */
- (void)setZoomOutButtonImage:(UIImage *)image forState:(UIControlState)state;
- (void)setZoomButtonsHidden:(BOOL)hidden;
@end
