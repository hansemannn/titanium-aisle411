/**
 * titanium-aisle411
 *
 * Created by Hans Knöchel
 * Copyright (c) 2017 Axway Appcelerator. All rights reserved.
 */

#import "TiModule.h"

typedef enum TiAisle411SearchType: NSUInteger {
  TiAisle411SearchTypeFulltextSearch = 0,
  TiAisle411SearchTypeShoppingList
} TiAisle411SearchType;

@interface TiAisle411Module : TiModule {

}

- (void)requestCachedRasterMap:(id)args;

- (void)search:(id)args;

@end
