/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2017 by Axway, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
package ti.aisle411;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import com.aisle411.sdk.core.error.Aisle411Error;
import com.aisle411.sdk.core.request.search.RasterMapSearch;
import com.aisle411.sdk.rastermap.bundle.RasterMapBundleResult;
import com.aisle411.sdk.rastermap.RasterMapBundleApi;
import com.aisle411.sdk.venuekit.VenueKitApi;
import org.appcelerator.kroll.annotations.Kroll;
import org.appcelerator.kroll.common.Log;
import org.appcelerator.kroll.common.TiConfig;
import org.appcelerator.kroll.KrollDict;
import org.appcelerator.kroll.KrollFunction;
import org.appcelerator.kroll.KrollModule;
import org.appcelerator.titanium.TiApplication;


@Kroll.module(name="Aisle411Module", id="ti.aisle411")
public class Aisle411Module extends KrollModule
{
	/** The Android log tag name to be used by this class. */
	private static final String TAG = "Aisle411Module";

	/** Reference to the application context. */
	private static TiApplication application;


	/** Creates a new module instance. Expected to be upon JavaScript require(). */
	public Aisle411Module()
	{
		super();
	}

	/**
	 * Called the first time JavaScript require() for this module was called.
	 * Will only be called once for the lifetime of the application.
	 * @param application The main application context. Cannot be null.
	 */
	@Kroll.onAppCreate
	public static void onAppCreate(TiApplication application)
	{
		// Validate argument.
		if (application == null) {
			return;
		}

		// Do not continue if the application context has already been assigned.
		if (Aisle411Module.application != null) {
			return;
		}

		// Initialize this module and its libraries.
		Aisle411Module.application = application;
		VenueKitApi.init(application);
		RasterMapBundleApi.init(application);
	}

	/**
	 * Initializes Aisle411 library with "partnerId", "partnerSecret", and "host" settings.
	 * Must be called before accessing Aisle411 maps or else an exception will be thrown.
	 * <p>
	 * This method is expected to be called once upon startup.
	 * @param settings
	 * Dictionary which must provide settings "partnerId", "partnerSecret", and "host".
	 * Cannot be null or missing these entries or else an exception will be thrown.
	 */
	@Kroll.method
	public void setup(KrollDict settings)
	{
		// Validate.
		if (settings == null) {
			throw new IllegalArgumentException("Argument cannot be null.");
		}

		// Fetch partner ID from the given settings.
		long partnerId;
		{
			Object value = settings.get("partnerId");
			if (value instanceof Number) {
				partnerId = ((Number)value).longValue();
			} else if (value instanceof String) {
				try {
					partnerId = Long.parseLong((String)value);
				} catch (Exception ex) {
					throw new IllegalArgumentException(
							"'partnerId' string must be in integer form. Given: '" +
							(String)value + "'");
				}
			} else if (value != null) {
				throw new IllegalArgumentException("'partnerId' must be of type number or a numeric string.");
			} else {
				throw new IllegalArgumentException("Dictionary argument must provide 'partnerId'.");
			}
		}

		// Fetch partner secret from the given settings.
		String partnerSecret;
		{
			Object value = settings.get("partnerSecret");
			if (value instanceof String) {
				partnerSecret = (String)value;
			} else if (value != null) {
				throw new IllegalArgumentException("'partnerSecret' must be of type string.");
			} else {
				throw new IllegalArgumentException("Dictionary argument must provide 'partnerSecret'.");
			}
		}

		// Fetch host from the given settings.
		String hostTypeName;
		{
			Object value = settings.get("host");
			if (value instanceof String) {
				hostTypeName = (String)value;
			} else if (value != null) {
				throw new IllegalArgumentException("'host' must be of type string.");
			} else {
				throw new IllegalArgumentException("Dictionary argument must provide 'host'.");
			}
		}

		// Fetch this application's name and version string.
		String appName = null;
		String versionName = null;
		if (Aisle411Module.application != null) {
			try {
				// Fetch the localized app name. (This is the name the user sees on-screen.)
				ApplicationInfo appInfo = Aisle411Module.application.getApplicationInfo();
				PackageManager packageManager = Aisle411Module.application.getPackageManager();
				CharSequence charSequence = packageManager.getApplicationLabel(appInfo);
				if (charSequence != null) {
					appName = charSequence.toString();
				}

				// Fetch the application "versionName" string from the "AndroidManifest.xml".
				String packageName = Aisle411Module.application.getPackageName();
				PackageInfo packageInfo = packageManager.getPackageInfo(packageName, 0);
				versionName = packageInfo.versionName;
			} catch (Exception ex) {
			}
		}
		if (appName == null) {
			appName = "";
		}
		if (versionName == null) {
			versionName = "";
		}

		// Initialize library with the app's partner ID and secret keys.
		VenueKitApi.setup(partnerId, partnerSecret, appName, versionName, hostTypeName);
	}

	/**
	 * Asynchronously fetches a map using the given the settings.
	 * @param settings Diction of settings. Must contain "venueId" and "callback".
	 */
	@Kroll.method
	public void requestCachedRasterMap(KrollDict settings)
	{
		// Validate.
		if (settings == null) {
			throw new IllegalArgumentException("Argument cannot be null.");
		}

		// Fetch venue ID from the given settings.
		final long venueId;
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

		// Fetch callback from given settings.
		final KrollFunction callback;
		{
			Object value = settings.get("callback");
			if (value instanceof KrollFunction) {
				callback = (KrollFunction)value;
			} else {
				throw new IllegalArgumentException("Given dictionary must provide a 'callback'.");
			}
		}

		// Do not continue if setup() method hasn't been called yet.
		if (VenueKitApi.isSetup() == false) {
			throw new IllegalStateException("You must call Aisle411 module's setup() method first.");
		}

		// Attempt to fetch the requested map.
		RasterMapSearch searchSettings = new RasterMapSearch(venueId);
		MapServices mapServices = new MapServices(this.application);
		mapServices.search(searchSettings, new MapServices.RasterMapSearchListener()
		{
			@Override
			public void onReceivedSearchResult(RasterMapBundleResult result)
			{
				// Create an unknown error result if given null. (Should never happen.)
				if (result == null) {
					Aisle411Error error = new Aisle411Error(
							Aisle411Error.UNKOWN, "Unknown error occurred.");
					result = new RasterMapBundleResult(venueId, error);
				}

				// Invoke the given JavaScript callback with the result of the async operation.
				KrollDict event = new KrollDict();
				if (result.hasError()) {
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
					event.put("path", MapId.fromVenueId(venueId).getUrl());
				}
				callback.callAsync(Aisle411Module.this.getKrollObject(), event);
			}
		});
	}
}
