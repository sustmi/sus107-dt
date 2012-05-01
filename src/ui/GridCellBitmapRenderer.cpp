/*
 * GridCellBitmapRenderer.cpp
 *
 *  Created on: 1.5.2012
 *      Author: mirek
 */

#include "GridCellBitmapRenderer.h"

GridCellBitmapRenderer::GridCellBitmapRenderer(wxBitmap *bitmap) {
	m_bmp = bitmap;
}

GridCellBitmapRenderer::~GridCellBitmapRenderer() {
	// TODO Auto-generated destructor stub
}

void GridCellBitmapRenderer::Draw(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc, const wxRect & rect, int row, int col, bool isSelected)
{
	if (grid.GetCellValue(row, col) == wxT("true")) {
		int x = (grid.GetColSize(col) - m_bmp->GetWidth()) / 2;
		int y = (grid.GetRowSize(col) - m_bmp->GetHeight()) / 2;
		dc.DrawBitmap(*m_bmp, rect.x + x, rect.y + y);
	} else {
		wxColour BgColor = attr.GetBackgroundColour();
		dc.SetBrush(wxBrush(BgColor, wxSOLID));
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);
	}
}



