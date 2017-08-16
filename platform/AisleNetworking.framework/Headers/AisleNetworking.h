//
//  AisleNetworking.h
//  AisleNetworking
//
//  Created by voronin on 7/28/17.
//  Copyright © 2017 com.aisle411. All rights reserved.
//

#import <UIKit/UIKit.h>

//! Project version number for AisleNetworking.
FOUNDATION_EXPORT double AisleNetworkingVersionNumber;

//! Project version string for AisleNetworking.
FOUNDATION_EXPORT const unsigned char AisleNetworkingVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <AisleNetworking/PublicHeader.h>



@class IVKError;
@class IVKVenueItem;
@class CLLocation;
@class IVKVenue;

@interface AisleServer : NSObject
@property (nonatomic, class, readonly, strong) AisleServer * _Nullable shared;
+ (AisleServer * _Nullable)shared;
@property (nonatomic, class, readonly, copy) NSString * _Nonnull kIVKReadyNotificationName;
+ (NSString * _Nonnull)kIVKReadyNotificationName;
/**
 Method to get url to cached map bundle. If map hasn’t cached it downloads map bundle first
 \param forVenueId id of store
 
 \param withResponseBlock completion callback
 
 */
- (BOOL)requestCachedRasterMapForVenueId:(NSInteger)id withResponseBlock:(void (^ _Nonnull)(NSURL * _Nullable, NSArray<IVKError *> * _Nonnull))block;
/**
 Method to search product by a search term
 \param venueWithId id of store
 
 \param searchTerm search term to find product
 
 \param withStartingIndex first product index in a total search result. Can be used to implement pagination
 
 \param andEndingIndex last product index in a total search result. Can be used to implement pagination
 
 \param withMaxCount max sections count per product
 
 \param withResponseBlock result callback
 
 */
- (BOOL)searchWithVenueWithId:(NSInteger)venueId forTerm:(NSString * _Nonnull)searchTerm withStartingIndex:(NSInteger)start andEndingIndex:(NSInteger)end withMaxCount:(NSInteger)maxCount withResponseBlock:(void (^ _Nonnull)(NSArray<IVKVenueItem *> * _Nonnull, NSArray<IVKError *> * _Nonnull))block;
- (BOOL)searchWithVenueWithId:(NSInteger)venueId forTerm:(NSString * _Nonnull)searchTerm withStartingIndex:(NSInteger)start andEndingIndex:(NSInteger)end withMaxCount:(NSInteger)maxCount customHeaders:(NSDictionary<NSString *, NSString *> * _Nonnull)customHeaders withResponseBlock:(void (^ _Nonnull)(NSArray<IVKVenueItem *> * _Nonnull, NSArray<IVKError *> * _Nonnull))block;
/**
 Method to search product by UPC
 \param venueWithId id of store
 
 \param forUPC product UPC
 
 \param withStartingIndex first product index in a total search result. Can be used to implement pagination
 
 \param andEndingIndex last product index in a total search result. Can be used to implement pagination
 
 \param withMaxCount max sections count per product
 
 \param withResponseBlock result callback
 
 */
- (BOOL)searchWithVenueWithId:(NSInteger)venueId forUPC:(NSString * _Nonnull)upc withStartingIndex:(NSInteger)start andEndingIndex:(NSInteger)end withMaxCount:(NSInteger)maxCount withResponseBlock:(void (^ _Nonnull)(NSArray<IVKVenueItem *> * _Nonnull, NSArray<IVKError *> * _Nonnull))block;
/**
 Method to search shopping list content
 \param venueWithId id of store
 
 \param forItemsFromDictionary dictionary with shopping list items attributes
 
 \param withResponseBlock result callback
 
 */
- (BOOL)searchWithVenueWithId:(NSInteger)venueId forItemsFromDictionary:(NSDictionary<NSString *, id> * _Nonnull)searchDictionary withResponseBlock:(void (^ _Nonnull)(NSArray<IVKVenueItem *> * _Nonnull, NSArray<IVKError *> * _Nonnull))block;
/**
 Method to check for updates to the search suggestions. If an update is available, it is downloaded and cached. MUST be called prior to calling suggestionsFor(text:, includeContained:)
 \param forVenueWithId id of store
 
 \param withResponseBlock result callback
 
 */
- (BOOL)refreshSearchSuggestionsForVenueWithId:(NSInteger)venueId withResponseBlock:(void (^ _Nonnull)(BOOL, NSArray<IVKError *> * _Nonnull))block;
/**
 Method to return a list of suggested terms beginning with the passed trimmed text.  If search suggestions are not available, an empty array will be returned.
 \param text text to search suggestions
 
 \param includeContained if true, then result will be cobntain terms that contain text overvise terms that starts with provided text
 
 */
- (NSArray<NSString *> * _Nonnull)suggestionsFor:(NSString * _Nonnull)text includeContained:(BOOL)includeContained;
/**
 Method to return a shop list based on provided location
 \param providedLocation location to search
 
 \param startingWithIndex first store index in a total result list. Can be used to implement pagination
 
 \param endingWithIndex last store index in a total result list. Can be used to implement pagination
 
 \param withResponseBlock result callback
 
 */
- (BOOL)requestVenuesWithNear:(CLLocation * _Nullable)providedLocation startingWithIndex:(NSInteger)start endingWithIndex:(NSInteger)end withResponseBlock:(void (^ _Nonnull)(NSArray<IVKVenue *> * _Nullable, NSArray<IVKError *> * _Nonnull))block;
- (nonnull instancetype)init;
@end


/**
 \code
 IVKObject
 \endcode is the base class for most VenueKit classes.  Some classes inherit from various Apple classes.
 */
@interface IVKObject : NSObject
/**
 Human readable formatted outut of this object.  If you subclass IVKObject, you should override this.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
/**
 Makes directory if it does not exist.  Returns true if dir is in existence when done, false if a throw happens.
 For convenience.
 */
- (BOOL)makeDirWithUrl:(NSURL * _Nonnull)url;
/**
 Runs passed block on main thread.  If running on main thread, then block is executed on the same thread.
 For convenience.
 */
- (void)runOnMainThread:(void (^ _Nonnull)(void))block;
/**
 Runs passed block asynchronously in a new queue called \code
 asyncer
 \endcode.
 For convenience.
 */
- (void)runAsync:(void (^ _Nonnull)(void))block;
- (nonnull instancetype)init;
@end

@class NSArray;

/**
 IVKAccount represents a user account in the Aisle411 system.  Use \code
 (IVKApplication).signin(_:, _:, _:)
 \endcode to acquire a valid instance of this with a valid email and password.  Can be handy to use as a container for venues if an app should have need of using multiple venues.  There is no benefit to doing this though.
 */
@interface IVKAccount : IVKObject
/**
 Unique system account identity.
 */
@property (nonatomic, readonly) NSInteger accountId;
/**
 Array of IVKVenue instances the account has access to viewing.  Can be overridden by calling \code
 initVenues()
 \endcode.
 */
@property (nonatomic, readonly, copy) NSArray<IVKVenue *> * _Nonnull venues;
/**
 Objective-C NSArray of IVKVenue instances the account has access to viewing.  Can be overridden by calling \code
 initVenues()
 \endcode.
 */
@property (nonatomic, readonly, strong) NSArray * _Nonnull arrayOfVenues;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
/**
 Override method for replacing or setting the venues.  Can be useful as a container for venue objects.
 */
- (void)initVenuesWithVenues:(NSArray<IVKVenue *> * _Nonnull)venues;
/**
 Override initializer that returns an empty IVKAccount instance.  You can add venues to this using \code
 initVenues(_:)
 \endcode.
 */
- (nonnull instancetype)init;
@end

@interface IVKAutoCompleteItem : IVKObject
@property (nonatomic, readonly, copy) NSString * _Nonnull query;
@property (nonatomic, readonly, copy) NSArray * _Nonnull boldRange;
@property (nonatomic, readonly, copy) NSArray<NSString *> * _Nonnull superDepartment;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
- (nonnull instancetype)init;
@end

@interface IVKConvertedPoint : IVKObject
@property (nonatomic, readonly) float x;
@property (nonatomic, readonly) float y;
@property (nonatomic, readonly) NSInteger floor;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
/**
 Create an empty instance.
 */
- (nonnull instancetype)init;
/**
 Create an IVKCoordinate from lat and long values.
 */
- (nonnull instancetype)initWithX:(float)x y:(float)y floor:(NSInteger)floor;
@end

@class NSError;

/**
 \code
 IVKError
 \endcode is the standard error object for VenueKit.
 */
@interface IVKError : IVKObject
@property (nonatomic, readonly, copy) NSString * _Nonnull additionalInfo;
/**
 Error number that corresponds to the IVKErrorType OR provided by external error such as NSError.
 */
@property (nonatomic, readonly) NSInteger number;
/**
 Error message that corresponds to the IVKErrorType OR provided by external error such as NSError.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull message;
/**
 The IVKErrorType’s raw value.
 */
@property (nonatomic, readonly) NSInteger typeId;
/**
 The IVKErrorType’s error message.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull typeName;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
/**
 Create an IVKError with an NSError.
 */
- (nonnull instancetype)initWithNsError:(NSError * _Nonnull)nsError;
- (nonnull instancetype)init;
@end



/**
 Class defining a search item in a search list.
 */
@interface IVKSearchItem : IVKObject <NSCopying>
/**
 Item name, descriptor or type.
 e.g. coke, coca-cola, cola, soda, beverage
 */
@property (nonatomic, copy) NSString * _Nonnull term;
/**
 UPC to be used in the search
 */
@property (nonatomic, copy) NSString * _Nonnull upc;
/**
 Array of UPCs that are related to term or upc.
 */
@property (nonatomic, copy) NSArray<NSString *> * _Nonnull upcs;
/**
 Category as set up when the venue/partner was set up.
 */
@property (nonatomic, copy) NSString * _Nonnull category;
/**
 Dictionary representation consistent with the legacy shopping list search.
 */
@property (nonatomic, readonly, copy) NSDictionary<NSString *, id> * _Nullable dictionary;
/**
 Init with an array of UPCs
 */
- (nonnull instancetype)initWithUpcs:(NSArray<NSString *> * _Nonnull)upcs;
/**
 Init with a UPC.
 */
- (nonnull instancetype)initWithUpc:(NSString * _Nonnull)upc;
/**
 Init with a term and optionally a category.
 */
- (nonnull instancetype)initWithTerm:(NSString * _Nonnull)term category:(NSString * _Nonnull)category;
/**
 Init with a term, array of UPCs and optionally a category.
 */
- (nonnull instancetype)initWithTerm:(NSString * _Nonnull)term upcs:(NSArray<NSString *> * _Nonnull)upcs category:(NSString * _Nonnull)category ;
- (id _Nonnull)copyWithZone:(struct _NSZone * _Nullable)zone;
- (nonnull instancetype)init;
@end


/**
 Struct for passing a list of IVKSearchItems to IVMKMapView.search(items:).  Aka: A shopping list struct.
 */
@interface IVKSearchList : IVKObject <NSCopying>
/**
 Name of the list, not required, is not used.
 */
@property (nonatomic, copy) NSString * _Nonnull name;
/**
 Array of IVKSearchItem instances.
 */
@property (nonatomic, copy) NSArray<IVKSearchItem *> * _Nonnull items;
/**
 Dictionary in the format required for executing the search API.  Also can be used to pass into IVMKMapView.search(dict:) and IVKApplication.search(dict:)
 Dictionary representation consistent with the legacy shopping list search.
 */
@property (nonatomic, readonly, copy) NSDictionary<NSString *, id> * _Nullable dictionary;
/**
 Init with an array of search terms.
 */
- (nonnull instancetype)initWithTerms:(NSArray<NSString *> * _Nonnull)terms;
/**
 Init with an array of IVKSearchItems.
 */
- (nonnull instancetype)initWithItems:(NSArray<IVKSearchItem *> * _Nonnull)items;
/**
 Append a search item with passed term.
 */
- (void)appendWithTerm:(NSString * _Nonnull)term;
/**
 Append a search item with passed upc.
 */
- (void)appendWithUpc:(NSString * _Nonnull)upc;
/**
 Append a search item with passed item.
 */
- (void)appendWithItem:(IVKSearchItem * _Nonnull)item;
- (id _Nonnull)copyWithZone:(struct _NSZone * _Nullable)zone;
- (nonnull instancetype)init;
@end

@class NSCoder;

/**
 IVKSection represents a location within a IVKVenue.
 */
@interface IVKSection : IVKObject <NSCoding>
/**
 Unique identity for a known location in a map.
 */
@property (nonatomic, readonly) NSInteger mapPointId;
/**
 The name of the aisle or larger area of the section.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull aisle;
/**
 The name of the section of the larger area.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull section;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)aDecoder;
- (void)encodeWithCoder:(NSCoder * _Nonnull)aCoder;
- (nonnull instancetype)init;
@end


@interface IVKSublocation : IVKObject
@property (nonatomic, readonly) NSInteger locationId;
@property (nonatomic, readonly, copy) NSString * _Nonnull locationName;
@property (nonatomic, readonly) NSInteger sublocationId;
@property (nonatomic, readonly, copy) NSString * _Nonnull sublocationName;
- (nonnull instancetype)init;
@end


/**
 IVKVenue represents a venue location in the Aisle411 system.
 */
@interface IVKVenue : IVKObject <NSCoding>
/**
 Unique id across all partners.
 Was called retailerStoreId previously.
 */
@property (nonatomic, readonly) NSInteger id;
/**
 Venue name.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull name;
/**
 Name of group or parent of the venue.
 Same as retailerName.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull vendorName;
/**
 Name of group or parent of the store.
 Same as groupName.
 <em>DEPRECATED - Use .vendorName instead.</em>
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull retailerName;
/**
 Display name.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull displayName;
/**
 Id of group or parent of the venue.
 Same as retailerId.
 */
@property (nonatomic, readonly) NSInteger vendorId;
/**
 Id of group or parent of the store.
 Same as groupId.
 <em>DEPRECATED - Use .vendorId instead.</em>
 */
@property (nonatomic, readonly) NSInteger retailerId;
/**
 Partner’s venue id.
 Should be unique per partner.
 Same as vendorStoreNumber.
 */
@property (nonatomic, readonly) NSInteger vendorVenueNumber;
/**
 Partner’s store id.
 Should be unique per partner.
 Same as vendorVenueNumber.
 <em>DEPRECATED - Use .vendorVenueNumber instead.</em>
 */
@property (nonatomic, readonly) NSInteger vendorStoreNumber;
/**
 Aisle411 Venue Number (not the same as id.)
 */
@property (nonatomic, readonly) NSInteger aisle411VenueNumber;
/**
 Street address.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull address1;
/**
 Second line of street address, often used for unit (apt, ste, etc.)
 Not yet implemented.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull address2;
/**
 City or locale name applicable for the location, should match city name of postal code in the US
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull city;
/**
 Genericized name for State or Province
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull region;
/**
 Shortcut for region name for US and other countries that refer to regional political divisions as states addresses
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull state;
/**
 Shortcut for region name for Candadian and other countries that refer to regional political divisions as provinces addresses
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull province;
/**
 Genericized name for State or Procince Code
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull regionCode;
/**
 Shortcut for region code for US addresses
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull stateCode;
/**
 Shortcut for region code for Candadian addresses
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull provinceCode;
/**
 The country name
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull country;
/**
 The country’s code
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull countryCode;
/**
 Postal Code for the address, this is refered to as the ZIP code in the US.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull postalCode;
/**
 Shortcut for accessing postal code for US Addresses.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull zipCode;
/**
 Venue category
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull category;
/**
 Venue Location
 */
@property (nonatomic, readonly, strong) CLLocation * _Nullable location;
/**
 Venue Latitiude (If a location is not set this will return 0.0)
 */
@property (nonatomic, readonly) double latitude;
/**
 Venue Longitude (If a location is not set this will return 0.0)
 */
@property (nonatomic, readonly) double longitude;
/**
 Venue Hours
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull hours;
/**
 Whether the venue has an indoor map.
 */
@property (nonatomic, readonly) BOOL mapped;
/**
 Whether the venue has search.
 */
@property (nonatomic, readonly) BOOL searchable;
/**
 Telephone number.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull phone;
/**
 Flag for future deletion.
 */
@property (nonatomic, readonly) BOOL willBeRemoved;
/**
 Flag for visibility of venue.
 */
@property (nonatomic, readonly) BOOL isVisible;
/**
 Path to logo.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull logoURL;
/**
 Path to map.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull mapURL;
/**
 Path to venue or partner website.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull websiteURL;
/**
 Distance from venue search origin location.
 */
@property (nonatomic, readonly) float distance;
/**
 Ordinal number of venue in search results.
 */
@property (nonatomic, readonly) NSInteger ordinal;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
- (void)encodeWithCoder:(NSCoder * _Nonnull)aCoder;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)aDecoder;
- (nonnull instancetype)init;
@end


/**
 Represents a user location triggered activity.
 */
@interface IVKVenueActivity : IVKObject <NSCopying, NSCoding>
/**
 Activity id.
 */
@property (nonatomic, readonly) NSInteger id;
/**
 Path to web page to display.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull url;
/**
 Trigger type
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull triggerType;
/**
 Properties for the trigger.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull triggerConfig;
/**
 Activity Type
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull activityType;
/**
 Create a IVKVenueActivity with a dictionary.
 */
- (nullable instancetype)initWithDict:(NSDictionary<NSString *, id> * _Nonnull)dict;
- (id _Nonnull)copyWithZone:(struct _NSZone * _Nullable)with;
- (void)encodeWithCoder:(NSCoder * _Nonnull)aCoder;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)aDecoder;
- (nonnull instancetype)init;
@end


/**
 A VenueItem including locations in a venue, if applicable.  Can be a Synonym, Typo/Suggestion og a Featured VenueItem as well.
 */
@interface IVKVenueItem : IVKObject <NSCopying>
/**
 Name of venue item type.
 Is the raw value of the IVKVenueItemType.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull venueItemTypeName;
/**
 What this item is called.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull name;
/**
 Convenience property for name or synonym.
 This should always return a value for display purposes.
 Will return the first value it finds in:
 1. .name
 2. The first section name in the sections array.
 3. .synonym
 4. .venueItemSearchTerm
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull normalizedName;
/**
 What you passed into the search method.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull venueItemSearchTerm;
/**
 Sections where this item can be found.
 */
@property (nonatomic, readonly, copy) NSArray<IVKSection *> * _Nonnull sections;
@property (nonatomic, readonly, copy) NSString * _Nonnull sectionName;
/**
 Besides being a great purveyor of video games, in this case it is just the unique identifier of the item.
 */
@property (nonatomic, readonly) NSInteger id;
/**
 The brand.  No irons needed.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull brand;
/**
 UPC of the item, or an empty String.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull upc;
/**
 Description from the cloud.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull venueItemDescription;
/**
 Path to a coupon.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull couponURL;
/**
 The opposite of antonym.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull synonym;
/**
 Id for Synonym.
 */
@property (nonatomic, readonly) NSInteger synonymId;
/**
 Ordinal position of the item in search results.
 */
@property (nonatomic, readonly) NSInteger ordinal;
/**
 Path to image for item.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull itemImageURL;
/**
 Price for nonpriceless items.
 */
@property (nonatomic, readonly) double price;
/**
 Discounted price for nonpriceless items.
 */
@property (nonatomic, readonly) double discountedPrice;
/**
 Can be acted upon in some way not provided in IVK.
 */
@property (nonatomic, readonly) BOOL isActionable;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
- (id _Nonnull)copyWithZone:(struct _NSZone * _Nullable)with;
- (nonnull instancetype)init;
@end


/**
 Hierarchical Venue Item.
 Experimental, should only ever be returned in Aisle411 demo instances and R&D builds.
 */
@interface IVKVenueItemWithVenueItems : IVKObject
/**
 Item that is probably a Synonym or grouping name.
 */
@property (nonatomic, readonly, strong) IVKVenueItem * _Nonnull venueItem;
/**
 Array of related child IVKVenueItem objects
 */
@property (nonatomic, readonly, copy) NSArray<IVKVenueItem *> * _Nonnull venueItems;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
- (nonnull instancetype)init;
@end


/**
 IVKVenue subclass that includes the best location in the venue for the search term using the fetchVenues() method with search term.
 */
@interface IVKVenueWithVenueItem : IVKVenue
/**
 Location in venue for the search result.
 */
@property (nonatomic, readonly, strong) IVKSection * _Nonnull section;
/**
 Search term.
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull venueItemSearchTerm;
/**
 Description
 */
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)aDecoder;
- (void)encodeWithCoder:(NSCoder * _Nonnull)aCoder;
@end
