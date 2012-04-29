/*
 * HexDataObject.cpp
 *
 *  Created on: 12.3.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include "HexDataObject.h"

HexDataObject::HexDataObject() {
	// TODO Auto-generated constructor stub
}

HexDataObject::~HexDataObject() {
	// TODO Auto-generated destructor stub
}

void HexDataObject::GetAllFormats(wxDataFormat *formats, Direction dir) const
{
	formats[0] = wxDF_TEXT;
}

bool HexDataObject::GetDataHere(const wxDataFormat & format, void *buf) const
{
}

size_t HexDataObject::GetDataSize(const wxDataFormat & format) const
{
}

size_t HexDataObject::GetFormatCount(Direction dir) const
{
	return 1;
}

wxDataFormat HexDataObject::GetPreferredFormat(Direction dir) const
{
}

bool HexDataObject::SetData(const wxDataFormat & format, size_t len, const void *buf)
{
}



