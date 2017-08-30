//
//  SVPulsingAnnotationView.h
//
//  Created by Sam Vermette on 01.03.13.
//  https://github.com/samvermette/SVPulsingAnnotationView
//

#import <UIKit/UIKit.h>

/*!
 @class SVPulsingAnnotationView
 @abstract custom view implementaion with same behavior as MKUserLocationView
 */

@interface SVPulsingAnnotationView : UIView

@property (nonatomic, strong) UIColor *annotationColor; // default is same as MKUserLocationView
@property (nonatomic, strong) UIColor *pulseColor; // default is same as annotationColor
@property (nonatomic, strong) UIImage *image; // default is nil
@property (nonatomic, strong) UIImage *headingImage; // default is nil

@property (nonatomic, readwrite) float pulseScaleFactor; // default is 5.3
@property (nonatomic, readwrite) NSTimeInterval pulseAnimationDuration; // default is 1s
@property (nonatomic, readwrite) NSTimeInterval outerPulseAnimationDuration; // default is 3s
@property (nonatomic, readwrite) NSTimeInterval delayBetweenPulseCycles; // default is 1s

@property (nonatomic, copy) void (^willMoveToSuperviewAnimationBlock)(SVPulsingAnnotationView *view, UIView *superview); // default is pop animation

- (void)hideHeadingImage:(BOOL)hide;
@end
