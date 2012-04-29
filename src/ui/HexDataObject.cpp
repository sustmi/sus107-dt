// HexDataObject.cpp

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



