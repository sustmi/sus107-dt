// GridCellBitmapRenderer.h

// Copyright (C) 2012  Miroslav Sustek <sus107@vsb.cz>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GRIDCELLBITMAPRENDERER_H_
#define GRIDCELLBITMAPRENDERER_H_

#include <wx/grid.h>
#include <wx/dc.h>
#include <wx/bitmap.h>

#include "icons.h"

class GridCellBitmapRenderer: public wxGridCellStringRenderer {
public:
	GridCellBitmapRenderer();
	virtual ~GridCellBitmapRenderer();

	void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected);
};

#endif /* GRIDCELLBITMAPRENDERER_H_ */
