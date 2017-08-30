var aisle411,
		mapView,
		mapLoaded,
		products,
		window,
		loader;

/**
 @abstract Constructor
 */
(function constructor() {
	// A list of products. Only use those that are not picked up and match the "productsToBuy" property
	products = [{
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
	
	aisle411 = require('ti.aisle411');
	
	mapLoaded = false;
	
	window = Ti.UI.createWindow({ backgroundColor: '#fff' });
	
	loader = Ti.UI.createActivityIndicator({
		style: Ti.UI.ActivityIndicatorStyle.BIG,
		indicatorColor: 'gray'
	});
	
	aisle411.requestCachedRasterMap({
		venueId: 1100951,
		callback: function(e) {	
			mapView = aisle411.createMapView({ 
				url: e.path,
				shoppingListEnabled: true,
				unselectedPinImage: 'unselected.png',
				selectedPinImage: 'selected.png',
				mapBackgroundColor: '#eee'
			});
			mapView.addEventListener('load', mapFinishedLoading);
			window.add(mapView);	
		}
	});
	
})();

window.add(loader);
window.open();
loader.show();

function onSearchReturn(e) {
	if (!mapLoaded) {
		alert('Map not loaded, yet!');
		return;
	}
	searchProductWitSearchTerm(e.value);
}

function searchProductWitSearchTerm(searchTerm) {	
	// Simulate fetching the product in our data-structure
	for (var i = 0; i < products.length; i++) {
		var product = products[i];
		if (product.name === searchTerm) {
			result = product.sectionCode;
			searchProducts([product])
			break;
		}
	}
}

function searchProducts(_products) {
	loader.show();
	
	mapView.search({
		venueId: 1100951,
		name: 'shoppingList',
		products: _products,
		callback: function(e) {
			Ti.API.info('Search completed!');

			loader.hide();
       
      if (e.error) {
        Ti.API.error('Error:' + e.error);
        return;
      }

      showPins(_products, e.venueItems);
		}
  });
}

function showPins(products, venueItems) {
	if (!mapLoaded) {
		Ti.API.error('Error: Trying to redraw on map that is not loaded');
	}
	mapView.redrawOverlay({
		products: products,
		venueItems: venueItems
	});
}

function mapFinishedLoading(e) {
	mapLoaded = true;
	searchProducts(products);
}
