// GridCellBitmapRenderer.cpp

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

#include "GridCellBitmapRenderer.h"

GridCellBitmapRenderer::GridCellBitmapRenderer() {

}

GridCellBitmapRenderer::~GridCellBitmapRenderer() {

}

void GridCellBitmapRenderer::Draw(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc, const wxRect & rect, int row, int col, bool isSelected)
{
	// clear
	wxColour BgColor = attr.GetBackgroundColour();
	dc.SetBrush(wxBrush(BgColor, wxSOLID));
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);

	wxBitmap *bmp_pc = _img_go_next_4_small;
	wxBitmap *bmp_bp = _img_stop;

	if (grid.GetCellValue(row, col).Contains(wxT("BP"))) {
		int x = (grid.GetColSize(col) - bmp_bp->GetWidth()) / 2;
		int y = (grid.GetRowSize(col) - bmp_bp->GetHeight()) / 2;
		dc.DrawBitmap(*bmp_bp, rect.x + x, rect.y + y);
	}
	if (grid.GetCellValue(row, col).Contains(wxT("PC"))) {
		int x = (grid.GetColSize(col) - bmp_pc->GetWidth()) / 2;
		int y = (grid.GetRowSize(col) - bmp_pc->GetHeight()) / 2;
		dc.DrawBitmap(*bmp_pc, rect.x + x, rect.y + y);
	}
}



