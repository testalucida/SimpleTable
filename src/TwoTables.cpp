/* 
 * File:   TwoTables.cpp
 * Author: max
 * 
 * Created on 16. Januar 2016, 19:02
 */

#include "TwoTables.h"
//#include <SimpleTable.h>

#include <my/TableData.h>

TwoTables::TwoTables( int x, int y, int w, int h, const char *pLbl ) 
: Fl_Tile( x, y, w, h, pLbl )
{
    _pLeft = new SimpleTable( Fl_Tile::x(), Fl_Tile::y(), w/3, h );
    //_pLeft = new SimpleTable( Fl_Tile::x(), Fl_Tile::y(), 200, 490 );
    _pLeft->row_header( 1 );
    _pLeft->setAlternatingRowColor();
    _pLeft->setSelectionCallback( onLeftSelectionStatic, this );
    _pLeft->setResizeCallback( onLeftResizeStatic, this );
    _pLeft->tab_cell_nav( 0 );
    
    _pRight = new SimpleTable( _pLeft->x() + _pLeft->w(), _pLeft->y(),
                               w - _pLeft->w(), _pLeft->h() );
    _pRight->setAlternatingRowColor();
    _pRight->setScrollCallback( onRightTableScrollStatic, this );
    _pRight->setSelectionCallback( onRightSelectionStatic, this );
    _pRight->setResizeCallback( onRightResizeStatic, this );
    _pRight->tab_cell_nav( 0 );
    
//    fprintf( stderr, "left: %d, %d, %d, %d\n", 
//                     _pLeft->x(), _pLeft->y(), _pLeft->w(), _pLeft->h() );
//    fprintf( stderr, "rght: %d, %d, %d, %d\n", 
//                     _pRight->x(), _pRight->y(), _pRight->w(), _pRight->h() );
    end();
}

void TwoTables::setTableData( my::TableData *pData ) {
    _pLeft->setTableData( pData );
    _pRight->setTableData( pData );
   
    
    _pLeft->showVScrollbar( false );
    _pLeft->makeColumnsFit();
    
    _pRight->set_selection( -1, -1, -1, -1 );
     _pLeft->set_selection( 0, 0, 0, 0 );
}

void TwoTables::onRightTableScroll( char c, int scrollVal ) {
    _pLeft->setScrollValue( c, scrollVal );
    _pLeft->redraw();
}

void TwoTables::onLeftSelection( Fl_Table_Copy::TableContext context, 
                                 int r1, int c1, int r2, int c2 ) 
{
    if( context == Fl_Table_Copy::CONTEXT_ROW_HEADER ) {
        _pRight->set_selection( r1, 0, r2, _pRight->cols() - 1 );
    } else {
        _pRight->set_selection( -1, -1, -1, -1 );
    }
}

 void TwoTables::onRightSelection( Fl_Table_Copy::TableContext context, 
                                   int r1, int c1, int r2, int c2 ) 
 {
     _pLeft->set_selection( -1, -1, -1, -1 );
 }
 
 void TwoTables::onLeftResize( int x, int y, int w, int h ) {
     _pLeft->makeColumnsFit();
//      fprintf( stderr, "left: %d, %d, %d, %d\n", 
//                     _pLeft->x(), _pLeft->y(), _pLeft->w(), _pLeft->h() );
//    fprintf( stderr, "rght: %d, %d, %d, %d\n", 
//                     _pRight->x(), _pRight->y(), _pRight->w(), _pRight->h() );
 }
 
 void TwoTables::onRightResize( int x, int y, int w, int h ) {
     _pRight->makeColumnsFit();
//      fprintf( stderr, "left: %d, %d, %d, %d\n", 
//                     _pLeft->x(), _pLeft->y(), _pLeft->w(), _pLeft->h() );
//    fprintf( stderr, "rght: %d, %d, %d, %d\n", 
//                     _pRight->x(), _pRight->y(), _pRight->w(), _pRight->h() );
 }
 
TwoTables::~TwoTables( ) {
}

