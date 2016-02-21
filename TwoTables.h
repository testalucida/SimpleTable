/* 
 * File:   TwoTables.h
 * Author: max
 *
 * Created on 16. Januar 2016, 19:02
 */

#ifndef TWOTABLES_H
#define TWOTABLES_H

#include <FL/Fl_Tile.H>
#include "SimpleTable.h"

namespace my {
    class TableData;
}

typedef void( *TwoTablesSelectionCallback ) ( SimpleTable *,
Fl_Table_Copy::TableContext, int r1, int c1, int r2, int c2, void * );

static const char *ID_LEFT = "LEFT";
static const char *ID_RIGHT = "RIGHT";


class TwoTables : public Fl_Tile {
public:
    TwoTables( int x, int y, int w, int h, const char *pLbl = 0 );
    
    /**one Model for both the tables*/
    virtual void setTableData( my::TableData *pData );

	/** register a selection callback function that will be called on each selection
	change */
	void setSelectionCallback( TwoTablesSelectionCallback, void * );
    
    /**Callback function when scrolling in the right table*/
    static void onRightTableScrollStatic( char c, int scrollVal, void *pUserData
    ) {
        ((TwoTables*)pUserData)->onRightTableScroll( c, scrollVal );
    }
    void onRightTableScroll( char c, int scrollVal );
    
    /**Callback function when changing selection in the left table*/
    static void onLeftSelectionStatic( Fl_Table_Copy::TableContext context, 
                                int r1, int c1, int r2, int c2, void *pUserData )
                                
    {
        ((TwoTables*)pUserData)->onLeftSelection( context, r1, c1, r2, c2 );
    }
    void onLeftSelection( Fl_Table_Copy::TableContext context, 
                          int r1, int c1, int r2, int c2 );
    
    /**Callback function when changing selection in the right table*/
    static void onRightSelectionStatic( Fl_Table_Copy::TableContext context, 
                                 int r1, int c1, int r2, int c2, void *pUserData
                                 ) 
    {
        ((TwoTables*)pUserData)->onRightSelection( context, r1, c1, r2, c2 );
    }
    void onRightSelection( Fl_Table_Copy::TableContext context, 
                           int r1, int c1, int r2, int c2 );
    
    /**Callback function when resizing the left table*/
    static void onLeftResizeStatic( int x, int y, int w, int h, void *pUserData )
    {
        ((TwoTables*)pUserData)->onLeftResize( x, y, w, h );
    }
    void onLeftResize( int x, int y, int w, int h );
    
       /**Callback function when resizing the right table*/
    static void onRightResizeStatic( int x, int y, int w, int h, void *pUserData
    ) {
        ((TwoTables*)pUserData)->onRightResize( x, y, w, h );
    }
    void onRightResize( int x, int y, int w, int h );

	/**causes _pLeft and _pRight to not show any more data*/
	void clearData();

	/**adds a Menu-Item to the rowheader popup menu (of the left hand table).
	Custom menu items will be added behind SimpleTable's own
	item "Copy Row Values" */
	void addRowHeaderPopupItem( const char *pLabel, int shortcut, int flags,
		PopupMenuCallback, int id, void *pUserdata );
    
    virtual ~TwoTables();

protected:
    SimpleTable *_pLeft;
    SimpleTable *_pRight;
private:
	TwoTablesSelectionCallback _selectionCallback;
	void *_pSelectionUserData;
};

#endif /* TWOTABLES_H */
