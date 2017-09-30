/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2017 by Axway, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
package ti.aisle411;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import com.aisle411.sdk.core.error.Aisle411Error;
import com.aisle411.sdk.core.request.search.RasterMapSearch;
import com.aisle411.sdk.core.request.search.VenueItemsListSearch;
import com.aisle411.sdk.core.request.search.VenueItemsSearch;
import com.aisle411.sdk.core.result.RasterMapResult;
import com.aisle411.sdk.core.result.VenueItemsListResult;
import com.aisle411.sdk.core.result.VenueItemsResult;
import com.aisle411.sdk.rastermap.bundle.RasterMapBundleResult;
import com.aisle411.sdk.rastermap.RasterMapBundleApi;
import com.aisle411.sdk.venuekit.VenueKitApi;
import java.util.ArrayList;


/** Provides easy access to Aisle411 features such as asynchronous searches. */
class MapServices
{
	/** Provides the result of an async map search when passed to the MapServices.search() method. */
	public interface RasterMapSearchListener
	{
		void onReceivedSearchResult(RasterMapBundleResult result);
	}

	/** Provides the result of an async items search when passed to the MapServices.search() method. */
	public interface VenueItemsSearchListener
	{
		void onReceivedSearchResult(VenueItemsResult result);
	}

	/** Provides the result of an async items search when passed to the MapServices.search() method. */
	public interface VenueItemsListSearchListener
	{
		void onReceivedSearchResult(VenueItemsListResult result);
	}


	/** The Android log tag name to be used by this class. */
	private static final String TAG = "MapServices";

	/** Broadcast receiver used to receive the Aisle411 library's events. */
	private static MapBroadcastReceiver broadcastReceiver;

	/** Reference to the application context. */
	private Context context;


	/**
	 * Creates a new object which provides Aisle411 services such as search capabilities.
	 * @param context Context needed to access application features. Cannot be null.
	 */
	public MapServices(Context context)
	{
		// Validate.
		if (context == null) {
			throw new IllegalArgumentException();
		}

		// Keep a reference to the application context.
		context = context.getApplicationContext();
		this.context = context;

		// Set up broadcast receiver, if not done already.
		// Will receive all of the library's VenueKit broadcasted events.
		synchronized (MapServices.class) {
			if (MapServices.broadcastReceiver == null) {
				MapBroadcastReceiver receiver = new MapBroadcastReceiver();
				IntentFilter intentFilter = new IntentFilter();
				intentFilter.addAction(RasterMapBundleApi.PARSE_BUNDLE_RESULT);
				intentFilter.addAction(VenueKitApi.SEARCH_RASTER_MAP_RESULT);
				intentFilter.addAction(VenueKitApi.SEARCH_VENUE_ITEMS_LIST_RESULT);
				intentFilter.addAction(VenueKitApi.SEARCH_VENUE_ITEMS_RESULT);
				context.registerReceiver(receiver, intentFilter);
				MapServices.broadcastReceiver = receiver;
			}
		}
	}

	/**
	 * Searches for a facility map asynchronously using the given search settings.
	 * @param searchSettings
	 * Provides search information such as "venue ID".
	 * <p>
	 * Can be null, in which case the method will no-op.
	 * @param listener
	 * The listener to be invoked with the results of the async search operation.
	 * <p>
	 * Can be null, in which case the method will no-op.
	 */
	public void search(
		RasterMapSearch searchSettings, final MapServices.RasterMapSearchListener listener)
	{
		// Validate arguments.
		if ((searchSettings == null) || (listener == null)) {
			return;
		}

		// Start an asynchronous search.
		// Note: Returned request ID is used to identify response from intents having same action name.
		final long venueId = searchSettings.getVenueId();
		final long requestId = VenueKitApi.startSearch(searchSettings);

		// Add a response listener to the broadcast receiver.
		MapServices.broadcastReceiver.addListener(new MapBroadcastReceiver.IntentListener()
		{
			@Override
			public void onReceived(Intent intent)
			{
				// Validate argument.
				if (intent == null) {
					return;
				}

				// Fetch the intent action.
				String actionName = intent.getAction();
				if (actionName == null) {
					return;
				}

				// Handle the received intent.
				RasterMapBundleResult bundleResult = null;
				if (actionName.equals(VenueKitApi.SEARCH_RASTER_MAP_RESULT)) {
					// We've received a search result for one map.
					// Check if it's the map this listener has been waiting for.
					if (intent.getLongExtra(VenueKitApi.REQUEST_ID_EXTRA, -1L) == requestId) {
						RasterMapResult searchResult = intent.getParcelableExtra(VenueKitApi.RESULT_EXTRA);
						if (searchResult == null) {
							// Generate an error response if given null. (Should never happen.)
							Aisle411Error error = new Aisle411Error(
									Aisle411Error.UNKOWN,
									"Intent 'SEARCH_RASTER_MAP_RESULT' returned a null reference.");
							bundleResult = new RasterMapBundleResult(venueId, error);
						} else if (searchResult.hasError()) {
							// The search failed. Bubble up the error code and error message.
							bundleResult = new RasterMapBundleResult(venueId, searchResult.getError());
						} else {
							// The search succeeded. Attempt to fetch a map bundle that's already parsed.
							bundleResult = RasterMapBundleApi.getBundleResult(searchResult.getVenueId());
							if (bundleResult == null) {
								// The downloaded map has not been parsed yet. Do so now asynchronously.
								// The below method will trigger a PARSE_BUNDLE_RESULT intent later.
								// Note: This will always happen the first time a map has been downloaded.
								RasterMapBundleApi.parseForBundle(searchResult);
							}
						}
					}
				} else if (actionName.equals(RasterMapBundleApi.PARSE_BUNDLE_RESULT)) {
					// Library has finished parsing the downloaded map.
					// Check if this is the map this listener is waiting for.
					if (intent.getLongExtra(RasterMapBundleApi.VENUE_ID_EXTRA, -1L) == venueId) {
						// Fetch the parsed bundle.
						bundleResult = RasterMapBundleApi.getBundleResult(venueId);
						if (bundleResult == null) {
							// Generate an error if map parsing has failed.
							Aisle411Error error = new Aisle411Error(
									Aisle411Error.UNKOWN,
									"Intent 'PARSE_BUNDLE_RESULT' returned a null reference.");
							bundleResult = new RasterMapBundleResult(venueId, error);
						}
					}
				}

				// Do not continue if the intent was not handled.
				if (bundleResult == null) {
					return;
				}

				// Invoke the given listener with the received search result.
				listener.onReceivedSearchResult(bundleResult);

				// The intent has been handled. Remove this listener from the broadcast receiver.
				MapServices.broadcastReceiver.removeListener(this);
			}
		});
	}

	/**
	 * Asynchronously searches for items/products within one venue map using the given search settings.
	 * @param searchSettings
	 * Provides product search information.
	 * <p>
	 * Can be null, in which case the method will no-op.
	 * @param listener
	 * The listener to be invoked with the results of the async search operation.
	 * <p>
	 * Can be null, in which case the method will no-op.
	 */
	public void search(
		VenueItemsSearch searchSettings, final VenueItemsSearchListener listener)
	{
		// Validate arguments.
		if ((searchSettings == null) || (listener == null)) {
			return;
		}

		// Start an asynchronous search.
		// Note: Returned request ID is used to identify response from intents having same action name.
		final long requestId = VenueKitApi.startSearch(searchSettings);

		// Add a response listener to the broadcast receiver.
		MapServices.broadcastReceiver.addListener(new MapBroadcastReceiver.IntentListener()
		{
			@Override
			public void onReceived(Intent intent)
			{
				// Validate argument.
				if (intent == null) {
					return;
				}

				// Ignore the intent if it's not the one this listener should be handling.
				if (VenueKitApi.SEARCH_VENUE_ITEMS_RESULT.equals(intent.getAction()) == false) {
					return;
				}
				if (intent.getLongExtra(VenueKitApi.REQUEST_ID_EXTRA, -1L) != requestId) {
					return;
				}

				// Invoke the given listener with the received search results.
				VenueItemsResult result = intent.getParcelableExtra(VenueKitApi.RESULT_EXTRA);
				if (result == null) {
					Aisle411Error error = new Aisle411Error(
							Aisle411Error.UNKOWN,
							"Intent 'SEARCH_VENUE_ITEMS_RESULT' returned a null reference.");
					result = new VenueItemsResult(requestId, error);
				}
				listener.onReceivedSearchResult(result);

				// The intent has been handled. Remove this listener from the broadcast receiver.
				MapServices.broadcastReceiver.removeListener(this);
			}
		});
	}

	/**
	 * Asynchronously searches for items/products that exist in one venue map,
	 * inclusively filtered using the item list stored in the search settings.
	 * @param searchSettings
	 * Provides product search information and a search list.
	 * <p>
	 * Can be null, in which case the method will no-op.
	 * @param listener
	 * The listener to be invoked with the results of the async search operation.
	 * <p>
	 * Can be null, in which case the method will no-op.
	 */
	public void search(
		VenueItemsListSearch searchSettings, final VenueItemsListSearchListener listener)
	{
		// Validate arguments.
		if ((searchSettings == null) || (listener == null)) {
			return;
		}

		// Start an asynchronous search.
		// Note: Returned request ID is used to identify response from intents having same action name.
		final long requestId = VenueKitApi.startSearch(searchSettings);

		// Add a response listener to the broadcast receiver.
		MapServices.broadcastReceiver.addListener(new MapBroadcastReceiver.IntentListener()
		{
			@Override
			public void onReceived(Intent intent)
			{
				// Validate argument.
				if (intent == null) {
					return;
				}

				// Ignore the intent if it's not the one this listener should be handling.
				if (VenueKitApi.SEARCH_VENUE_ITEMS_LIST_RESULT.equals(intent.getAction()) == false) {
					return;
				}
				if (intent.getLongExtra(VenueKitApi.REQUEST_ID_EXTRA, -1L) != requestId) {
					return;
				}

				// Invoke the given listener with the received search results.
				VenueItemsListResult result = intent.getParcelableExtra(VenueKitApi.RESULT_EXTRA);
				if (result == null) {
					Aisle411Error error = new Aisle411Error(
							Aisle411Error.UNKOWN,
							"Intent 'SEARCH_VENUE_ITEMS_LIST_RESULT' returned a null reference.");
					result = new VenueItemsListResult(requestId, error);
				}
				listener.onReceivedSearchResult(result);

				// The intent has been handled. Remove this listener from the broadcast receiver.
				MapServices.broadcastReceiver.removeListener(this);
			}
		});
	}


	/**
	 * Broadcast receiver only intended to be used by the MapServices class.
	 * Passes received intents to listener given to the addListener() method.
	 */
	private static class MapBroadcastReceiver extends BroadcastReceiver
	{
		/**
		 * Listener which gets invoked when the BroadcastReceiver.onReceive() method gets called.
		 * Instances are expected to be passed to the addListener() and removeListener() methods.
		 */
		public interface IntentListener
		{
			void onReceived(Intent intent);
		}

		/** Stores a collection of all listeners added to the broadcast receiver. */
		ArrayList<IntentListener> listenerCollection;


		/** Creates a new broadcast receiver. */
		public MapBroadcastReceiver()
		{
			this.listenerCollection = new ArrayList<IntentListener>(8);
		}

		/**
		 * Adds the given intent listener to the collection,
		 * to be invoked when the broadcast receiver receives an intent.
		 * <p>
		 * You cannot add the same listener instance twice to the receiver's collection (will no-op).
		 * @param listener The listener to be added. Can be null, in which case, this method will no-op.
		 */
		public synchronized void addListener(IntentListener listener)
		{
			// Validate argument.
			if (listener == null) {
				return;
			}

			// Do not continue if listener has already been added.
			if (this.listenerCollection.contains(listener)) {
				return;
			}

			// Add the listener to the collection.
			this.listenerCollection.add(listener);
		}

		/**
		 * Removes the given intent listener from the broadcast receiver's collection by reference.
		 * @param listener Listener to be removed by reference. Can be null.
		 */
		public synchronized void removeListener(IntentListener listener)
		{
			if (listener != null) {
				this.listenerCollection.remove(listener);
			}
		}

		/**
		 * Determines if the given listener reference exists in the broadcast receiver's collection.
		 * @param listener Listener to search for by reference.
		 * @return
		 * Returns true if the given listener has been added and exists in the listener collection.
		 * <p>
		 * Returns false if the listener reference has not been added or has been removed.
		 */
		public synchronized boolean hasListener(IntentListener listener)
		{
			return this.listenerCollection.contains(listener);
		}

		/**
		 * Called when a new intent has been sent to this broadcast receiver.
		 * @param context The context this broadcast receiver is running on.
		 * @param intent The intent that was broadcasted to this receiver.
		 */
		@Override
		public void onReceive(Context context, Intent intent)
		{
			// Validate arguments.
			if ((context == null) || (intent == null)) {
				return;
			}

			// Clone the listener collection to be iterated down below.
			// Note: We need to do this in case listeners are added/removed to/from the collection
			//       while invoking listener. Prevents breaking the iterator.
			ArrayList<IntentListener> clonedListenerCollection = null;
			synchronized (this) {
				if (this.listenerCollection.size() > 0) {
					clonedListenerCollection = (ArrayList<IntentListener>)this.listenerCollection.clone();
				}
			}
			if (clonedListenerCollection == null) {
				return;
			}

			// Pass the received intent to all listeners added to this broadcast receiver.
			for (IntentListener listener : clonedListenerCollection) {
				// Validate.
				if (listener == null) {
					continue;
				}

				// Check if next listener in the cloned collection is no longer in the main collection.
				// Can happen if removeListener() was called while a listener was invoked down below.
				if (hasListener(listener) == false) {
					continue;
				}

				// Invoke the listener.
				try {
					listener.onReceived(intent);
				} catch (Exception ex) {
					ex.printStackTrace();
				}
			}
		}
	}
}
