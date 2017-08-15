/**
 * titanium-aisle411
 *
 * Created by Hans Knöchel
 * Copyright (c) 2017 Axway Appcelerator. All rights reserved.
 */

#import "TiAisle411Module.h"
#import "TiBase.h"
#import "TiHost.h"
#import "TiUtils.h"

@implementation TiAisle411Module

#pragma mark Internal

// This is generated for your module, please do not change it
- (id)moduleGUID
{
	return @"2bd872ef-d80a-4d07-be50-cbd424b67eb4";
}

// This is generated for your module, please do not change it
- (NSString *)moduleId
{
	return @"ti.aisle411";
}

#pragma mark Lifecycle

- (void)startup
{
	// This method is called when the module is first loaded
	// You *must* call the superclass
	[super startup];
	NSLog(@"[DEBUG] %@ loaded",self);
}

#pragma Public APIs

@end
