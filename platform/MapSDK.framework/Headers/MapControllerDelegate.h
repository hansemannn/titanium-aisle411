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

@class MapController;
@class CMControlPoint;


/*!
 @protocol MapControllerDelegate
 @abstract Implements the main floor map actions.
 */
@protocol MapControllerDelegate <NSObject>
@required

/*!
 @method mapControllerDidFinishLoading:forItem:
 @abstract Notifies the observer when map did finish loading
 @param aMapController The MapController instance
 */
- (void)mapControllerDidFinishLoading:(MapController*)aMapController;

@optional

/*!
 @method mapTappedAtPoint:forItem:
 @abstract Notifies the observer when user tapped some control point
 @param aPoint Destination point
 */
- (void)mapTappedAtPoint:(CGPoint)aPoint;

/*!
 @method userInteractedWithScrollView
 @abstract Notifies the observer when user starts to interact with map by scrolling, dragging or zooming it
 */
- (void)userInteractedWithScrollView;

/*!
 @method mapDidZoom
 @abstract Notifies the observer when the map is zooming
 */
- (void)mapDidZoom;

@end