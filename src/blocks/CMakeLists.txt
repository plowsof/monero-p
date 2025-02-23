# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

set(GENERATED_SOURCES "")

set(GENERATOR "${CMAKE_CURRENT_BINARY_DIR}/blocks_generator.cmake")
file(GENERATE OUTPUT ${GENERATOR} CONTENT [=[
file(READ "${INPUT_DAT_FILE}" DATA HEX)
string(REGEX REPLACE "[0-9a-fA-F][0-9a-fA-F]" "0x\\0," DATA "${DATA}")
file(WRITE "${OUTPUT_C_SOURCE}" "
#include <stddef.h>
const unsigned char ${BLOB_NAME}[]={
  ${DATA}
};
const size_t ${BLOB_NAME}_len = sizeof(${BLOB_NAME});
"
)
]=])
foreach(BLOB_NAME checkpoints testnet_blocks stagenet_blocks)
    set(OUTPUT_C_SOURCE "generated_${BLOB_NAME}.c")
    list(APPEND GENERATED_SOURCES ${OUTPUT_C_SOURCE})
    set(INPUT_DAT_FILE "${BLOB_NAME}.dat")
    add_custom_command(
      OUTPUT ${OUTPUT_C_SOURCE}
      MAIN_DEPENDENCY ${INPUT_DAT_FILE}
      DEPENDS ${GENERATOR}
      COMMAND ${CMAKE_COMMAND}
        "-DINPUT_DAT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_DAT_FILE}"
        "-DBLOB_NAME=${BLOB_NAME}"
        "-DOUTPUT_C_SOURCE=${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_C_SOURCE}"
        -P "${GENERATOR}"
    )
endforeach()

monero_add_library(blocks blocks.cpp ${GENERATED_SOURCES})
