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
, _pLeft( NULL )
, _pRight( NULL )
, _selectionCallback( NULL )
, _pSelectionUserData( NULL )
{
    _pLeft = new SimpleTable( Fl_Tile::x(), Fl_Tile::y(), w/3, h );
	_pLeft->setId( ID_LEFT );
    _pLeft->row_header( 1 );
    _pLeft->setAlternatingRowColor();
    _pLeft->setSelectionCallback( onLeftSelectionStatic, this );
    _pLeft->tab_cell_nav( 0 );

    _pRight = new SimpleTable( _pLeft->x() + _pLeft->w(), _pLeft->y(),
                               w - _pLeft->w(), _pLeft->h() );
	_pRight->setId( ID_RIGHT );
    _pRight->setAlternatingRowColor();
    _pRight->setScrollCallback( onRightTableScrollStatic, this );
    _pRight->setSelectionCallback( onRightSelectionStatic, this );
    _pRight->tab_cell_nav( 0 );
    
    end();
}

void TwoTables::setTableData( my::TableData *pData ) {
    _pLeft->setTableData( pData );
    _pRight->setTableData( pData );
      
    _pLeft->showVScrollbar( false );
    
    _pRight->set_selection( -1, -1, -1, -1 );
    _pLeft->set_selection( 0, 0, 0, 0 );
}

void TwoTables::setSelectionCallback( TwoTablesSelectionCallback cb, void
*pUserData ) {
	_selectionCallback = cb;
	_pSelectionUserData = pUserData;
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

	if( _selectionCallback ) {
		( *_selectionCallback )( _pRight, context, -1, -1, -1, -1, _pSelectionUserData
		);
		( *_selectionCallback )( _pLeft, context, r1, c1, r2, c2, _pSelectionUserData
		);		
	}
}

 void TwoTables::onRightSelection( Fl_Table_Copy::TableContext context, 
                                   int r1, int c1, int r2, int c2 ) 
 {
     _pLeft->set_selection( -1, -1, -1, -1 );

	 if( _selectionCallback ) {
		 ( *_selectionCallback )( _pLeft, context, -1, -1, -1, -1, _pSelectionUserData
		 );
		 ( *_selectionCallback )( _pRight, context, r1, c1, r2, c2, _pSelectionUserData
		 );		 
	 }
 }
 
 void TwoTables::onLeftResize( int x, int y, int w, int h ) {
     //_pLeft->makeColumnsFit();
//    fprintf( stderr, "-----TwoTables::onLeftResize-------\n" );
//    fprintf( stderr, "left: %d, %d, %d, %d -- parent->w = %d\n", 
//                     _pLeft->x(), _pLeft->y(), _pLeft->w(), _pLeft->h(),this->w() );
//    fprintf( stderr, "rght: %d, %d, %d, %d\n", 
//                     _pRight->x(), _pRight->y(), _pRight->w(), _pRight->h() );
//    fprintf( stderr, "-----------------------------------\n" );
 }
 
 void TwoTables::onRightResize( int x, int y, int w, int h ) {
     //_pRight->makeColumnsFit();
//      fprintf( stderr, "left: %d, %d, %d, %d\n", 
//                     _pLeft->x(), _pLeft->y(), _pLeft->w(), _pLeft->h() );
//    fprintf( stderr, "rght: %d, %d, %d, %d\n", 
//                     _pRight->x(), _pRight->y(), _pRight->w(), _pRight->h() );
 }

 /**causes _pLeft and _pRight to not show any more data*/
 void TwoTables::clearData() {
	 _pLeft->releaseTableData();
	 _pRight->releaseTableData();
 }

 void TwoTables::addRowHeaderPopupItem( const char *pLabel, int shortcut, int
 flags,
										PopupMenuCallback cb, int id, void *pUserdata )
 {
	 _pLeft->addRowHeaderPopupItem( pLabel, shortcut, flags, cb, id, pUserdata );
 }

TwoTables::~TwoTables( ) {
}