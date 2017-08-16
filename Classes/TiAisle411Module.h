/**
 * titanium-aisle411
 *
 * Created by Hans Knöchel
 * Copyright (c) 2017 Axway Appcelerator. All rights reserved.
 */

#import "TiModule.h"

@interface TiAisle411Module : TiModule {

}

- (void)requestCachedRasterMapForVenueId:(id)args;

- (void)search:(id)args;

@end
