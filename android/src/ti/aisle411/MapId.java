/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2017 by Axway, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
package ti.aisle411;


/**
 * Class used to generate a custom map URL from a map venue ID and vice-versa.
 * Intended to replicate Aisle411 iOS library's behavior for JavaScript portability.
 * <p>
 * You can only create instances of this class via the static fromVenueId() and fromUrl() methods.
 */
class MapId
{
	private static final String MAP_URL_PREFIX = "aisle411://venue_id=";
	private String url;
	private long venueId;


	private MapId(String url, long venueId)
	{
		this.url = url;
		this.venueId = venueId;
	}

	public long getVenueId()
	{
		return this.venueId;
	}

	public String getUrl()
	{
		return this.url;
	}

	@Override
	public String toString()
	{
		return this.url;
	}

	public boolean equals(MapId object)
	{
		if (object == null) {
			return false;
		}
		return (this.venueId == object.venueId);
	}

	@Override
	public boolean equals(Object object)
	{
		if (object instanceof MapId) {
			return equals((MapId)object);
		}
		return false;
	}

	public static MapId fromVenueId(long value)
	{
		String url = MAP_URL_PREFIX + value;
		return new MapId(url, value);
	}

	public static MapId fromUrl(String url)
	{
		try {
			if ((url != null) && url.startsWith(MAP_URL_PREFIX)) {
				long venueId = Long.parseLong(url.substring(MAP_URL_PREFIX.length()));
				return new MapId(url, venueId);
			}
		} catch (Exception ex) {
		}
		return null;
	}
}
