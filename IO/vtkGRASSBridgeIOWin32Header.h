/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/


#ifndef __vtkGRASSBridgeIOWin32Header_h
#define __vtkGRASSBridgeIOWin32Header_h

#include <vtkGRASSBridgeConfigure.h>

#if defined(WIN32) && !defined(VTK_GRASS_BRIDGE_STATIC)
#if defined(vtkGRASSBridgeIO_EXPORTS)
#define VTK_GRASS_BRIDGE_IO_EXPORT __declspec( dllexport ) 
#else
#define VTK_GRASS_BRIDGE_IO_EXPORT __declspec( dllimport ) 
#endif
#else
#define VTK_GRASS_BRIDGE_IO_EXPORT
#endif

#endif
