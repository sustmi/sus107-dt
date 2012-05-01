/*
 * GridCellBitmapRenderer.h
 *
 *  Created on: 1.5.2012
 *      Author: mirek
 */

#ifndef GRIDCELLBITMAPRENDERER_H_
#define GRIDCELLBITMAPRENDERER_H_

#include <wx/grid.h>
#include <wx/dc.h>
#include <wx/bitmap.h>

class GridCellBitmapRenderer: public wxGridCellBoolRenderer {
public:
	GridCellBitmapRenderer(wxBitmap *bitmap);
	virtual ~GridCellBitmapRenderer();

	void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected);

private:
	wxBitmap *m_bmp;
};

#endif /* GRIDCELLBITMAPRENDERER_H_ */
