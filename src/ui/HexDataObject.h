// HexDataObject.h

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

#ifndef HEXDATAOBJECT_H_
#define HEXDATAOBJECT_H_

#include <wx/dataobj.h>

class HexDataObject : wxDataObject {
public:
	HexDataObject();
	virtual ~HexDataObject();

	virtual void GetAllFormats(wxDataFormat *formats, Direction dir = Get) const;
	virtual bool GetDataHere(const wxDataFormat& format, void *buf) const;
	virtual size_t GetDataSize(const wxDataFormat& format) const;
	virtual size_t GetFormatCount(Direction dir = Get) const;
	virtual wxDataFormat GetPreferredFormat(Direction dir = Get) const;
	virtual bool SetData(const wxDataFormat& format, size_t len, const void *buf);
};

#endif /* HEXDATAOBJECT_H_ */
