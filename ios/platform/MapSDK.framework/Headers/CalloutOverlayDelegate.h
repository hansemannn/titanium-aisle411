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

@class CalloutOverlay;

@protocol CalloutOverlayDelegate <NSObject>

@optional

- (void)calloutOverlay:(CalloutOverlay*)overlay didItemSelected:(OverlayItem*)item;
- (void)calloutOverlay:(CalloutOverlay*)overlay didItemReselected:(OverlayItem*)oldItem withItem:(OverlayItem*)item;
- (void)calloutOverlay:(CalloutOverlay*)overlay didItemDeselected:(OverlayItem*)item;

- (BOOL)calloutOverlay:(CalloutOverlay*)overlay shouldZoom:(OverlayItem*)item;

@end