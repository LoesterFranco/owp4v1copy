/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WGML tags :HP0 :HP1, :HP2, :HP3 and :eHPx processing
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"

/***************************************************************************/
/*  allow 20 active hilighting levels                                      */
/***************************************************************************/

static  int hpcount = -1;               // no hilighting active
static  int hp[20] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
#define HPMAXIND  (int)(sizeof( hp ) / sizeof( hp[0] ) - 1)


/***************************************************************************/
/*  common routines for :HPx and :eHPx                                     */
/***************************************************************************/

static void g_err_tag( void )
{
    char            tagn[8];

    sprintf_s( tagn, 8, "%ceHP%d", GML_char, hp[hpcount] );
    g_err( ERR_TAG_EXPECTED, tagn );
    file_mac_info();
    err_count++;
    return;
}

/***************************************************************************/
/*  set_space possibly suppress space if tag follows                       */
/***************************************************************************/

static void set_space( char * p )
{
    char    c;

    if( p_char != NULL ) {
        if( !(input_cbs->fmflags & II_sol) ) {
            c = p_char->text[p_char->count - 1];
            if( !(is_stop_char( c ) | (*p == ' ')) ) {
                 post_space = 0;
            }
        }
    } else {
        if( !(input_cbs->fmflags & (II_eol | II_sol) | (*p == ' ')) ) {
            post_space = 0;
        }
    }
}


static bool gml_hpx_common( const gmltag * entry, int font )
{
    char    *   p;
    bool        result = true;

    if( hpcount >= HPMAXIND ) {
        g_err_tag();                    // all levels active
        result = false;
        return( result );
    }

    if( (hpcount > -1) && (hp[hpcount] == font) ) {
        g_err_tag();                    // same hilighting already active
        result = false;
        return( result );
    }
    hpcount++;
    hp[hpcount] = font;
    g_curr_font_num = font;
    scan_err = false;
    p = scan_start;
    if( *p == '.' ) p++;                // over '.'
    set_space( p );
    if( *p ) {
        process_text( p, font);
    }
    scan_start = scan_stop + 1;
    return( result );
}


/***************************************************************************/
/*  :HP0  :HP1  :HP2  :HP3                                                 */
/***************************************************************************/

void    gml_hp0( const gmltag * entry )
{
    gml_hpx_common( entry, 0 );
}

void    gml_hp1( const gmltag * entry )
{
    gml_hpx_common( entry, 1 );
}

void    gml_hp2( const gmltag * entry )
{
    gml_hpx_common( entry, 2 );
}

void    gml_hp3( const gmltag * entry )
{
    gml_hpx_common( entry, 3 );
}

/***************************************************************************/
/*  :eHPx tags                                                             */
/***************************************************************************/

void    gml_ehpx( const gmltag * entry )
{
    char    *   p;

    if( hpcount < 0 ) {
        g_err( ERR_TAG_EXPECTED, "no :eHPx" );
        file_mac_info();
        err_count++;
    } else {
        if( hpcount > 0 ) {
           g_curr_font_num = hp[hpcount - 1];
        } else {
           g_curr_font_num = layout_work.defaults.font;
        }
        hpcount--;
        scan_err = false;
        p = scan_start;
        if( *p == '.' ) p++;                // over '.'
        set_space( p );
        if( *p ) {
            process_text( p, g_curr_font_num );
        }
    }
    scan_start = scan_stop + 1;
}