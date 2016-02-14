

#ifndef _ICOLORPROVIDER_H_
#define _ICOLORPROVIDER_H_

#include <FL/Enumerations.H>
#include "Fl_Table_Copy.h"

struct BackgroundStyle {
	BackgroundStyle() 
		: boxtype( FL_FLAT_BOX )
		, backColor( FL_WHITE )
	{
	}
	BackgroundStyle clone() {
		BackgroundStyle clone;
		clone.backColor = this->backColor;
		clone.boxtype = this->boxtype;
		return clone;
	}
	Fl_Boxtype boxtype;
	Fl_Color backColor;
};

struct FontStyle {
	FontStyle() 
		: font( FL_HELVETICA )
		, size( 12 )
		, color( FL_BLACK )
	{}
	Fl_Font font;
	Fl_Fontsize size;
	Fl_Color color;
};

class ICellStyleProvider {
public:
	virtual const BackgroundStyle& 
    getBackgroundStyle( int row, int colIdxView, int colIdxModel, bool isSelected ) = 0;
    
	virtual const FontStyle& 
    getFontAndColor( int row, int colIdxView, int colIdxModel, bool isSelected ) = 0;
    
	virtual const FontStyle& getDefaultFontAndColor( Fl_Table_Copy::TableContext
	context ) const = 0;
	virtual void setAlternatingColumnColor( Fl_Color color ) = 0;
	virtual void setAlternatingRowColor( Fl_Color color ) = 0;
};

#endif

