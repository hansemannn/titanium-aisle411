
var loader = Ti.UI.createActivityIndicator({ style: Ti.UI.ActivityIndicatorStyle.BIG, indicatorColor: "gray" });
var window = Ti.UI.createWindow({ backgroundColor: '#fff', barColor: '#2f880f', title: 'Map Search' });
var nav = Ti.UI.iOS.createNavigationWindow({ window: window });
var searchBar = Ti.UI.createSearchBar({barColor: '#2f880f', top: 0});

searchBar.addEventListener('return', function(e) {
	if (!mapLoaded) {
		alert('Map not loaded, yet!');
		return;
	}
	searchProductWitSearchTerm(e.value);
});

window.add(loader);
loader.show();

// store 356 (stage)
var venueId = 1100951;

// store 356 (production)
// var venueId = '1141855';

var aisle411 = require('ti.aisle411');
var mapView = null;
var mapLoaded = false;

aisle411.requestCachedRasterMap({
	venueId:  venueId,
	callback: function (e) {
    mapView = aisle411.createMapView({ 
			url: e.path,
			shoppingModeEnabled: false,
			unselectedPinImage: 'unselected.png',
      selectedPinImage: 'selected.png'
		});
		mapView.addEventListener('load', mapFinishedLoading);
		window.add(mapView);
		window.add(searchBar);
	},
});

nav.open();

// A list of products. Only use those that are not picked up and match the "productsToBuy" property
var products = [{
	id: 1,
	name: 'Shampoo',
	sectionCode: '7'
},{
	id: 2,
	name: 'Hand Cream',
	sectionCode: '7'
},//{
// 	id: 3,
// 	name: 'Isotonic drink',
// 	sectionCode: '44',
// 	isPickedUp: true
// },
{
	id: 4,
	name: 'Bread',
	sectionCode: '4'
},//{
// 	id: 5,
// 	name: 'Milk',
// 	sectionCode: '4'
// },{
// 	id: 6,
// 	name: 'Sour Cream',
// 	sectionCode: '4'
// },
{
	id: 7,
	name: 'Toothpaste',
	sectionCode: '8'
},{
	id: 8,
	name: 'Cheese',
	sectionCode: '36'
}];

function searchProductWitSearchTerm(searchTerm) {	
	var result = null;
	
	for (var i = 0; i < products.length; i++) {
		var product = products[i];
		console.log(product.name + '/' + searchTerm);
		if (product.name === searchTerm) {
			result = product.sectionCode;
			break;
		}
	}
	
	if (result == null) {
		alert('Cannot find product with name: ' + searchTerm);
		return;
	}
	
	var product = products[0];
	mapView.search({
		venueId: venueId,
		searchTerm: result, // Pass the section-code of the desired product
		startingIndex: 0,
		endingIndex: 1,
		maxCount: 25,
		callback: function(e) {
			Ti.API.warn('Search completed:');
			Ti.API.warn(e);
        
      if (e.error) {
        Ti.API.error('Error:' + e.error);
        loader.hide();
        return;
      }

      showPins([product], e.venueItems);
		}
  });
}

function showPins(products, venueItems) {
	if (!mapLoaded) {
    loader.show();
		Ti.API.error('Error: Trying to redraw on map that is not loaded');
	}
	mapView.redrawOverlay({
		products: products,
		venueItems: venueItems
	});
  loader.hide();
}

function mapFinishedLoading(e) {
	mapLoaded = true;
}
