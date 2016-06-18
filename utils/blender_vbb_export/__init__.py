#  ***** GPL LICENSE BLOCK *****
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  All rights reserved.
#  ***** GPL LICENSE BLOCK *****

bl_info = {
    "name": "Export Vertex Buffers and Bones Format (.vbb)",
    "author": "Michał Nieznański",
    "version": (1, 0, 1),
    "blender": (2, 76, 0),
    "location": "File > Export > Vertex Buffers and Bones (.vbb)",
    "description": "The script exports Blender mesh to VBB format.",
    "category": "Import-Export",
}

if "bpy" in locals():
    import importlib
    importlib.reload(VBBExport)
else:
    from VBB import VBBExport

import bpy

E = VBBExport.VBBExporter

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(VBBExport.menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(VBBExport.menu_func)

if __name__ == "__main__":
    register()
