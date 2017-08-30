# Aisle411 in Axway Titanium

Use the Aisle411 SDK in Axway Titanium.

## Requirements (iOS)
- File `com.aisle411.venuekit.plist` bundled with your application:
```plist
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>partnerID</key>
	<string>PARTNER_ID</string>
	<key>partnerSecret</key>
	<string>PARTNER_SECRET</string>
	<key>host</key>
	<string>test.aisle411.ws</string>
</dict>
</plist>
```
- Location permissions in your `Info.plist`:
```plist
<key>NSLocationWhenInUseUsageDescription</key>
<string>Can we access your location?</string>
```

## Documentation

### Top-Level

#### Methods

- [x] `requestCachedRasterMap(args)`
  - `venueId` (Number)
  - `callback` (Callback)
- [x] `search(args)`
  - `venueId` (Number)
  - `term` (String)
  - `startingIndex` (Number)
  - `endingIndex` (Number)
  - `maxCount` (Number)
  - `callback` (Callback)

### `MapView`
Create a new map-view with `Aisle411.createMapView()`.

#### Properties

- [x] `url` (use the `e.path` from the `requestCachedRasterMap` callback)
- [x] `floor`
- [x] `logoPosition`
- [x] `shoppingListEnabled`
- [x] `compassEnabled`
- [x] `zoomButtonsHidden`
- [x] `zoomOnPinClickEnabled`
- [x] `mapBackgroundColor`
- [x] `selectedPinImage`
- [x] `unselectedPinImage`

#### Methods

- [x] `clear()`
- [x] `zoomIn()`
- [x] `zoomOut()`
- [x] `redraw()`
- [x] `redrawOverlay()`
- [x] `fadeIn(args)`
  - `zoom` (Number)
  - `relativeToScreen` (Boolean)
- [x] `reloadTiles()`

#### Events
- [x] `load`
- [x] `informationBarItemClick`
- [x] `didItemSelected`
- [x] `didItemReselected`
- [x] `didItemDeselected`

## Author
Hans Knöchel

## License
Copyright(c) 2017-present by Ayway Appcelerator. All Rights Reserved. 
Please see the LICENSE file included in the distribution for further details.
