/*
* File: SimpleTable.h
* Author: max
*
* Created on 22. März 2015, 16:43
*/

#ifndef SIMPLE_TABLE_H
#define SIMPLE_TABLE_H

//#include <FL/Fl_Table_Row.H>
//#include <FL/Fl_Table.H>
#include "Fl_Table_Copy.h"
#include <FL/Fl_Input.H>
#include <my/TableData.h>


struct IndexRel {
    int viewIdx;
    int modelIdx;
};

enum SelectionMode {
    SELECTIONMODE_CELL_SINGLE,
    SELECTIONMODE_CELL_MULTI
};

enum ResizeMode {
    RESIZEMODE_NONE,
    RESIZEMODE_LAST_COL,
    RESIZEMODE_ALL_COLS  
};

typedef void (*ScrollCallback) (char orientiation, int scrollvalue, void * );
typedef void (*SelectionCallback) (Fl_Table_Copy::TableContext, int r1, int c1, int r2, int c2, void * );
typedef void (*ResizeCallback) (int x, int y, int w, int h, void * );

class SimpleTable : public Fl_Table_Copy {
public:
    SimpleTable( int X, int Y, int W, int H, const char* L = 0 );
    void setTableData( my::TableData *pDataTable );
    my::TableData* getTableData() { return _pData; }
    int handle( int );
    bool isNothingSelected();
    /**prevents a column of TableData from beeing displayed*/
    void hideColumn( const char * );
    void setSelectionMode( SelectionMode selMode ) { _selMode = selMode; }
    virtual bool canSelectCell ( int r, int c ) { return true; }
    virtual Fl_Color getCellBackground( int row, int col, bool isSelected ) const;
    inline int getModelIndex( int viewIndex ) const;
    void setAlternatingColumnColor( Fl_Color color = fl_lighter( fl_rgb_color( 242, 234, 255 ) ) );    
    void setAlternatingRowColor( Fl_Color color = fl_lighter( fl_rgb_color( 242, 234, 255 ) ) );
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
    /**fills the available table width by dividing table width by
     the number of columns and giving all columns the same 
     width*/
    void makeColumnsFit();
    void adaptColumnWidthToContent();
    virtual ~SimpleTable() {};
protected:
    virtual void draw_cell( TableContext context, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0 );
    
    /**is beeing called if the user resizes a column.
       Fills possibly occuring empty spaces by expanding the 
       last column*/
    virtual void checkEmptySpaceAndFill();
   
private:
//    void adjustSelection( TableContext, int, int );
    static void onScrollStatic( Fl_Widget *, void * );
    void onScroll( Fl_Scrollbar * );
    void doSelectionCallback( Fl_Table_Copy::TableContext );
private:
    my::TableData *_pData;
    Fl_Fontsize _headerFontsize;
    Fl_Fontsize _cellFontsize;
    std::vector<IndexRel> _indexRelations;
    std::vector<IndexRel> _tmp;
    SelectionMode _selMode;
    //bool _enableDragging;
    bool _isAlternatingColumnColor;
    bool _isAlternatingRowColor;       
    Fl_Color _backgroundColor;
    Fl_Color _alternatingColumnColor;
    Fl_Color _alternatingRowColor;
    //bool _hideVScrollbar;
    ScrollCallback _scrollCallback;
    void *_pScrollUserData;
    SelectionCallback _selectionCallback;
    void *_pSelectionUserData;
    ResizeCallback _resizeCallback;
    void *_pResizeUserData;
    ResizeMode _resizeMode;
};

#endif /* FLX_SPREADSHEET_H */