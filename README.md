# Aisle411 in Axway Titanium

Use the Aisle411 iOS-SDK in Axway Titanium.

## Requirements
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
Create a new map-view with Aisle411.createMapView()

#### Properties

- [x] `floor`
- [x] `logoPosition`
- [x] `compassEnabled`

#### Methods

- [x] `clear()`
- [x] `zoomIn()`
- [x] `zoomOut()`
- [x] `redraw()`
- [x] `redrawOverlay(args)`
  - `products` (Array<Object<name, id>>)
- [x] `fadeIn(args)`
  - `zoom` (Number)
  - `relativeToScreen` (Boolean)
- [x] `reloadTiles()`

## Author
Hans Knöchel

## License
Copyright(c) 2017-Present by Ayway Appcelerator. All Rights Reserved. 
Please see the LICENSE file included in the distribution for further details.
