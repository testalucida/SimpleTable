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
    void hideColumn( const char * );
    void enableDragging( bool enable ) { _enableDragging = enable;}
    virtual bool canSelectCell ( int r, int c ) { return true; }
    virtual Fl_Color getCellBackground( int row, int col, bool isSelected ) const;
    int getModelIndex( int viewIndex ) const;
    void setAlternatingColumnColor( Fl_Color color = fl_lighter( fl_rgb_color( 242, 234, 255 ) ) );    
    void setAlternatingRowColor( Fl_Color color = fl_lighter( fl_rgb_color( 242, 234, 255 ) ) );
    int getVScrollbarWidth() const;
    int getAllColumnsWidth();
    bool isVScrollbarVisible() const;
    void setScrollCallback( ScrollCallback, void * );
    void setScrollValue( char orientation, int newValue );
    void setSelectionCallback( SelectionCallback, void * );
    void setResizeCallback( ResizeCallback, void * );
    virtual void resize(int x, int y, int w, int h);
    void makeColumnsFit();
//    static void event_callback( Fl_Widget*, void *v ) { // table's event callback (static)
//        ( (SimpleTable*) v )->event_callback2( );
//    }
    virtual ~SimpleTable() {};
protected:
    virtual void draw_cell( TableContext context, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0 );
//    void event_callback2( ); // table's event callback (instance)
   
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
    bool _enableDragging;
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
};

#endif /* FLX_SPREADSHEET_H */