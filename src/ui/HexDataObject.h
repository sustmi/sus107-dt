/*
 * HexDataObject.h
 *
 *  Created on: 12.3.2012
 *      Author: mirek
 */

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
