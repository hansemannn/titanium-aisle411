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

/*!
 @header MapBundleParser.h
 @abstract Map bundle parser class interface.
 */

#import <Foundation/Foundation.h>
#import "MapBundle.h"

/*!
 @class MapBundleParser
 @abstract Use instances of this class to parse map bundles.
 */
@interface MapBundleParser : NSObject {
	NSString *_bundlePath;
	BOOL _deleteOnRecycle;
}

/*!
 @method initWithPath:
 @abstract Initialize parser instance with a path to map bundle.
 @param pathToBundle map bundle directory path string
 @result initialized parser instance
 */
- (id) initWithPath:(NSString *)pathToBundle;

/*!
 @method initWithPathToArchive:
 @abstract Initialize parser instance with a path to map bundle ZIP archive.
 @discussion Decopressed map bundle will be stored in the application Documents folder.
 @param pathToArchive map bundle ZIP archive path string
 @result initialized parser instance
 */
- (id) initWithPathToArchive:(NSString *)pathToArchive;

/*!
 @method uncompressArchiveAtPath:toPath:
 @abstract Uncomress bundle ZIP archive file at pathToArchive and save result at pathToBundle.
 @param pathToArchive path to ZIP archive
 @param pathToBundle decompressed file path
 */
- (void) uncompressArchiveAtPath:(NSString *)pathToArchive toPath:(NSString *)pathToBundle;

/*!
 @method parse
 @abstract Parse map bundle at path specified in one of the init... methods and create MapBundle instance from the parsed data.
 @result MapBundle autorelease object
 */
- (MapBundle *)parse;

@end
