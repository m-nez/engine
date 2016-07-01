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
    "name": "Export Animation Matrix (.amt)",
    "author": "Michał Nieznański",
    "version": (1, 0, 0),
    "blender": (2, 77, 0),
    "location": "File > Export > Animation Matrix (.amt)",
    "description": "The script exports Blender action to AMT format.",
    "category": "Import-Export",
}

import bpy

from bpy.props import StringProperty

from ctypes import c_float, c_uint32
from array import array
import numpy

def exportAMT(context, filepath, settings):
    fout = open(filepath, "wb")

    print("Exporting AMT to %s" % filepath)

    obj = context.active_object
    scene = context.scene
    action = obj.animation_data.action

    fps = scene.render.fps
    frames = set()

    num_bones = len(obj.pose.bones)

    for fcu in action.fcurves:
        for keyframe in fcu.keyframe_points:
            frames.add(keyframe.co[0])
    frames = sorted(frames)
    print("Exporting matrices during %d keyframes:" % len(frames))
    print(frames)
    times = [i/fps for i in frames]

    matrices = array("f")
    tmp_mat = [array("f") for _ in range(num_bones)]
    bind_inv = [pbone.bone.matrix_local.inverted() for pbone in obj.pose.bones]
    for f in frames:
        scene.frame_set(f)
        for i, pbone in enumerate(obj.pose.bones):
            M = pbone.matrix * bind_inv[i]
            for c in M.col:
                tmp_mat[i].extend(c)

    for i in tmp_mat:
        matrices.extend(i)

    print("Matrices exported: %d" % (len(matrices)/16))
    fout.write(bytes(c_uint32(num_bones)))
    fout.write(bytes(c_uint32(len(times))))
    fout.write(numpy.array(times, dtype=numpy.float32).tobytes())
    fout.write(numpy.array(matrices, dtype=numpy.float32).tobytes())

class AMTExporter(bpy.types.Operator):
    """
    Export to the AMT format (.amt)
    """
    bl_idname = "export.amt"
    bl_label = "Export AMT"
    filepath = StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        filePath = bpy.path.ensure_ext(self.filepath, ".amt")
        config = {}

        exportAMT(context, filePath, config)
        return {'FINISHED'}

    def invoke(self, context, event):
        if not self.filepath:
            self.filepath = bpy.path.ensure_ext(bpy.data.filepath, ".amt")
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {'RUNNING_MODAL'}

def menu_func(self, context):
    self.layout.operator(AMTExporter.bl_idname, text="Animation Matrix (.amt)")

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
