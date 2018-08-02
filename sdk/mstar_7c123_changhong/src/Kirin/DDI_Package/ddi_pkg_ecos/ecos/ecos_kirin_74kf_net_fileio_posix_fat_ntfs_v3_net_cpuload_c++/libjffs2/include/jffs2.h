#ifndef CYGONCE_FS_JFFS2_JFFS2_H
#define CYGONCE_FS_JFFS2_JFFS2_H
//========================================================================
//
//      jffs2.h
//
//      Exported definitions and APIs for applications
//
//========================================================================
//===========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2007 Free Software Foundation, Inc.                        
// Copyright (C) 2007 eCosCentric Limited                                   
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//===========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     jlarmour
// Contributors:
// Date:          2007-07-03
// Purpose:       Exported definitions and APIs for direct application use
// Description:   
// Usage:         This is used for JFFS2-specific APIs not accessed by the
//                abstracted filesystem API.
//
//####DESCRIPTIONEND####
//
//========================================================================

#include <pkgconf/fs_jffs2.h>
#include <cyg/fileio/fileio.h>

#ifndef FS_INFO_FSPRIVATE_START // just in case of older fileio package
# define FS_INFO_FSPRIVATE_START 10000
#endif

#ifdef CYGOPT_FS_JFFS2_GC_THREAD
# define FS_INFO_JFFS2_SET_GC_THREAD_TICKS    (FS_INFO_FSPRIVATE_START+0)
# define FS_INFO_JFFS2_GET_GC_THREAD_TICKS    (FS_INFO_FSPRIVATE_START+1)
#endif

#endif // ifndef CYGONCE_FS_JFFS2_JFFS2_H

// EOF jffs2.h
