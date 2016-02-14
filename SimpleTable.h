/*
* File: SimpleTable.h
* Author: max
*
* Created on 22. März 2015, 16:43
*/

#ifndef SIMPLE_TABLE_H
#define SIMPLE_TABLE_H


#include "Fl_Table_Copy.h"
#include <FL/Enumerations.H>
#include <my/TableData.h>
#include "ICellStyleProvider.h"
#include <string>

enum SelectionMode {
    SELECTIONMODE_CELL_SINGLE,
    SELECTIONMODE_CELL_MULTI
};

enum ResizeMode {
    RESIZEMODE_NONE,
    RESIZEMODE_LAST_COL,
    RESIZEMODE_ALL_COLS  
};

class DefaultCellStyleProvider : public ICellStyleProvider {
public:
	DefaultCellStyleProvider();
    
	virtual const BackgroundStyle& 
    getBackgroundStyle( int row, int colIdxView, int colIdxModel, bool isSelected );
    
	virtual const FontStyle& 
    getFontAndColor( int row, int colIdxView, int colIdxModel, bool isSelected );
    
	virtual const FontStyle& 
    getDefaultFontAndColor( Fl_Table_Copy::TableContext	context ) const;
    
	virtual void 
    setAlternatingColumnColor( Fl_Color color = 
                               fl_lighter( fl_rgb_color( 242, 234, 255 ) ) ) 
     
    {
		_alternatingColumnColor = color;
		_isAlternatingColumnColor = color == _cellBackground.backColor ? false : true;
	}
    
	virtual void 
    setAlternatingRowColor( Fl_Color color = 
                            fl_lighter( fl_rgb_color( 242, 234, 255 ) ) ) 
    {
		_alternatingRowColor = color;
		_isAlternatingRowColor = true;
	}
	virtual ~DefaultCellStyleProvider() {}
protected:
	//Cells
	BackgroundStyle _cellBackground;
	FontStyle _cellFontStyle;
	BackgroundStyle _tmpBackground;
	
	//Column Headers
	BackgroundStyle _colHeaderBackground;
	FontStyle _colHeaderFontStyle;

	//Row Headers
	BackgroundStyle _rowHeaderBackground;
	FontStyle _rowHeaderFontStyle;

	bool _isAlternatingRowColor;
	bool _isAlternatingColumnColor;
	Fl_Color _alternatingRowColor;
	Fl_Color _alternatingColumnColor;

};


typedef void (*ScrollCallback) (char orientiation, int scrollvalue, void * );
typedef void (*SelectionCallback) ( Fl_Table_Copy::TableContext, int r1, int c1, 
                                    int r2, int c2, void * );
typedef void (*ResizeCallback) (int x, int y, int w, int h, void * );

/** callback after having selected a popup menu item.
    TableContext: CONTEXT_CELL, CONTEXT_COL_HEADER, CONTEXT_ROW_HEADER
    id: custom id
     r: row selected
     c: col selected (that's the model index, *not* the view index
     void *: userdata */
typedef void (*PopupMenuCallback) ( Fl_Table_Copy::TableContext, int id, 
                                    int r, int c, void * );

struct Fl_Menu_Item;
class Fl_Menu_Button;

class SimpleTable : public Fl_Table_Copy {
    struct IndexRel {
        int viewIdx;
        int modelIdx;
    };
    struct MenuItemEx {
//        MenuItemEx();
        std::string label;
        int shortcut;
        int flags;
        int id;
        void *userdata;
        PopupMenuCallback cb;
//        ~MenuItemEx();
    };
public:
    SimpleTable( int X, int Y, int W, int H, const char* L = 0 );
    void setTableData( my::TableData *pDataTable );
    my::TableData* getTableData() { return _pData; }
    int handle( int );
    bool isNothingSelected();
    /**prevents a column of TableData from beeing displayed*/
    void hideColumn( const char * );
	/** shows all hidden columns */
	void unhideColumns();
	/** shows a hidden column */
	void unhideColumn( const char *pColName );
    void setSelectionMode( SelectionMode selMode ) { _selMode = selMode; }
    virtual bool canSelectCell ( int r, int c ) { return true; }
	/**
	\deprecated
	Wird nur noch von Calendar verwendet, deshalb können wir das hier nicht 
	rausschmeißen.
	*/
    virtual Fl_Color getCellBackground( int row, int col, bool isSelected )
    const;
    inline int getModelIndex( int viewIndex ) const;
	inline int getViewIndex( int modelIndex ) const;
    int getViewIndex( const char *pLabel ) const;
    void setAlternatingColumnColor( Fl_Color color = 
                                 fl_lighter( fl_rgb_color( 242, 234, 255 ) ) );    
    void setAlternatingRowColor( Fl_Color color = 
                                 fl_lighter( fl_rgb_color( 242, 234, 255 ) ) );
    int getVScrollbarWidth() const;
    /**return the width of all columns optional including rowheader */
    int getAllColumnsWidth( bool includeRowHeader = true );
    bool isVScrollbarVisible() const;
    void setScrollCallback( ScrollCallback, void * );
    void setScrollValue( char orientation, int newValue );
    void setSelectionCallback( SelectionCallback, void * );
    void setResizeMode( ResizeMode mode ) { _resizeMode = mode; }
    void setResizeCallback( ResizeCallback, void * );
    virtual void resize(int x, int y, int w, int h);
    
	/**sets rows and cols to zero; releases pointer to TableData.
	   Does *not* delete TableData. */
	void releaseTableData();
    
    /**fills the available table width by dividing table width by
     the number of columns and giving all columns the same 
     width*/
    void makeColumnsFit();
    
	/**makes a column fit to its widest content.
	   if skipHiddenColumns == false, hidden columns will be unhide()ed 
	   and adapted like the unhidden columns */
    void adaptColumnWidthToContent( bool skipHiddenColumns = true );
	void adaptColumnWidthToContent( int colIdx );
	
	/** set a ICellStyleProvider for drawing cell operations */
	void setCellStyleProvider( ICellStyleProvider *pProvider ) { 
        _pCellStyleProvider = pProvider; 
    }

	/**sets an ID for identifying purposes*/
	void setId( const std::string &id ) { _id = id; }

	/**gets the ID */
	const std::string& getId() const { return _id; }
    
    /**adds a Menu-Item to the cell popup menu.
       Custom Menu Items will be added behind SimpleTable's own
       items (Copy, Paste, Search) */ 
    void addCellPopupItem( const char *pLabel, int shortcut, int flags, 
                           PopupMenuCallback, int id, void *pUserdata );
    
    /**adds a Menu-Item to the rowheader popup menu.
       Custom menu items will be added behind SimpleTable's own
       item "Copy Row Values" */ 
    void addRowHeaderPopupItem( const char *pLabel, int shortcut, int flags, 
                                PopupMenuCallback, int id, void *pUserdata );
    
    MenuItemEx *createMenuItemEx( const char *pLabel, int shortcut, int flags,
                                  PopupMenuCallback cb, int id, void *pUserdata );
    
    virtual ~SimpleTable();
protected:
    virtual void draw_cell( TableContext context, int = 0, int = 0, int = 0, 
                            int = 0, int = 0, int = 0 );
    
    /**is beeing called if the user resizes a column.
       Fills possibly occuring empty spaces by expanding the 
       last column*/
    virtual void checkEmptySpaceAndFill();
    bool isInCurrentSelection( int r, int c );
   
private:
	void tableDataCallback( my::TableData &src, my::DataChangedEvent &evt );
	void createIndexRelations();
//    void adjustSelection( TableContext, int, int );
	void colWidth2Content( int colIdx );
    static void onScrollStatic( Fl_Widget *, void * );
    void onScroll( Fl_Scrollbar * );
    void doSelectionCallback( Fl_Table_Copy::TableContext );
    void createCellAndRowPopup();
    void handlePopup( TableContext context, int x, int y, int r, int c );
    void handleColumnHeaderPopup( int x, int y, int c );
    void handleRowHeaderPopup( int x, int y, int r );
    void handleCellPopup( int x, int y, int r, int c );
    void copySelectionToClipboard();
    void copyColumnValuesToClipboard( int col, bool withSeparator );
    void copyRowValuesToClipboard( int row );
    void createAllColumnsSubmenu( Fl_Menu_Button &menu );
    /** sorts columns in submenu */
    static bool alphaSort( const char *pVal1, const char *pVal2 );
    void search();
private:
    my::TableData *_pData;
    std::vector<IndexRel> _indexRelations;
    std::vector<IndexRel> _tmp;
	std::vector<IndexRel> _hidden;
    SelectionMode _selMode;
    ScrollCallback _scrollCallback;
    void *_pScrollUserData;
    SelectionCallback _selectionCallback;
    void *_pSelectionUserData;
    ResizeCallback _resizeCallback;
    void *_pResizeUserData;
    ResizeMode _resizeMode;
	ICellStyleProvider *_pCellStyleProvider;
	std::string _id;
    std::vector<MenuItemEx*> _rowHeaderPopItems;
    std::vector<MenuItemEx*> _cellPopItems;
    my::SortDirection _sortDirection;
    int _sortedColumn;
};

#endif /* FLX_SPREADSHEET_H */