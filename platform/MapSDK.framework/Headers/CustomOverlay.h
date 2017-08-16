//
//  CustomOverlay.h
//  MapSDK
//
//  Created by Minxin Guo on 11/22/16.
//
//

#import "Overlay.h"
#import "OverlayItem.h"

@interface CustomOverlay : Overlay
{
    OverlayItem *item;
}

/*!
 @property item
 @abstract OverlayItem used for displaying as custom item position. Use point property of OverlayItem to specify item coordinates; use setImage: method of OverlayItem instance to specify item image.
 */
@property (nonatomic, retain) OverlayItem *item;
@end
