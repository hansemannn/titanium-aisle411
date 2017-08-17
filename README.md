# Aisle411 in Axway Titanium

Use the Aisle411 iOS-SDK in Axway Titanium.

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
