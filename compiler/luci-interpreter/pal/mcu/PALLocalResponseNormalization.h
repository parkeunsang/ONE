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

#ifndef LUCI_INTERPRETER_PAL_LOCALRESPONSENORMALIZATION_H
#define LUCI_INTERPRETER_PAL_LOCALRESPONSENORMALIZATION_H

#include <tensorflow/lite/kernels/internal/reference/reference_ops.h>
#include <cassert>

namespace luci_interpreter_pal
{
static inline void
LocalResponseNormalization(const tflite::LocalResponseNormalizationParams &op_params,
                           const tflite::RuntimeShape &input_shape, const float *input_data,
                           const tflite::RuntimeShape &output_shape, float *output_data)
{
  // MARK: At this moment this operation doesn't support on mcu
  (void)op_params;
  (void)input_shape;
  (void)input_data;
  (void)output_shape;
  (void)output_data;
}
} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_LOCALRESPONSENORMALIZATION_H
