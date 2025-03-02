/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LUCI_INTERPRETER_PAL_SPACETODEPTH_H
#define LUCI_INTERPRETER_PAL_SPACETODEPTH_H

#include <tensorflow/lite/kernels/internal/reference/reference_ops.h>

namespace luci_interpreter_pal
{
template <typename T>
static inline void SpaceToDepth(const tflite::SpaceToDepthParams &op_params,
                                const tflite::RuntimeShape &unextended_input_shape,
                                const T *input_data,
                                const tflite::RuntimeShape &unextended_output_shape, T *output_data)
{
  tflite::reference_ops::SpaceToDepth(op_params, unextended_input_shape, input_data,
                                      unextended_output_shape, output_data);
}
} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_SPACETODEPTH_H
