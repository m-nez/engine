add_library(physics 
	col_object.c
	col_object.h
	col_world.c
	col_world.h
	col_shape.c
	col_shape.h
	col_detect.h
	col_detect_inl.h
	col_res.h
	col_res_inl.h
	)

target_include_directories(physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
