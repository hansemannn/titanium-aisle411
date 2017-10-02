/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2017 by Axway, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
package ti.aisle411;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.view.ViewGroup;
import com.aisle411.mapsdk.map.InformationBar;
import com.aisle411.mapsdk.map.InformationBarHeader;
import com.aisle411.mapsdk.map.InformationBarLayout;
import com.aisle411.mapsdk.map.ItemizedOverlay;
import com.aisle411.mapsdk.map.LevelInfo;
import com.aisle411.mapsdk.map.MapBundle;
import com.aisle411.mapsdk.map.MapView;
import com.aisle411.mapsdk.shopping.Product;
import com.aisle411.mapsdk.shopping.ProductOverlayItem;
import com.aisle411.mapsdk.shopping.ProductSetOverlay;
import com.aisle411.mapsdk.shopping.ProductType;
import com.aisle411.mapsdk.shopping.Section;
import com.aisle411.sdk.core.error.Aisle411Error;
import com.aisle411.sdk.core.object.BaseVenueItem;
import com.aisle411.sdk.core.object.FeaturedVenueItem;
import com.aisle411.sdk.core.object.GTINsVenueItem;
import com.aisle411.sdk.core.object.GTINVenueItem;
import com.aisle411.sdk.core.object.TermVenueItem;
import com.aisle411.sdk.core.object.VenueSection;
import com.aisle411.sdk.core.request.search.object.SearchList;
import com.aisle411.sdk.core.request.search.object.SearchPager;
import com.aisle411.sdk.core.request.search.object.SearchString;
import com.aisle411.sdk.core.request.search.object.TermSearchItem;
import com.aisle411.sdk.core.request.search.VenueItemsListSearch;
import com.aisle411.sdk.core.request.search.VenueItemsSearch;
import com.aisle411.sdk.core.result.VenueItemsListResult;
import com.aisle411.sdk.core.result.VenueItemsResult;
import com.aisle411.sdk.rastermap.bundle.RasterMapBundleResult;
import com.aisle411.sdk.rastermap.RasterMapBundleApi;
import com.aisle411.sdk.rastermap.RasterMapUtility;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import org.appcelerator.kroll.annotations.Kroll;
import org.appcelerator.kroll.common.Log;
import org.appcelerator.kroll.KrollDict;
import org.appcelerator.kroll.KrollFunction;
import org.appcelerator.kroll.KrollObject;
import org.appcelerator.kroll.KrollProxy;
import org.appcelerator.titanium.proxy.TiViewProxy;
import org.appcelerator.titanium.TiApplication;
import org.appcelerator.titanium.util.TiConvert;
import org.appcelerator.titanium.view.TiUIView;
import org.appcelerator.titanium.view.TiDrawableReference;


@Kroll.proxy(creatableInModule=Aisle411Module.class, propertyAccessors = {
	MapViewProxy.PropertyNames.COMPASS_ENABLED,
	MapViewProxy.PropertyNames.FLOOR,
	MapViewProxy.PropertyNames.LOGO_POSITION,
	MapViewProxy.PropertyNames.MAP_BACKGROUND_COLOR,
	MapViewProxy.PropertyNames.ZOOM_BUTTONS_HIDDEN,
	MapViewProxy.PropertyNames.ZOOM_ON_PIN_CLICK_ENABLED
})
public class MapViewProxy extends TiViewProxy
{
	private static final String TAG = "MapViewProxy";

	public abstract class PropertyNames
	{
		public static final String COMPASS_ENABLED = "compassEnabled";
		public static final String FLOOR = "floor";
		public static final String LOGO_POSITION = "logoPosition";
		public static final String MAP_BACKGROUND_COLOR = "mapBackgroundColor";
		public static final String SELECTED_PIN_IMAGE = "selectedPinImage";
		public static final String SHOPPING_LIST_ENABLED = "shoppingListEnabled";
		public static final String UNSELECTED_PIN_IMAGE = "unselectedPinImage";
		public static final String URL = "url";
		public static final String ZOOM_BUTTONS_HIDDEN = "zoomButtonsHidden";
		public static final String ZOOM_ON_PIN_CLICK_ENABLED = "zoomOnPinClickEnabled";
	}


	private static class MapViewContainer extends TiUIView
	{
		private InformationBarLayout infoBarLayout;
		private InformationBar infoBar;
		private ProductSetOverlay productsOverlay;
		private MapProductListAdapter productListAdapter;
		private BitmapDrawable defaultPinDrawable;
		private Drawable selectedPinDrawable;
		private int lastSelectedPinIndex = -1;
		private boolean isZoomOnPinEnabled = true;
		private boolean isShoppingListEnabled = false;

		public MapViewContainer(TiViewProxy proxy, Activity activity)
		{
			super(proxy);

			// Create a green rectangle drawable to be used as the default map pin marker.
			// Note: Aisle411 library does not have a default marker and will show nothing.
			//       Also, it must be a BitmapDrawable. Library will not render any other drawable type.
			Bitmap bitmap = Bitmap.createBitmap(16, 16, Bitmap.Config.ARGB_8888);
			bitmap.eraseColor(android.graphics.Color.argb(255, 0, 128, 0));
			bitmap.setDensity(android.util.DisplayMetrics.DENSITY_DEFAULT);
			this.defaultPinDrawable = new BitmapDrawable(activity.getResources(), bitmap);

			// Set up a product list adapter needed by the "infoBar" member variable.
			// Will be used to display a list of products in the information bar overlay.
			this.productListAdapter = new MapProductListAdapter(activity);
			this.productListAdapter.setItemTappedListener(new MapProductListAdapter.ItemTappedListener()
			{
				@Override
				public void onTapped(Product product)
				{
					// Validate.
					if (product == null) {
						return;
					}

					// Fetch the map view's proxy.
					KrollProxy proxy = MapViewContainer.this.getProxy();
					if (proxy == null) {
						return;
					}

					// Fire an event providing info about the item tapped on in the slide-out view.
					KrollDict event = new KrollDict();
					event.put("product", MapViewProxy.createDictionaryFrom(product));
					proxy.fireEvent("informationBarItemClick", event);
				}
			});

			// Set up the information bar overlay view. Used to display a list of products.
			this.infoBar = new InformationBar()
			{
				@Override
				public boolean isFixed(
					ItemizedOverlay<ProductOverlayItem> overlay, ProductOverlayItem item)
				{
					// Returning true prevents user from being able to slide-out product list overlay.
					return !MapViewContainer.this.isShoppingListEnabled;
				}

				@Override
				public CharSequence getExpandInstructions(
					ItemizedOverlay<ProductOverlayItem> overlay, ProductOverlayItem item)
				{
					return null;
				}

				@Override
				public CharSequence getCollapseInstructions(
					ItemizedOverlay<ProductOverlayItem> overlay, ProductOverlayItem item)
				{
					return null;
				}

				@Override
				public CharSequence getSublocation(
					ItemizedOverlay<ProductOverlayItem> overlay, ProductOverlayItem item)
				{
					// Fetch the aisle section the given product collection resides in.
					return (item != null) ? item.getTitle() : null;
				}

				@Override
				public CharSequence getLocation(
					ItemizedOverlay<ProductOverlayItem> overlay, ProductOverlayItem item)
				{
					// Fetch the "aisle" name that the 1st product in the given collection resides in.
					Section section = null;
					if (item != null) {
						List<Product> productList = item.getProducts();
						if ((productList != null) && !productList.isEmpty()) {
							Product product = productList.get(0);
							if (product != null) {
								Set<Section> sectionSet = product.getSections();
								if (sectionSet != null) {
									for (Section nextSection : sectionSet) {
										section = nextSection;
										break;
									}
								}
							}
						}
					}
					return (section != null) ? section.getAisle() : null;
				}

				@Override
				public CharSequence getKeyword(
					ItemizedOverlay<ProductOverlayItem> overlay, ProductOverlayItem item)
				{
					// Only provide a keyword if there is just 1 product in the collection.
					if (item != null) {
						List<Product> productList = item.getProducts();
						if ((productList != null) && (productList.size() == 1)) {
							Product product = productList.get(0);
							if (product != null) {
								return product.getName();
							}
						}
					}
					return null;
				}

				@Override
				public android.widget.ListAdapter getListAdapter(
					ItemizedOverlay<ProductOverlayItem> overlay, ProductOverlayItem item)
				{
					// Return an adapter set up to use the given product collection.
					MapViewContainer.this.productListAdapter.clear();
					if (item != null) {
						MapViewContainer.this.productListAdapter.add(item.getProducts());
					}
					return MapViewContainer.this.productListAdapter;
				}
			};

			// Set up the pin/marker overlay view.
			this.productsOverlay = new ProductSetOverlay()
			{
				@Override
				protected boolean shouldCenterOnPin()
				{
					return MapViewContainer.this.isZoomOnPinEnabled;
				}

				@Override
				protected boolean onTap(int index)
				{
					// Fetch the proxy object.
					KrollProxy proxy = getProxy();

					// Fetch the index to the last selected map marker pin.
					// Notes:
					// - Will be -1 if nothing was selected before.
					// - We can't use getTappedIndex() because it alway returns -1 when
					//   the shouldCenterOnPin() method returns false. (Aisle411 bug?)
					int lastIndex = MapViewContainer.this.lastSelectedPinIndex;

					// Update the last selected marker pin index to the given one.
					// This will raise a "didItemDeselected" event if selection has changed.
					updateLastSelectedPinIndexTo(index);

					// Traverse all items belonging to the marking pin index.
					Set<Section> sectionSet = getSections(index);
					if (sectionSet != null) {
						for (Section section : sectionSet) {
							// Validate.
							if (section == null) {
								continue;
							}

							// Raise an item selection/re-selection event.
							if (proxy != null) {
								KrollDict event = new KrollDict();
								event.put("item", section.getTitle());
								if ((lastIndex < 0) || (lastIndex != index)) {
									proxy.fireEvent("didItemSelected", event);
								} else {
									proxy.fireEvent("didItemReselected", event);
								}
							}

							// Update the map pin to use the "selected" drawable image.
							if (MapViewContainer.this.selectedPinDrawable != null) {
								MapViewContainer.this.productsOverlay.setSelectedSection(
										section, MapViewContainer.this.selectedPinDrawable);
							}
						}
						MapView mapView = getMapView();
						if (mapView != null) {
							mapView.invalidate();
						}
					}

					// Let the base class handle the pin selection.
					return super.onTap(index);
				}

				public void setProducts(Set<Product> products)
				{
					// Clear last selected map marker pin index.
					updateLastSelectedPinIndexTo(-1);

					// Display map markers for the given products.
					super.setProducts(products);
				}

				private void updateLastSelectedPinIndexTo(int index)
				{
					// Never let index be less than -1. (Flags that there is no selection.)
					if (index < -1) {
						index = -1;
					}

					// Fire events for any deselected marker pins.
					int lastIndex = MapViewContainer.this.lastSelectedPinIndex;
					KrollProxy proxy = getProxy();
					if ((proxy != null) && (lastIndex >= 0) && (lastIndex != index)) {
						Set<Section> sectionSet = getSections(lastIndex);
						if (sectionSet != null) {
							for (Section section : sectionSet) {
								if (section != null) {
									KrollDict event = new KrollDict();
									event.put("item", section.getTitle());
									proxy.fireEvent("didItemDeselected", event);
								}
							}
						}
					}

					// Update member variable with new selection index.
					MapViewContainer.this.lastSelectedPinIndex = index;
				}
			};
			this.productsOverlay.setDrawable(this.defaultPinDrawable);

			// Set up the main Aisle411 info layout view which hosts the map and overlays.
			// Note: We must create this view via an XML layout in order to display an "InformationBar"
			//       product/shopping slide-out list. There's no way to do this programmatically.
			try {
				String resourceName = "layout.ti_aisle411_informationbarlayout";
				int resourceId = org.appcelerator.titanium.util.TiRHelper.getResource(resourceName);
				this.infoBarLayout =
						(InformationBarLayout)activity.getLayoutInflater().inflate(resourceId, null);
			} catch (Exception ex) {
				ex.printStackTrace();
			}
			if (this.infoBarLayout == null) {
				this.infoBarLayout = new InformationBarLayout(activity);
			}
			this.infoBarLayout.setInformationBar(this.infoBar);

			// Work-around a bug in the slide-out overlay's header bar where it's text labels
			// won't auto-size when selecting product pins on the map, causing ellipsis to be shown.
			InformationBarHeader infoBarHeader = this.infoBarLayout.getInformationBarHeader();
			if (infoBarHeader != null) {
				class ViewWidthFixer {
					public void fixView(View view) {
						if (view != null) {
							ViewGroup.LayoutParams params = view.getLayoutParams();
							if (params != null) {
								if (params.width != ViewGroup.LayoutParams.WRAP_CONTENT) {
									params.width = ViewGroup.LayoutParams.WRAP_CONTENT;
									view.setLayoutParams(params);
								}
							}
						}
					}
				}
				ViewWidthFixer viewWidthFixer = new ViewWidthFixer();
				viewWidthFixer.fixView(infoBarHeader.getLocation());
				viewWidthFixer.fixView(infoBarHeader.getSection());
				viewWidthFixer.fixView(infoBarHeader.getKeyword());
			}

			// Wrap the root Aisle411 view.
			setNativeView(this.infoBarLayout);
		}

		@Override
		public void processProperties(KrollDict properties)
		{
			// Let the base class handle the creation properties first.
			super.processProperties(properties);

			// Fetch the map view.
			Activity activity = getProxy().getActivity();
			MapView mapView = this.infoBarLayout.getMapView();
			if (mapView == null) {
				return;
			}

			// Load a map using the given URL.
			Object value = properties.get(PropertyNames.URL);
			if (value instanceof String) {
				String url = (String)value;
				MapId mapId = MapId.fromUrl(url);
				if (mapId != null) {
					RasterMapBundleResult result = RasterMapBundleApi.getBundleResult(mapId.getVenueId());
					if (result != null) {
						// Display and set up the map.
						mapView.setBundle(result.getMapBundle());
						mapView.setRotationEnabled(false);
						mapView.addOverlay(this.productsOverlay);

						// Display the floor selection buttons if there is more than 1 floor.
						List<LevelInfo> levelList = result.getMapBundle().getLevelInfos();
						if ((levelList != null) && (levelList.size() > 1)) {
							mapView.setFloorSelectorControls(true);
						} else {
							mapView.setFloorSelectorControls(false);
						}

						// Redraw the map view with the above changes.
						this.infoBarLayout.invalidate();

						// Fire an event and indicating that the map was loaded.
						getProxy().fireEvent("load", new KrollDict());
					} else {
						Log.e(TAG, "Aisle411 failed to load map for url: " + url);
					}
				} else {
					Log.e(TAG, "Aisle411 was given invalid url: " + url);
				}
			}

			// Load a custom "unselected" marker pin image if provided.
			value = properties.get(PropertyNames.UNSELECTED_PIN_IMAGE);
			if (value != null) {
				TiDrawableReference drawableReference = null;
				if (value instanceof String) {
					drawableReference = TiDrawableReference.fromUrl(getProxy(), (String)value);
				} else {
					drawableReference = TiDrawableReference.fromObject(activity, value);
				}
				Drawable drawable = null;
				if (drawableReference != null) {
					drawable = drawableReference.getDensityScaledDrawable();
					if ((drawable instanceof BitmapDrawable) && (activity != null) &&
					    !drawableReference.isTypeResourceId())
					{
						drawable = new BitmapDrawable(
								activity.getResources(),
								((BitmapDrawable)drawable).getBitmap());
					}
				}
				if (drawable != null) {
					this.productsOverlay.setDrawable(drawable);
				}
			}

			// Load a custom "selected" marker pin image if provided.
			value = properties.get(PropertyNames.SELECTED_PIN_IMAGE);
			if (value != null) {
				TiDrawableReference drawableReference = null;
				if (value instanceof String) {
					drawableReference = TiDrawableReference.fromUrl(getProxy(), (String)value);
				} else {
					drawableReference = TiDrawableReference.fromObject(activity, value);
				}
				Drawable drawable = null;
				if (drawableReference != null) {
					drawable = drawableReference.getDensityScaledDrawable();
					if ((drawable instanceof BitmapDrawable) && (activity != null) &&
					    !drawableReference.isTypeResourceId())
					{
						drawable = new BitmapDrawable(
								activity.getResources(),
								((BitmapDrawable)drawable).getBitmap());
					}
				}
				this.selectedPinDrawable = drawable;
			}

			// Apply all given property settings.
			value = properties.get(PropertyNames.COMPASS_ENABLED);
			if (value instanceof Boolean) {
				if (mapView.getMapContoller() != null) {
					mapView.getMapContoller().setCompassEnabled(((Boolean)value).booleanValue());
				}
			}
			if (properties.containsKey(PropertyNames.FLOOR)) {
				logUnsupportedPropertyWarningFor(PropertyNames.FLOOR);
			}
			if (properties.containsKey(PropertyNames.LOGO_POSITION)) {
				logUnsupportedPropertyWarningFor(PropertyNames.LOGO_POSITION);
			}
			value = properties.get(PropertyNames.MAP_BACKGROUND_COLOR);
			if (value instanceof String) {
				setMapBackgroundColor((String)value);
			}
			value = properties.get(PropertyNames.ZOOM_BUTTONS_HIDDEN);
			if (value instanceof Boolean) {
				setZoomButtonsVisible(!((Boolean)value).booleanValue());
			} else {
				setZoomButtonsVisible(true);
			}
			value = properties.get(PropertyNames.ZOOM_ON_PIN_CLICK_ENABLED);
			if (value instanceof Boolean) {
				this.isZoomOnPinEnabled = ((Boolean)value).booleanValue();
			}
			value = properties.get(PropertyNames.SHOPPING_LIST_ENABLED);
			if (value instanceof Boolean) {
				this.isShoppingListEnabled = ((Boolean)value).booleanValue();
			}
		}

		@Override
		public void propertyChanged(String key, Object oldValue, Object newValue, KrollProxy proxy)
		{
			// Validate arguments.
			if ((key == null) || (proxy == null)) {
				return;
			}

			// Fetch the map view.
			MapView mapView = this.infoBarLayout.getMapView();
			if (mapView == null) {
				return;
			}

			// Handle the property change.
			if (key.equals(PropertyNames.COMPASS_ENABLED)) {
				if ((newValue instanceof Boolean) && (mapView.getMapContoller() != null)) {
					mapView.getMapContoller().setCompassEnabled(((Boolean)newValue).booleanValue());
				}
			} else if (key.equals(PropertyNames.FLOOR)) {
				logUnsupportedPropertyWarningFor(key);
			} else if (key.equals(PropertyNames.LOGO_POSITION)) {
				logUnsupportedPropertyWarningFor(key);
			} else if (key.equals(PropertyNames.MAP_BACKGROUND_COLOR)) {
				if (newValue instanceof String) {
					setMapBackgroundColor((String)newValue);
				}
			} else if (key.equals(PropertyNames.ZOOM_BUTTONS_HIDDEN)) {
				if (newValue instanceof Boolean) {
					setZoomButtonsVisible(!((Boolean)newValue).booleanValue());
				}
			} else if (key.equals(PropertyNames.ZOOM_ON_PIN_CLICK_ENABLED)) {
				if (newValue instanceof Boolean) {
					this.isZoomOnPinEnabled = ((Boolean)newValue).booleanValue();
				}
			} else {
				// This class does not handle the given property. Let the base class try to handle it.
				super.propertyChanged(key, oldValue, newValue, proxy);
			}
		}

		public MapView getMapView()
		{
			if (this.infoBarLayout != null) {
				return this.infoBarLayout.getMapView();
			}
			return null;
		}

		/**
		 * Displays the given collection of products as marker pins in the map view.
		 * Previously displayed products not in the given collection will be removed from the map.
		 * @param products
		 * Collection of products to be displayed.
		 * <p>
		 * Can be null or empty, in which case, all map markers will be removed from the map.
		 */
		public void showProducts(HashSet<Product> products)
		{
			// Create an empty collection if given null.
			// Used to clear all product pins from map.
			if (products == null) {
				products = new HashSet<Product>();
			}

			// Update the product pin overlay with the given collection.
			this.productsOverlay.setProducts(products);

			// Hide the slide-out product panel if hiding all product markers.
			if (products.size() <= 0) {
				this.infoBarLayout.hidePanel();
			}

			// Redraw the map with the new pins.
			MapView mapView = getMapView();
			if (mapView != null) {
				mapView.invalidate();
			}
		}

		/** Removes all product marker pins from the map. */
		public void hideAllProducts()
		{
			showProducts(null);
		}

		/**
		 * Scales the map view "relatively" based on its current scale.
		 * <p>
		 * For example, if map is currently 2x scaled and you pass a "2.0" scale factor
		 * to this method, then the map will be 4x scaled.
		 * <p>
		 * If given 1.0f, then the scale won't change.
		 * @param value Scale factor to be used.
		 */
		public void scaleBy(float value)
		{
			// Fetch the loaded map view.
			MapView mapView = getMapView();
			if (mapView == null) {
				return;
			}

			// Make sure given scale factor is valid.
			if (value <= 0.1f) {
				value = 0.1f;
			}

			// Apply the given scale factor, relative to the current scale factor.
			float zoomScale = mapView.getZoom() * value;
			float panX = mapView.getPanX();
			float panY = mapView.getPanY();
			mapView.setZoomAndPan(zoomScale, panX, panY);
		}

		private void logUnsupportedPropertyWarningFor(String propertyName)
		{
			if ((propertyName != null) && (propertyName.length() > 0)) {
				Log.w(TAG, "The 'MapView." + propertyName + "' property is not supported on Android.");
			}
		}

		private void setZoomButtonsVisible(boolean value)
		{
			// Fetch the map view.
			MapView mapView = getMapView();
			if (mapView == null) {
				return;
			}

			// Show/hide the map's onscreen zoom buttons.
			mapView.setBuiltInZoomControls(value);
			View buttonsView = mapView.getZoomButtonsView();
			if (buttonsView != null) {
				buttonsView.setVisibility(value ? View.VISIBLE : View.GONE);
			}
		}

		private void setMapBackgroundColor(String value)
		{
			// Validate argument.
			if (value == null) {
				return;
			}

			// Apply the given color to the map's background.
			this.infoBarLayout.setBackgroundColor(TiConvert.toColor(value));
		}
	}


	public MapViewProxy()
	{
		super();
	}

	@Override
	public TiUIView createView(Activity activity)
	{
		TiUIView view = new MapViewContainer(this, activity);
		view.getLayoutParams().autoFillsHeight = true;
		view.getLayoutParams().autoFillsWidth = true;
		return view;
	}

	@Kroll.method
	public void clear()
	{
		Log.w(TAG, "The MapView.clear() method is not supported on Android.");
	}

	@Kroll.method
	public void zoomIn()
	{
		Activity activity = getActivity();
		if (activity != null) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					MapViewContainer view = getMapViewContainer();
					if (view != null) {
						view.scaleBy(2.0f);
					}
				}
			});
		}
	}

	@Kroll.method
	public void zoomOut()
	{
		Activity activity = getActivity();
		if (activity != null) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					MapViewContainer view = getMapViewContainer();
					if (view != null) {
						view.scaleBy(0.5f);
					}
				}
			});
		}
	}

	@Kroll.method
	public void redraw()
	{
		Activity activity = getActivity();
		if (activity != null) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					TiUIView uiView = peekView();
					if (uiView != null) {
						View view = uiView.getNativeView();
						if (view != null) {
							view.invalidate();
						}
					}
				}
			});
		}
	}

	@Kroll.method
	public void redrawOverlay(KrollDict settings)
	{
		// Validate.
		if (settings == null) {
			throw new IllegalArgumentException("Argument cannot be null.");
		}

		// Fetch an array of products from the given settings.
		HashMap[] productArray = null;
		{
			Object value = settings.get("products");
			if (value instanceof HashMap[]) {
				productArray = (HashMap[])value;
			} else if (value instanceof Object[]) {
				Object[] objectArray = (Object[])value;
				productArray = new HashMap[objectArray.length];
				for (int index = 0; index < objectArray.length; index++) {
					value = objectArray[index];
					productArray[index] = (value instanceof HashMap) ? (HashMap)value : null;
				}
			} else if (value != null) {
				throw new IllegalArgumentException("'products' must be an array of dictionaries.");
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'products' entry.");
			}
		}

		// Fetch an array of venue items from the given settings.
		HashMap[] venueItemArray = null;
		{
			Object value = settings.get("venueItems");
			if (value instanceof HashMap[]) {
				venueItemArray = (HashMap[])value;
			} else if (value instanceof Object[]) {
				Object[] objectArray = (Object[])value;
				venueItemArray = new HashMap[objectArray.length];
				for (int index = 0; index < objectArray.length; index++) {
					value = objectArray[index];
					venueItemArray[index] = (value instanceof HashMap) ? (HashMap)value : null;
				}
			} else if (value != null) {
				throw new IllegalArgumentException("'venueItems' must be an array of dictionaries.");
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'venueItems' entry.");
			}
		}

		// Do not continue if given array are not the same length.
		if (productArray.length != venueItemArray.length) {
			throw new IllegalArgumentException(
					"Given 'products' and 'venueItems' arrays must have the same number of elements.");
		}

		// Create a product set to be displayed as map pins from the given product array.
		final HashSet<Long> pickedUpItemIdSet = new HashSet<Long>();
		final ArrayList<BaseVenueItem> venueItemList = new ArrayList<BaseVenueItem>(productArray.length);
		for (int index = 0; index < productArray.length; index++) {
			// Fetch the next product and venue item from the argument's given arrays.
			HashMap productSettings = productArray[index];
			HashMap venueItemSettings = venueItemArray[index];
			if ((productSettings == null) || (venueItemSettings == null)) {
				continue;
			}

			// Create a venue item object from the dictionary of venue item settings.
			BaseVenueItem venueItem = createVenueItemFrom(venueItemSettings);
			if (venueItem == null) {
				continue;
			}

			// Modify venue item to use the given product settings' name and id.
			Object value = productSettings.get("name");
			if (value instanceof String) {
				venueItem.setName((String)value);
			}
			value = productSettings.get("id");
			if (value instanceof Number) {
				venueItem.setId(((Number)value).intValue());
			}

			// If the product is flagged as "picked up", then store the product's ID to a set.
			// We'll check off this item from the slide-out overlay product list.
			value = productSettings.get("isPickedUp");
			if ((value instanceof Boolean) && ((Boolean)value).booleanValue()) {
				pickedUpItemIdSet.add(venueItem.getId());
			}

			// Add the venue item to the collection, to be displayed as a map marker later.
			venueItemList.add(venueItem);
		}

		// Add product markers to the map view.
		Activity activity = getActivity();
		if (activity != null) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					// Fetch the map view container. (Will be null if proxy was released.)
					MapViewContainer viewContainer = getMapViewContainer();
					if (viewContainer == null) {
						return;
					}

					// Fetch the map bundle. (Will be null if map not loaded.)
					MapBundle mapBundle = null;
					MapView mapView = viewContainer.getMapView();
					if (mapView != null) {
						mapBundle = mapView.getBundle();
					}
					if (mapBundle == null) {
						return;
					}

					// Convert the venue item collection to a product collection.
					HashSet<Product> productSet = new HashSet<Product>(venueItemList.size());
					if (venueItemList.size() > 0) {
						// Convert to a product collection with coordinates relative to loaded map.
						BaseVenueItem[] venueItemArray = venueItemList.toArray(new BaseVenueItem[0]);
						Product[] productArray = RasterMapUtility.convertToRasterMap(
								venueItemArray, mapBundle);

						// Remove products that are not in the map.
						if (productArray != null) {
							productArray = RasterMapUtility.removeProductsAndSectionsWithoutMapPoints(productArray);
						}

						// Add the array's products to the product set.
						if (productArray != null) {
							for (Product nextProduct : productArray) {
								// Validate.
								if (nextProduct == null) {
									continue;
								}

								// Check-off the product if it's in the "picked up" collection.
								if (pickedUpItemIdSet.contains((long)nextProduct.getId())) {
									nextProduct.setChecked(true);
								}

								// Add the product to the set.
								productSet.add(nextProduct);
							}
						}
					}

					// Display product markers on the map.
					viewContainer.showProducts(productSet);
				}
			});
		}
	}

	@Kroll.method
	public void fadeIn(KrollDict settings)
	{
		Log.w(TAG, "The MapView.fadeIn() method is not supported on Android.");
	}

	@Kroll.method
	public void reloadTiles()
	{
		redraw();
	}

	@Kroll.method
	public void search(KrollDict settings)
	{
		// Determine if map was configured to search from a list or the entire venue.
		boolean isShoppingListEnabled = false;
		{
			Object value = getProperty("shoppingListEnabled");
			if (value instanceof Boolean) {
				isShoppingListEnabled = ((Boolean)value).booleanValue();
			}
		}

		// Perform the search operation.
		if (isShoppingListEnabled) {
			searchForItemsInListUsing(settings);
		} else {
			searchForItemsUsing(settings);
		}
	}

	private void searchForItemsUsing(KrollDict settings)
	{
		// Validate.
		if (settings == null) {
			throw new IllegalArgumentException("Argument cannot be null.");
		}

		// Fetch venue ID from the given settings.
		long venueId;
		{
			Object value = settings.get("venueId");
			if (value instanceof Number) {
				venueId = ((Number)value).longValue();
			} else if (value instanceof String) {
				try {
					venueId = Long.parseLong((String)value);
				} catch (Exception ex) {
					throw new IllegalArgumentException(
							"'venueId' string must be in integer form. Given: '" +
							(String)value + "'");
				}
			} else if (value != null) {
				throw new IllegalArgumentException("'venueId' must be of type number or a numeric string.");
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'venueId'.");
			}
		}

		// Fetch search string.
		String searchTerm = TiConvert.toString(settings.get("searchTerm"), "");

		// Fetch page settings.
		int pageItemCount = TiConvert.toInt(settings.get("maxCount"), 25);
		if (pageItemCount < 1) {
			pageItemCount = 1;
		}
		int startPageIndex = TiConvert.toInt(settings.get("startingIndex"), 0);
		if (startPageIndex < 0) {
			startPageIndex = 0;
		}
		int endPageIndex = TiConvert.toInt(settings.get("endingIndex"), 0);
		if (endPageIndex < 0) {
			endPageIndex = 0;
		} else if (endPageIndex < startPageIndex) {
			endPageIndex = startPageIndex;
		}

		// Fetch callback.
		final KrollFunction callback;
		{
			Object value = settings.get("callback");
			if (value instanceof KrollFunction) {
				callback = (KrollFunction)value;
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'callback'.");
			}
		}

		// Fetch the application context.
		Context context = TiApplication.getInstance();
		if (context == null) {
			return;
		}

		// Set up the search using the above settings.
		// Note: We must convert JavaScript's 0-based page indexes to 1-based for SearchPager.
		SearchPager pager = new SearchPager(
				startPageIndex + 1, ((endPageIndex - startPageIndex) + 1) * pageItemCount);
		SearchString searchString = new SearchString(searchTerm, false);
		VenueItemsSearch searchSettings = new VenueItemsSearch(venueId, searchString, pager);

		// Perform the requested search.
		MapServices mapServices = new MapServices(context);
		mapServices.search(searchSettings, new MapServices.VenueItemsSearchListener()
		{
			@Override
			public void onReceivedSearchResult(VenueItemsResult result)
			{
				// Fetch the proxy's kroll object.
				KrollObject krollObject = getKrollObject();
				if (krollObject == null) {
					return;
				}

				// Create an unknown error result if given null. (Should never happen.)
				if (result == null) {
					Aisle411Error error = new Aisle411Error(
							Aisle411Error.UNKOWN, "Unknown error occurred.");
					result = new VenueItemsResult(0, error);
				}

				// Invoke the given JavaScript callback with the result of the async operation.
				KrollDict event = new KrollDict();
				if (result.hasError()) {
					// An error occurred. Copy its error message to the event table.
					String message = null;
					Aisle411Error error = result.getError();
					if (error != null) {
						message = error.getDescription();
					}
					if (message == null) {
						message = "Unknown error occurred";
					}
					event.put("error", message);
				} else {
					// Copy the received items found in the search to an array of dictionaries.
					KrollDict[] copiedItems;
					BaseVenueItem[] receivedItems = result.getVenueItems();
					if (receivedItems != null) {
						copiedItems = new KrollDict[receivedItems.length];
						for (int index = 0; index < receivedItems.length; index++) {
							copiedItems[index] = createDictionaryFrom(receivedItems[index]);
						}
					} else {
						copiedItems = new KrollDict[0];
					}
					event.put("venueItems", copiedItems);
				}
				callback.callAsync(krollObject, event);
			}
		});
	}

	private void searchForItemsInListUsing(KrollDict settings)
	{
		// Validate.
		if (settings == null) {
			throw new IllegalArgumentException("Argument cannot be null.");
		}

		// Fetch venue ID from the given settings.
		long venueId;
		{
			Object value = settings.get("venueId");
			if (value instanceof Number) {
				venueId = ((Number)value).longValue();
			} else if (value instanceof String) {
				try {
					venueId = Long.parseLong((String)value);
				} catch (Exception ex) {
					throw new IllegalArgumentException(
							"'venueId' string must be in integer form. Given: '" +
							(String)value + "'");
				}
			} else if (value != null) {
				throw new IllegalArgumentException("'venueId' must be of type number or a numeric string.");
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'venueId'.");
			}
		}

		// Fetch search string.
		String searchTerm = TiConvert.toString(settings.get("name"), "");

		// Populate search list with given settings' array of product dictionaries.
		SearchList searchList = new SearchList("");
		{
			Object value = settings.get("products");
			if (value instanceof Object[]) {
				Object[] objectArray = (Object[])value;
				for (int index = 0; index < objectArray.length; index++) {
					value = objectArray[index];
					if (value instanceof HashMap) {
						value = ((HashMap)value).get("sectionCode");
						if (value instanceof String) {
							searchList.addItem(new TermSearchItem((String)value));
						}
					}
				}
			} else if (value != null) {
				throw new IllegalArgumentException("'products' must be an array of dictionaries.");
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'products' entry.");
			}
		}

		// Fetch callback.
		final KrollFunction callback;
		{
			Object value = settings.get("callback");
			if (value instanceof KrollFunction) {
				callback = (KrollFunction)value;
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'callback'.");
			}
		}

		// Fetch the application context.
		Context context = TiApplication.getInstance();
		if (context == null) {
			return;
		}

		// Perform the requested search.
		MapServices mapServices = new MapServices(context);
		VenueItemsListSearch searchSettings = new VenueItemsListSearch(venueId, searchList);
		mapServices.search(searchSettings, new MapServices.VenueItemsListSearchListener()
		{
			@Override
			public void onReceivedSearchResult(VenueItemsListResult result)
			{
				// Fetch the proxy's kroll object.
				KrollObject krollObject = getKrollObject();
				if (krollObject == null) {
					return;
				}

				// Create an unknown error result if given null. (Should never happen.)
				if (result == null) {
					Aisle411Error error = new Aisle411Error(
							Aisle411Error.UNKOWN, "Unknown error occurred.");
					result = new VenueItemsListResult(0, error);
				}

				// Invoke the given JavaScript callback with the result of the async operation.
				KrollDict event = new KrollDict();
				if (result.hasError()) {
					// An error occurred. Copy its error message to the event table.
					String message = null;
					Aisle411Error error = result.getError();
					if (error != null) {
						message = error.getDescription();
					}
					if (message == null) {
						message = "Unknown error occurred";
					}
					event.put("error", message);
				} else {
					// Copy the received items found in the search to an array of dictionaries.
					KrollDict[] copiedItems;
					BaseVenueItem[] receivedItems = result.getItems();
					if (receivedItems != null) {
						copiedItems = new KrollDict[receivedItems.length];
						for (int index = 0; index < receivedItems.length; index++) {
							copiedItems[index] = createDictionaryFrom(receivedItems[index]);
						}
					} else {
						copiedItems = new KrollDict[0];
					}
					event.put("venueItems", copiedItems);
				}
				callback.callAsync(krollObject, event);
			}
		});
	}

	private MapViewContainer getMapViewContainer()
	{
		TiUIView uiView = peekView();
		if (uiView instanceof MapViewContainer) {
			return (MapViewContainer)uiView;
		}
		return null;
	}

	private static KrollDict createDictionaryFrom(BaseVenueItem item)
	{
		KrollDict dictionary = new KrollDict();
		if (item != null) {
			dictionary.put("id", item.getId());
			dictionary.put("name", item.getName());
			dictionary.put("brand", item.getBrand());
			dictionary.put("venueItemTypeName", item.getClass().getName());
			if (item instanceof FeaturedVenueItem) {
				FeaturedVenueItem featuredItem = (FeaturedVenueItem)item;
				dictionary.put("description", featuredItem.getDescription());
				dictionary.put("couponUrl", featuredItem.getCouponUrl());
				dictionary.put("upcList", new String[] { featuredItem.getGTIN() });
			} else if (item instanceof GTINsVenueItem) {
				GTINsVenueItem gtinsItem = (GTINsVenueItem)item;
				dictionary.put("quantity", gtinsItem.getQuantity());
				dictionary.put("upcList", gtinsItem.getGTINs());
			} else if (item instanceof GTINVenueItem) {
				GTINVenueItem gtinItem = (GTINVenueItem)item;
				dictionary.put("description", gtinItem.getDescription());
				dictionary.put("upcList", new String[] { gtinItem.getGTIN() });
			} else if (item instanceof TermVenueItem) {
				TermVenueItem termItem = (TermVenueItem)item;
				dictionary.put("category", termItem.getCategoryName());
				dictionary.put("searchTerm", termItem.getSearchTerm());
			}
			{
				VenueSection[] sectionArray = item.getSections();
				KrollDict[] dictionaryArray = null;
				if (sectionArray != null) {
					dictionaryArray = new KrollDict[sectionArray.length];
					for (int index = 0; index < sectionArray.length; index++) {
						dictionaryArray[index] = createDictionaryFrom(sectionArray[index]);
					}
				} else {
					dictionaryArray = new KrollDict[0];
				}
				dictionary.put("sections", dictionaryArray);
				dictionary.put("section", (dictionaryArray.length > 0) ? dictionaryArray[0] : null);
			}
		}
		return dictionary;
	}

	private static BaseVenueItem createVenueItemFrom(HashMap settings)
	{
		// Validate argument.
		if (settings == null) {
			return null;
		}

		// Fetch venue item type from given settings.
		String className = null;
		Object value = settings.get("venueItemTypeName");
		if (value instanceof String) {
			className = (String)value;
		}
		if ((className == null) || (className.length() <= 0)) {
			return null;
		}

		// Create the venue item now that the type is known.
		BaseVenueItem venueItem = null;
		if (className.equals(FeaturedVenueItem.class.getName())) {
			FeaturedVenueItem featuredItem = new FeaturedVenueItem();
			featuredItem.setType(BaseVenueItem.TYPE_FEATURED);
			value = settings.get("description");
			if (value instanceof String) {
				featuredItem.setDescription((String)value);
			}
			value = settings.get("couponUrl");
			if (value instanceof String) {
				featuredItem.setCouponUrl((String)value);
			}
			value = settings.get("upcList");
			if ((value instanceof Object[]) && (((Object[])value).length > 0)) {
				value = ((Object[])value)[0];
				if (value instanceof String) {
					featuredItem.setGTIN((String)value);
				}
			}
			venueItem = featuredItem;
		} else if (className.equals(GTINsVenueItem.class.getName())) {
			GTINsVenueItem gtinsItem = new GTINsVenueItem();
			gtinsItem.setType(BaseVenueItem.TYPE_GTINS);
			value = settings.get("quantity");
			if (value instanceof Number) {
				gtinsItem.setQuantity(((Number)value).intValue());
			}
			value = settings.get("upcList");
			if (value instanceof String[]) {
				gtinsItem.setGTINs((String[])value);
			}
			venueItem = gtinsItem;
		} else if (className.equals(GTINVenueItem.class.getName())) {
			GTINVenueItem gtinItem = new GTINVenueItem();
			gtinItem.setType(BaseVenueItem.TYPE_GTIN);
			value = settings.get("description");
			if (value instanceof String) {
				gtinItem.setDescription((String)value);
			}
			value = settings.get("upcList");
			if ((value instanceof Object[]) && (((Object[])value).length > 0)) {
				value = ((Object[])value)[0];
				if (value instanceof String) {
					gtinItem.setGTIN((String)value);
				}
			}
			venueItem = gtinItem;
		} else if (className.equals(TermVenueItem.class.getName())) {
			TermVenueItem termItem = new TermVenueItem();
			termItem.setType(BaseVenueItem.TYPE_TERM);
			value = settings.get("category");
			if (value instanceof String) {
				termItem.setCategoryName((String)value);
			}
			value = settings.get("searchTerm");
			if (value instanceof String) {
				termItem.setSearchTerm((String)value);
			}
			venueItem = termItem;
		}
		if (venueItem == null) {
			return null;
		}

		// Finishing copying the base venue item settings.
		value = settings.get("id");
		if (value instanceof Number) {
			venueItem.setId(((Number)value).intValue());
		}
		value = settings.get("name");
		if (value instanceof String) {
			venueItem.setName((String)value);
		}
		value = settings.get("brand");
		if (value instanceof String) {
			venueItem.setBrand((String)value);
		}
		value = settings.get("sections");
		if (value instanceof Object[]) {
			Object[] objectArray = (Object[])value;
			ArrayList<VenueSection> sectionList = new ArrayList<VenueSection>(objectArray.length);
			for (Object nextObject : objectArray) {
				if (nextObject instanceof HashMap) {
					VenueSection section = createVenueSectionFrom((HashMap)nextObject);
					if (section != null) {
						sectionList.add(section);
					}
				}
			}
			if (sectionList.size() > 0) {
				venueItem.setSections(sectionList.toArray(new VenueSection[0]));
			}
		}

		// Return the venue item configured using the given settings.
		return venueItem;
	}

	private static KrollDict createDictionaryFrom(VenueSection section)
	{
		KrollDict dictionary = new KrollDict();
		if (section != null) {
			dictionary.put("mapPointId", section.getMapId());
			dictionary.put("aisle", section.getLocation());
			dictionary.put("section", section.getSubLocation());
			dictionary.put("category", section.getCategory());
			dictionary.put("locationId", section.getLocationId());
			dictionary.put("subLocationId", section.getSubLocationId());
		}
		return dictionary;
	}

	private static VenueSection createVenueSectionFrom(HashMap settings)
	{
		if (settings == null) {
			return null;
		}

		VenueSection section = new VenueSection();
		Object value = settings.get("category");
		if (value instanceof String) {
			section.setCategory((String)value);
		}
		value = settings.get("aisle");
		if (value instanceof String) {
			section.setLocation((String)value);
		}
		value = settings.get("locationId");
		if (value instanceof Number) {
			section.setLocationId(((Number)value).intValue());
		}
		value = settings.get("section");
		if (value instanceof String) {
			section.setSubLocation((String)value);
		}
		value = settings.get("subLocationId");
		if (value instanceof Number) {
			section.setSubLocationId(((Number)value).intValue());
		}
		value = settings.get("mapPointId");
		if (value instanceof Number) {
			section.setMapId(((Number)value).intValue());
		}
		return section;
	}

	private static KrollDict createDictionaryFrom(Product product)
	{
		KrollDict dictionary = new KrollDict();
		if (product != null) {
			dictionary.put("id", product.getId());
			dictionary.put("name", product.getName());
			dictionary.put("isPickedUp", product.isChecked());
			dictionary.put("couponUrl", product.getCouponUrl());
			{
				String typeName = "unknown";
				switch (product.getType()) {
					case FEATURED:
						typeName = "featured";
						break;
					case GENERIC:
						typeName = "generic";
						break;
					case UNFOUND:
						typeName = "unfound";
						break;
				}
				dictionary.put("productType", typeName);
			}
			{
				Set<Section> sectionSet = product.getSections();
				KrollDict[] dictionaryArray = null;
				if (sectionSet != null) {
					dictionaryArray = new KrollDict[sectionSet.size()];
					int index = 0;
					for (Section section : sectionSet) {
						dictionaryArray[index] = createDictionaryFrom(section);
						index++;
					}
				} else {
					dictionaryArray = new KrollDict[0];
				}
				dictionary.put("sections", dictionaryArray);
			}
		}
		return dictionary;
	}

	private static KrollDict createDictionaryFrom(Section section)
	{
		KrollDict dictionary = new KrollDict();
		if (section != null) {
			dictionary.put("aisle", section.getAisle());
			dictionary.put("locationId", section.getLocation());
			dictionary.put("subLocationId", section.getSublocation());
			dictionary.put("mapPointId", section.getMapId());
			dictionary.put("title", section.getTitle());
		}
		return dictionary;
	}
}
