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

import bpy

from bpy.props import StringProperty

from ctypes import c_float, c_uint32
from array import array
import numpy

from mathutils import Vector
from itertools import chain

def average_tangent(me):
    # tangents have to be pre-calculated
    # this will also calculate loop normal
    me.calc_tangents()

    avg_tan = []
    avg_bt = []
    for _ in range(len(me.vertices)):
        avg_tan.append([Vector([0.0, 0.0, 0.0]), 0])
        avg_bt.append([Vector([0.0, 0.0, 0.0]), 0])
    # loop faces
    for face in me.polygons:
        # loop over face loop
        tan = Vector([0.0, 0.0, 0.0])
        bt = Vector([0.0, 0.0, 0.0])
        
        for vert in [me.loops[i] for i in face.loop_indices]:
            tan += vert.tangent
            bt += vert.bitangent

        tan /= len(face.loop_indices)
        bt /= len(face.loop_indices)
        for i in [me.loops[i].vertex_index for i in face.loop_indices]:
            avg_tan[i][0] += tan
            avg_tan[i][1] += 1
            avg_bt[i][0] += bt
            avg_bt[i][1] += 1

    #Correct Tangent
    for tan in avg_tan:
        tan[0] /= tan[1]
    for bt in avg_bt:
        bt[0] /= bt[1]

    return avg_tan, avg_bt

class VBBExporter(bpy.types.Operator):
    """
    Export to the VBB format (.vbb)
    """
    bl_idname = "export.vbb"
    bl_label = "Export VBB"
    filepath = StringProperty(subtype='FILE_PATH')


    def exportVBB(self, context, fpath, settings):
        f = open(fpath, "wb")

        print("Exporting VBB to %s" % fpath)
        print("n-th vertex group must correspond to the n-th bone")

        MAX_BONE_WEIGHTS = 4

        obj = context.active_object
        me = obj.data

        index = {
            "VBF_EBO":          1,
            "VBF_VERTEX":       2,
            "VBF_NORMAL":       3,
            "VBF_UV":	        4,
            "VBF_COLOR":        5,
            "VBF_TANGENT":      6,
            "VBF_BITANGENT":    7,
            "VBF_BONE_WEIGHT":  8,
            "VBF_BONE_INDEX":   9
        }

        if (len(me.uv_layers)) == 0:
            raise ValueError("No active UV layer")

        uv_layer = me.uv_layers.active.data

        # EBO, VERTEX, UV, NORMAL, BONE_WEIGHT, BONE_INDEX

        vert = array("f")
        norm = array("f")
        bone_weight = array("f")
        bone_index = array("L")
        ebo = array("L")
        uv = array("f", [0.0 for _ in range(len(me.vertices) * 2)])

        for v in me.vertices:
            vert.extend(v.co)
            norm.extend(v.normal)
            zero_padding = MAX_BONE_WEIGHTS - len(v.groups)

            groups = sorted(v.groups,
                    key=lambda x: x.weight,
                    reverse=True)[:4]
            s = sum(g.weight for g in groups)

            for g in groups:
                bone_weight.append(g.weight/s)
                bone_index.append(g.group)
            for _ in range(zero_padding):
                bone_weight.append(0.0)
                bone_index.append(0)

        for poly in me.polygons:
            if len(poly.loop_indices) != 3:
                raise ValueError("Non triangle face found")

            for loop_index in poly.loop_indices:
                v_ind = me.loops[loop_index].vertex_index
                ebo.append(v_ind)
                uv[2 * v_ind] = uv_layer[loop_index].uv[0]
                uv[2 * v_ind + 1] = uv_layer[loop_index].uv[1]

        avg_tan, avg_bt = average_tangent(me)
        avg_tan = [i for t in avg_tan for i in t[0]]
        avg_bt = [i for t in avg_bt for i in t[0]]
        tangent = array("f", avg_tan)
        bitangent = array("f", avg_bt)

        num_bones = len(obj.vertex_groups)
        print("%d bones found" % num_bones)

        export_list = [
                ["VBF_VERTEX", vert, numpy.float32],
                ["VBF_EBO", ebo, numpy.uint32],
                ["VBF_UV", uv, numpy.float32],
                ["VBF_NORMAL", norm, numpy.float32],
                ["VBF_TANGENT", tangent, numpy.float32],
                ["VBF_BITANGENT", bitangent, numpy.float32],
                ]
        if num_bones:
            export_list.extend([
                ["VBF_BONE_WEIGHT", bone_weight, numpy.float32],
                ["VBF_BONE_INDEX", bone_index, numpy.float32], #glsl supports only float vectors
                ])
        else:
            print("Skipping export of VBF_BONE_WEIGHT and VBF_BONE_INDEX")

        f.write(bytes(c_uint32(len(export_list))))
        f.write(bytes(c_uint32(num_bones)))

        for e in export_list:
            print("%s: %d" % (e[0], len(e[1])))
            f.write(bytes(c_uint32(len(e[1]))))
            f.write(bytes(c_uint32(index[e[0]])))
            f.write(numpy.array(e[1], dtype=e[2]).tobytes())

    def execute(self, context):
        filePath = bpy.path.ensure_ext(self.filepath, ".vbb")
        config = {}

        self.exportVBB(context, filePath, config)
        return {'FINISHED'}

    def invoke(self, context, event):
        if not self.filepath:
            self.filepath = bpy.path.ensure_ext(bpy.data.filepath, ".vbb")
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {'RUNNING_MODAL'}

def menu_func(self, context):
    self.layout.operator(VBBExporter.bl_idname, text="Vertex Buffers and Bones (.vbb)")
