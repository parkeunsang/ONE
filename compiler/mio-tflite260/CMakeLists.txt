nnas_find_package(FlatBuffers QUIET)

if(NOT FlatBuffers_FOUND)
  message(STATUS "Build mio-tflite260: FAILED (missing Flatbuffers)")
  return()
endif(NOT FlatBuffers_FOUND)

# TODO change to 2.6.0
nnas_find_package(TensorFlowSource EXACT 2.3.0 QUIET)

if(NOT TensorFlowSource_FOUND)
  message(STATUS "Build mio-tflite260: FAILED (missing TensorFlowSource 2.6.0)")
  return()
endif(NOT TensorFlowSource_FOUND)

message(STATUS "Build mio-tflite260: TRUE")

set(SCHEMA_FILE "${TensorFlowSource_DIR}/tensorflow/lite/schema/schema.fbs")

# NOTE Use copy of schema.fbs as to provide unified way for circle also
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/schema.fbs"
  COMMAND ${CMAKE_COMMAND} -E copy "${SCHEMA_FILE}" schema.fbs
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  DEPENDS "${SCHEMA_FILE}"
)

FlatBuffers_Target(mio_tflite260
  OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/gen/mio/tflite"
  INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/gen"
  SCHEMA_DIR "${CMAKE_CURRENT_BINARY_DIR}"
  SCHEMA_FILES "schema.fbs"
)

add_executable(mio_tflite260_example example.cpp)
target_link_libraries(mio_tflite260_example mio_tflite260)

# Temporay tflite validation tool to replace nnkit-tflite
# TODO provide full tflite validation with runtime/interpreter
add_executable(mio_tflite260_validate example.cpp)
target_link_libraries(mio_tflite260_validate mio_tflite260)

# TODO change to 2.6.0
nnas_find_package(TensorFlowGEMMLowpSource EXACT 2.3.0 QUIET)

if(NOT TensorFlowGEMMLowpSource_FOUND)
  return()
endif(NOT TensorFlowGEMMLowpSource_FOUND)

add_library(mio_tflite260_inc INTERFACE)
target_include_directories(mio_tflite260_inc SYSTEM INTERFACE "${TensorFlowSource_DIR}")
target_include_directories(mio_tflite260_inc SYSTEM INTERFACE "${TensorFlowGEMMLowpSource_DIR}")
