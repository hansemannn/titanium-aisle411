/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2017 by Axway, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
package ti.aisle411;

import android.content.Context;
import android.graphics.Paint;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import com.aisle411.mapsdk.shopping.Product;
import java.util.ArrayList;


/**
 * Product list adapter intended to be returned by the Aisle411 InformationBar.getListAdapter() method.
 * Provides product names in the map's slide-out list overlay and an event listener for when
 * products/items get tapped on in the list.
 */
class MapProductListAdapter extends BaseAdapter
{
	/** Listener which gets invoked when a product item gets tapped on in a view. */
	public interface ItemTappedListener
	{
		void onTapped(Product product);
	}

	/** Context to be used to generate views in the getView() method. */
	private Context context;

	/** Listener to be invoked when a product has been tapped in the list. Can be null. */
	private ItemTappedListener itemTappedListener;

	/** Product collection that this adapter is wrapping. */
	private ArrayList<Product> list;


	/**
	 * Creates a new adapter for an Aisle411 product collection.
	 * @param context Context needed to create views. Cannot be null or else an exception will be thrown.
	 */
	public MapProductListAdapter(Context context)
	{
		if (context == null) {
			throw new IllegalArgumentException("Context cannot be null.");
		}

		this.context = context;
		this.list = new ArrayList<Product>();
	}

	public void setItemTappedListener(MapProductListAdapter.ItemTappedListener listener)
	{
		this.itemTappedListener = listener;
	}

	public MapProductListAdapter.ItemTappedListener getItemTappedListener()
	{
		return this.itemTappedListener;
	}

	public void add(Product item)
	{
		// Validate argument.
		if (item == null) {
			return;
		}

		// Do not continue if given item already exists in the list.
		if (this.list.contains(item)) {
			return;
		}

		// Add the given item to the list.
		this.list.add(item);
		notifyDataSetChanged();
	}

	public void add(Iterable<Product> items)
	{
		if (items != null) {
			for (Product item : items) {
				add(item);
			}
		}
	}

	public void clear()
	{
		this.list.clear();
		notifyDataSetChanged();
	}

	@Override
	public int getCount()
	{
		return this.list.size();
	}

	@Override
	public Product getItem(int index)
	{
		if ((index < 0) || (index >= this.list.size())) {
			return null;
		}
		return this.list.get(index);
	}

	@Override
	public long getItemId(int index)
	{
		return (long)index;
	}

	@Override
	public View getView(int index, View convertView, ViewGroup parent)
	{
		// Set up a new text view to display the indexed product.
		TextView textView = new TextView(this.context);
		textView.setTextColor(android.graphics.Color.BLACK);
		textView.setTextSize(android.util.TypedValue.COMPLEX_UNIT_SP, 20);
		if (parent != null) {
			int padding = (int)android.util.TypedValue.applyDimension(
					android.util.TypedValue.COMPLEX_UNIT_DIP, 10.0f, parent.getResources().getDisplayMetrics());
			textView.setPadding(padding, padding, padding, padding);
		}

		// Apply the indexed product information to the text view created up above.
		final Product product = getItem(index);
		if (product != null) {
			// Strike-out the text if prodcut is flagged as checked.
			doStrikeoutText(textView, product.isChecked());

			// Display the product's name.
			String name = product.getName();
			if (name != null) {
				textView.setText(name);
			}

			// Set up a tap event handler.
			textView.setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View view)
				{
					// Toggle the product's check status.
					product.setChecked(!product.isChecked());
					if (view instanceof TextView) {
						doStrikeoutText((TextView)view, product.isChecked());
					}

					// Invoke the "tapped" listener, if provided.
					if (MapProductListAdapter.this.itemTappedListener != null) {
						MapProductListAdapter.this.itemTappedListener.onTapped(product);
					}
				}
			});
		}

		// Return the requested view for the indexed product.
		return textView;
	}

	private static void doStrikeoutText(TextView textView, boolean isStrikedOut)
	{
		// Validate.
		if (textView == null) {
			return;
		}

		// Apply the given strikout font setting to the text view.
		int flags = textView.getPaintFlags();
		if (isStrikedOut) {
			flags |= Paint.STRIKE_THRU_TEXT_FLAG;
		} else {
			flags &= ~Paint.STRIKE_THRU_TEXT_FLAG;
		}
		textView.setPaintFlags(flags);
	}
}
