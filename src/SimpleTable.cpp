/*
 * File: SimpleTable.cpp
 * Author: max
 *
 * Created on 22. März 2015, 16:43
 */

#include "SimpleTable.h"
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Menu_Item.H>
#include "ICellStyleProvider.h"

using namespace my;

//===========================================================================
//                        Implementation of ICellStyleProvider
//===========================================================================

DefaultCellStyleProvider::DefaultCellStyleProvider() 
	: _isAlternatingRowColor( false )
	, _isAlternatingColumnColor( false )
	, _alternatingRowColor(fl_lighter( fl_lighter( FL_GREEN ) ) )
	, _alternatingColumnColor( fl_lighter( fl_lighter( FL_GREEN ) ) )
{
	_colHeaderBackground.backColor = fl_lighter( FL_GRAY );
	_rowHeaderBackground.backColor = fl_lighter( FL_GRAY );
}

const BackgroundStyle& DefaultCellStyleProvider::getBackgroundStyle( int row, int colIdxView, int colIdxModel, bool isSelected ) 								
{
	if( row < 0 ) {
		return _colHeaderBackground;
	} else if( colIdxView < 0 ) {
		return _rowHeaderBackground;
	} else {
		//BackColor
		_tmpBackground = _cellBackground.clone();
		_tmpBackground.backColor = isSelected ? FL_YELLOW : _cellBackground.backColor;
		if( !isSelected && ( _isAlternatingColumnColor || _isAlternatingRowColor ) ) {
			if( _isAlternatingColumnColor ) {
				int rem = colIdxView % 2;
				if( rem != 0 ) _tmpBackground.backColor = _alternatingColumnColor;
			}
			if( _isAlternatingRowColor ) {
				int rem = row % 2;
				if( rem != 0 ) _tmpBackground.backColor = _alternatingRowColor;
			}
		}
		
		return _tmpBackground;
	}
}

const FontStyle& DefaultCellStyleProvider::getFontAndColor( int row, int colIdxView, int colIdxModel ) 
{
	if( row < 0 ) {
		//column header style
		return _colHeaderFontStyle;
	} else if( colIdxView < 0 ) {
		//row header style
		return _rowHeaderFontStyle;
	} else {
		// cell style
		return _cellFontStyle;
	}
}

const FontStyle& DefaultCellStyleProvider::getDefaultFontAndColor(
Fl_Table_Copy::TableContext context ) const {
	if( context == Fl_Table_Copy::TableContext::CONTEXT_CELL ) {
		return _cellFontStyle;
	} else if( context == Fl_Table_Copy::TableContext::CONTEXT_ROW_HEADER ) {
		return _rowHeaderFontStyle;
	} else if( context == Fl_Table_Copy::TableContext::CONTEXT_COL_HEADER ) {
		return _colHeaderFontStyle;
	} else {
		return _cellFontStyle;
	}
}

//===========================================================================
//===========================================================================

//MenuItemEx::MenuItemEx() {
//    menu = new Fl_Menu_Button();
//}
//       
//MenuItemEx::~MenuItemEx() {
//    delete menuItem;
//}
        
 //===========================================================================
//===========================================================================       

const char *COPY =  "Kopieren   ";
const char *PASTE = "Einfügen   ";
const char *SEARCH = "Suchen... ";
const char *ROWS   = "Zeilen:   ";

SimpleTable::SimpleTable( int X, int Y, int W, int H, const char* L )
: Fl_Table_Copy( X, Y, W, H, L )
, _pData( NULL )
, _selMode( SELECTIONMODE_CELL_SINGLE )
, _scrollCallback( NULL )
, _pScrollUserData( NULL )
, _selectionCallback( NULL )
, _pSelectionUserData( NULL )
, _resizeCallback( NULL )
, _pResizeUserData( NULL )
, _resizeMode( RESIZEMODE_LAST_COL )
, _pCellStyleProvider( new DefaultCellStyleProvider() )
{
   
#if FLTK_ABI_VERSION >= 10303
    tab_cell_nav( 1 );
#endif

    col_header( 1 );
    col_resize( 1 );
    row_header_color( FL_LIGHT1 | FL_GRAY );
    col_header_color( FL_LIGHT1 | FL_GRAY );

    end( );
    set_selection( 0, 0, 0, 0 );
    
    vscrollbar->callback( onScrollStatic, this );
    
    createCellPopup();
}

void SimpleTable::createCellPopup() {
    addCellPopupItem( ROWS, 0, FL_MENU_INACTIVE, 0, -1, 0 );
    addCellPopupItem( COPY, FL_CTRL+'c', 0 /*flags*/, 0 /*PopupMenuCallback*/, -1, 0 );
    addCellPopupItem( PASTE, FL_CTRL+'p', FL_MENU_DIVIDER | FL_MENU_INACTIVE, 0, -1, 0 );
    addCellPopupItem( SEARCH, FL_CTRL+'f', FL_MENU_DIVIDER | FL_MENU_INACTIVE, 0, -1, 0 );
    
}

void SimpleTable::setTableData( my::TableData *pDataTable ) {
    _pData = pDataTable;
	rows( _pData->getRowCount() ); 
	cols( _pData->getColumnCount() );
	_pData->signalDataChanged.disconnect < SimpleTable,
	&SimpleTable::tableDataCallback >( this );
	_pData->signalDataChanged.connect< SimpleTable, &SimpleTable::tableDataCallback
	>( this );
	createIndexRelations();
    adaptColumnWidthToContent();
}

void SimpleTable::tableDataCallback( my::TableData &src, my::DataChangedEvent &evt ) {
	switch( evt.dataChange ) {
	case TABLEDATA_VALUE_CHANGED:
	
	break;
	case TABLEDATA_ROW_ADDED:
		rows( ++evt.row );		
		break;
	case TABLEDATA_COLUMN_ADDED:
		cols( ++evt.col );		
		createIndexRelations();
		break;
	case TABLEDATA_ROW_REMOVED:
	{
		int r = rows();
		rows( --r );		
		break;
	}
	case TABLEDATA_ALL_ROWS_REMOVED:
		rows( 0 );		
		break;
	case TABLEDATA_COLUMN_REMOVED:
	{
		int c = cols();
		cols( --c );	
		createIndexRelations();
		break;
	}
	case TABLEDATA_CLEARED:
		rows( 0 );
		cols( 0 );	
		break;
	case TABLEDATA_CHANGED:
		clear();
		rows( _pData->getRowCount() );
		cols( _pData->getColumnCount() );		
		break;
	case TABLEDATA_SORTED:
		break;
	default:
		throw( "Flx_Table::tableDataCallback: no such TableDataChange" );
	}

	redraw();
}

void SimpleTable::createIndexRelations() {
	_indexRelations.clear();
	_hidden.clear();
	if( _pData ) {
		rows( _pData->getRowCount() );
		cols( _pData->getColumnCount() );
		for( int c = 0, cmax = _pData->getColumnCount(); c < cmax; c++ ) {
			IndexRel rel;
			rel.viewIdx = rel.modelIdx = c;
			_indexRelations.push_back( rel );
		}
	}
}

int SimpleTable::handle( int evt ) {
    //int rc = Fl_Table_Copy::handle( evt );
    int r, c;
    ResizeFlag resizeFlag;
    TableContext context = cursor2rowcol( r, c, resizeFlag );
    
    switch( evt ) {
        case FL_PUSH:
            if( Fl::focus( ) != this ) {
                Fl::focus( this );
            }
            if( !canSelectCell( r, c ) ) {
                return 1;
            } else {
                Fl_Table_Copy::handle( evt );                
                if( Fl::event_state( FL_BUTTON3 ) ) {
                    set_selection( r, c, r, c );
                }
                doSelectionCallback( context );
                if( Fl::event_state( FL_BUTTON3 ) ) {
                    showPopup( context, Fl::event_x(), Fl::event_y(), r, c );
                }
                return 1;
            }
            break;
        case FL_KEYDOWN:
        {
            int key = Fl::event_key();
            if( key == FL_Tab || key == FL_Down || key == FL_Up || 
                key == FL_Left || key == FL_Right ) 
            {
                Fl_Table_Copy::handle( evt );
                doSelectionCallback( context );
                return 1;
            }
        }
        case FL_DRAG:
            if( context == CONTEXT_CELL && _selMode == SELECTIONMODE_CELL_SINGLE ) {
                return 1;
            } else {
                Fl_Table_Copy::handle( evt );                
                if( context == CONTEXT_COL_HEADER ) {
                    //resizing a column; 
                    //check empty spaces of table and fill them, if any
//                    checkEmptySpaceAndFill();                    
                } else {
                    doSelectionCallback( context );
                }
                return 1;
            }
            break;
        default:
            break;
    }
    //return rc;
    return Fl_Table_Copy::handle( evt );
}

bool SimpleTable::isNothingSelected( ) {
    int x1, y1, x2, y2;
    get_selection( x1, y1, x2, y2 );
    return x1 < 0 && y1 < 0 && x2 < 0 && y2 < 0;
}

void SimpleTable::hideColumn( const char *pColName ) {
    //Model-Index der Spalte ermitteln...
    int modelIdx = _pData->getColumnIndex( pColName );
	col_width( getViewIndex( modelIdx ), 0 );
}

void SimpleTable::unhideColumns() {
	//createIndexRelations();
	adaptColumnWidthToContent();
}

void SimpleTable::unhideColumn( const char *pColName ) {
	//Model-Index der Spalte ermitteln...
	int modelIdx = _pData->getColumnIndex( pColName );
	adaptColumnWidthToContent( getViewIndex( modelIdx ) ); //TODO:  statt modelIdx	viewIdx aus IndexRel verwenden

	//hack to enforce redraw of last column :
	col_width( cols()-1, col_width( cols()-1 ) + 1 );
	col_width( cols() - 1, col_width( cols() - 1 ) - 1 );
}

/** Handle drawing all cells in table */
void SimpleTable::draw_cell( TableContext context, int R, int C, int X, int Y, int W, int H ) {
	int c = getModelIndex( C );
    switch( context ) {
        case CONTEXT_STARTPAGE: // table about to redraw
            break;
        case CONTEXT_COL_HEADER: // table wants us to draw a column heading (C is column)
			fl_font( FL_HELVETICA, _pCellStyleProvider->getFontAndColor( -1, C, c ).size ); // _headerFontsize ); // set font for heading to bold
            fl_push_clip( X - 1, Y, W, H ); // clip region for text
        {
            fl_draw_box( FL_FLAT_BOX, X, Y, W, H, col_header_color( ) );
            //draw a vertical line between columns and a downsided horizontal one
            fl_color( FL_GRAY );
            fl_line( X - 1, Y, X - 1, Y + H - 1, X + W, Y + H - 1 );
            if( _pData ) {
                fl_color( FL_BLACK );
                fl_draw( _pData->getColumnHeader( c ), X, Y, W, H,
                FL_ALIGN_CENTER );
            }
        }
            fl_pop_clip( );
            return;
        case CONTEXT_ROW_HEADER: // table wants us to draw a row heading (R is row)
			fl_font( FL_HELVETICA, _pCellStyleProvider->getFontAndColor( R, -1, -1 ).size ); // set font for row heading to bold
            fl_push_clip( X, Y - 1, W + 1, H );
        {
            fl_draw_box( FL_FLAT_BOX, X, Y, W, H, row_header_color( ) );
            //draw horizontal line between rows and a vertical right sided line
            fl_color( FL_GRAY);
            fl_line( X, Y - 1, X + W - 1, Y - 1, X + W - 1, Y + H );
            if( _pData ) {
                fl_color( FL_BLACK );
                fl_draw( _pData->getRowHeader( R ), X, Y, W, H, FL_ALIGN_CENTER
                );
            }
        }
            fl_pop_clip( );
            return;
        case CONTEXT_CELL:
        { // table wants us to draw a cell
            //fl_push_clip( X - 1, Y - 1, W, H );
            fl_push_clip( X, Y, W, H );
            // Background
//            fl_draw_box( FL_FLAT_BOX, X - 1, Y - 1, W + 2, H + 2, 
						
			BackgroundStyle backstyle = _pCellStyleProvider->getBackgroundStyle( R, C, c,
			is_selected( R, C ) );
			//fprintf( stderr, "drawing cell: %d, %d -- selected: %s ==> backColor: %d\n",
			
			//	     R, C, is_selected( R, C ) ? "true" : "false", backstyle.backColor );
			fl_draw_box( backstyle.boxtype, X, Y, W, H, backstyle.backColor );             
			         
            
            //draw a vertical line between columns and a downsided horizontal one
            fl_color( FL_GRAY );
            fl_line( X - 1, Y, X - 1, Y + H - 1, X + W, Y + H - 1 );
            //draw horizontal line between rows and a vertical right sided line
            fl_line( X, Y - 1, X + W - 1, Y - 1, X + W - 1, Y + H );
            
            if( _pData ) {				
				FontStyle fontstyle = _pCellStyleProvider->getFontAndColor( R, C, c );				
                fl_color( fontstyle.color );
                fl_font( fontstyle.font, fontstyle.size );
                fl_draw( _pData->getValue( R,  c ), 
                         X + 2, Y + 2, W - 4, H - 4, FL_ALIGN_LEFT );
            }
            
            fl_pop_clip( );
            return;
        }
        case CONTEXT_RC_RESIZE: // table resizing rows or columns
           
            return;
        default:
            return;
    }
}

void SimpleTable::checkEmptySpaceAndFill() {
    int w = getAllColumnsWidth( false );
//    fprintf( stderr, 
//             "SimpleTable::checkEmptySpaceAndFill -- all cols width: %d, tiw:%d ==> delta: %d\n", 
//                     w, tiw, tiw - w );
    //if( w < tiw ) {
        int d = tiw - w;
        int lastColIdx = cols()-1;
        Fl_Table_Copy::col_width( lastColIdx, col_width( lastColIdx ) + d );
        redraw();
    //}
}

void SimpleTable::onScrollStatic( Fl_Widget *w, void *u ) {
    Fl_Table_Copy::scroll_cb( w, u );
    ((SimpleTable*)u)->onScroll( (Fl_Scrollbar*)w );
}

void SimpleTable::onScroll( Fl_Scrollbar *pScroll ) {
    if( _scrollCallback ) {
        char c = ( pScroll == vscrollbar ) ? FL_VERTICAL : FL_HORIZONTAL;
        int val = pScroll->value();
        (*_scrollCallback)( c, val, _pScrollUserData );
    }
}

void SimpleTable::doSelectionCallback( TableContext context ) {
    if( _selectionCallback ) {
        int r1, c1, r2, c2;
        get_selection( r1, c1, r2, c2 );
        (*_selectionCallback) ( context, r1, c1, r2, c2, _pSelectionUserData );
    }
    
}

/**only for backward compatibility purposes*/
Fl_Color SimpleTable::getCellBackground( int row, int col, bool isSelected ) const {
	return _pCellStyleProvider->getBackgroundStyle( row, col, getModelIndex( col ),
	isSelected ).backColor;
}

int SimpleTable::getModelIndex( int viewIndex ) const {
    for( auto itr = _indexRelations.begin(); itr != _indexRelations.end(); itr++
    ) {
        const IndexRel &ixRel = *itr; 
        if( ixRel.viewIdx == viewIndex ) {
            return ixRel.modelIdx;
        }
    }
    return -1;
}

int SimpleTable::getViewIndex( int modelIndex ) const {
	for( auto itr = _indexRelations.begin(); itr != _indexRelations.end(); itr++ ) {
		const IndexRel &ixRel = *itr;
		if( ixRel.modelIdx == modelIndex ) {
			return ixRel.viewIdx;
		}
	}
	return -1;
}

void SimpleTable::setAlternatingColumnColor( Fl_Color color ) {
	_pCellStyleProvider->setAlternatingColumnColor( color );
}

void SimpleTable::setAlternatingRowColor( Fl_Color color ) {
	_pCellStyleProvider->setAlternatingRowColor( color );
}

int SimpleTable::getVScrollbarWidth() const {
    return vscrollbar->visible() ? vscrollbar->w() : 0;
}

int SimpleTable::getAllColumnsWidth( bool includeRowHeader) {
    int w = ( includeRowHeader && row_header() ) ? row_header_width() : 0;
    for( int c = 0, cmax = cols(); c < cmax; c++ ) {
        w += col_width( c );
    }
    return w;
}

bool SimpleTable::isVScrollbarVisible() const {
    return vscrollbar->visible();
}

void SimpleTable::setScrollCallback( ScrollCallback cb, void *userData ) {
    _scrollCallback = cb;
    _pScrollUserData = userData;
}

void SimpleTable::setScrollValue( char orientation, int newValue ) {
    if( orientation == FL_VERTICAL ) {
        vscrollbar->value( newValue );
    } else {
        hscrollbar->value( newValue );
    }
    table_scrolled();
}

void SimpleTable::setSelectionCallback( SelectionCallback cb, void *pUserData ) {
    _selectionCallback = cb;
    _pSelectionUserData = pUserData;
}

void SimpleTable::setResizeCallback( ResizeCallback cb, void *pUserData ) {
    _resizeCallback = cb;
    _pResizeUserData = pUserData;
}

void SimpleTable::resize(int x, int y, int w, int h) {
    Fl_Table_Copy::resize( x, y, w, h );
    if( _resizeMode == RESIZEMODE_LAST_COL ) {
       //checkEmptySpaceAndFill();
    } else if( _resizeMode == RESIZEMODE_ALL_COLS ) {
        //makeColumnsFit();
    }
    if( _resizeCallback ) {
        ( *_resizeCallback ) ( x, y, w, h, _pResizeUserData );
    }
}

void SimpleTable::makeColumnsFit() {
	if( cols() == 0 ) return;
    int max = cols();
    int colW = tiw / max;
    int usedW = 0;
    max--;
    for( int c = 0; c < max; c++ ) {
        col_width( c, colW );
        usedW += colW;
    }
    col_width( max, tiw - usedW );
    redraw();
}

void SimpleTable::adaptColumnWidthToContent( bool skipHiddenColumns ) {
	Fl_Font f = fl_font();
    int fs = fl_size();
    
    for( int c = 0, cmax = cols(); c < cmax; c++ ) {
		fl_font( labelfont(), _pCellStyleProvider->getFontAndColor( -1, c,
		getModelIndex( c ) ).size );
		if( col_width( c ) > 0 || !skipHiddenColumns ) {
			colWidth2Content( c );
		}
    }
    
    redraw();

    fl_font(f, fs);
}

void SimpleTable::adaptColumnWidthToContent( int colIdx ) {
	Fl_Font f = fl_font();
	int fs = fl_size();
	fl_font( labelfont(), _pCellStyleProvider->getFontAndColor( -1, colIdx,
	getModelIndex( colIdx ) ).size );

	colWidth2Content( colIdx );

	redraw();

	fl_font( f, fs );
}

void SimpleTable::colWidth2Content( int colIdx ) {
	int maxw = 0, w = 0, h = 0;
	fl_measure( _pData->getColumnHeader( getModelIndex( colIdx ) ), maxw, h, false
	);
	maxw += 4; //some extra space for the column header

	for( int r = 0, rmax = rows(); r < rmax; r++ ) {
		w = 0, h = 0;
		fl_measure( _pData->getValue( r, getModelIndex( colIdx ) ), w, h, 1 );
		maxw = ( w > maxw ) ? w : maxw;
	}

	col_width( colIdx, maxw + 6 );
}


void SimpleTable::releaseTableData() {
	_pData = NULL;
	rows( 0 );
	cols( 0 );
	_indexRelations.clear();
	
	redraw();
}

/** adds a Menu Item before the ROWS-Item */
void SimpleTable::addCellPopupItem( const char *pLabel, int shortcut, int flags,
                                    PopupMenuCallback cb, int id, void *pUserdata ) 
{
    MenuItemEx *ex = new MenuItemEx();
    ex->label = pLabel;
    ex->shortcut = shortcut;
    ex->flags = flags;
    ex->id = id;
    ex->userdata = pUserdata;
    ex->cb = cb;
    
    auto itr = _cellPopItems.begin();
    for( ; itr != _cellPopItems.end(); itr++ ) {
        MenuItemEx *pItem = *itr;
        if( !strcmp( pItem->label.c_str(), ROWS ) ) {
            _cellPopItems.insert( itr, ex );
            return;
        }
    }
    
    _cellPopItems.push_back( ex );
}

void SimpleTable::showPopup( TableContext context, int x, int y, int r, int c ) {
    if( context == CONTEXT_COL_HEADER ) {
        showColumnHeaderPopup( x, y, c );
    } else if( context == CONTEXT_CELL ) {
        handleCellPopup( x, y, r, c );
    }
}

void SimpleTable::showColumnHeaderPopup( int x, int y, int c  ) {
    
}

void SimpleTable::handleCellPopup( int x, int y, int r, int c ) {
    Fl_Menu_Button btn( 0, 0,0,0 );
    for_each( _cellPopItems.begin(), _cellPopItems.end(), [&]( MenuItemEx *pItem ) {
        string label = pItem->label;
        if( _pData && label == ROWS  ) {
            label.append( to_string( _pData->getRowCount() ) );
        }
        btn.add( label.c_str(), 
                 pItem->shortcut, 
                 (Fl_Callback*) 0, 
                 pItem->userdata, 
                 pItem->flags );
    });
    
    btn.position( x, y );
    const Fl_Menu_Item *pSel = btn.popup();
    
    if( pSel ) {
        switch( pSel->shortcut() ) {
            case FL_CTRL+'c':
                copySelectionToClipboard();
                break;
            case FL_CTRL+'p':
                break;
            case FL_CTRL+'f':
                search();
                break;
            default:
                //custom menu item
                for( auto itr = _cellPopItems.begin(); 
                     itr != _cellPopItems.end(); itr++ ) 
                {
                    MenuItemEx *pItem = *itr;
                    if( pItem->cb && !strcmp( pItem->label.c_str(), pSel->label() ) ) {
                        (pItem->cb)( CONTEXT_CELL, pItem->id, r, getModelIndex( c ), pItem->userdata );
                    }
                }
                break;
        }
    }
}

const char *NEXTROW = "\n"; //separator for clipboard actions
const char *NEXTCOL = "\t"; 

void SimpleTable::copySelectionToClipboard() {
    int r1, c1, r2, c2;
    get_selection( r1, c1, r2, c2 );
    string cp;
    for( int r = r1; r <= r2; r++ ) {
        for( int c = c1; c <= c2; c++ ) {            
            const char *pVal = _pData->getValue( r, getModelIndex( c ) );
            cp.append( pVal );
            if( c < c2 ) cp.append( NEXTCOL );
            else cp.append( NEXTROW );
        }
    }
    Fl::copy( cp.c_str(), cp.size(), 1 );
}

void SimpleTable::search() {
    fprintf( stderr, "searching");
}

SimpleTable::~SimpleTable() { 
    /*TODO: delete DefaultCellStyleProvider, aber NICHT
    einen CustomCellStyleProvider*/
    
    for( auto itr = _cellPopItems.begin(); itr != _cellPopItems.end(); itr++) {
//        _cellPopupItems.erase( itr );
        delete *itr;
    }
}