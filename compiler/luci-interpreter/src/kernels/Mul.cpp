/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#include "kernels/Mul.h"

#include "kernels/BinaryOpCommon.h"
#include "kernels/Utils.h"

#include "PALMul.h"

#include <stdexcept>

namespace luci_interpreter
{
namespace kernels
{

Mul::Mul(const Tensor *input1, const Tensor *input2, Tensor *output, const MulParams &params)
  : KernelWithParams<MulParams>({input1, input2}, {output}, params)
{
}

void Mul::configure()
{
  LUCI_INTERPRETER_CHECK(input1()->element_type() == input2()->element_type());
  LUCI_INTERPRETER_CHECK(output()->element_type() == input1()->element_type());
  if (input1()->element_type() == DataType::S16)
  {
    LUCI_INTERPRETER_CHECK(input1()->zero_point() == 0 && input2()->zero_point() == 0 &&
                           output()->zero_point() == 0);
  }

  output()->resize(calculateShapeForBroadcast(input1()->shape(), input2()->shape()));
}

void Mul::execute() const
{
  switch (input1()->element_type())
  {
    case DataType::FLOAT32:
      evalFloat();
      break;
    case DataType::S16:
      evalQuantizedS16();
      break;
    default:
      throw std::runtime_error("Unsupported type.");
  }
}

void Mul::evalFloat() const
{
  float activation_min{};
  float activation_max{};
  calculateActivationRange(_params.activation, &activation_min, &activation_max);

  tflite::ArithmeticParams params{};
  params.float_activation_min = activation_min;
  params.float_activation_max = activation_max;

  const bool need_broadcast = tflite::reference_ops::ProcessBroadcastShapes(
    getTensorShape(input1()), getTensorShape(input2()), &params);

  if (need_broadcast)
  {
    luci_interpreter_pal::BroadcastMul4DSlow(
      params, getTensorShape(input1()), getTensorData<float>(input1()), getTensorShape(input2()),
      getTensorData<float>(input2()), getTensorShape(output()), getTensorData<float>(output()));
  }
  else
  {
    luci_interpreter_pal::Mul(params, getTensorShape(input1()), getTensorData<float>(input1()),
                              getTensorShape(input2()), getTensorData<float>(input2()),
                              getTensorShape(output()), getTensorData<float>(output()));
  }
}

void Mul::evalQuantizedS16() const
{
  const auto input1_scale = static_cast<double>(input1()->scale());
  const auto input2_scale = static_cast<double>(input2()->scale());
  const auto output_scale = static_cast<double>(output()->scale());

  const double real_multiplier = input1_scale * input2_scale / output_scale;

  int32_t output_multiplier;
  int output_shift;
  quantizeMultiplier(real_multiplier, &output_multiplier, &output_shift);

  int32_t activation_min{};
  int32_t activation_max{};
  calculateActivationRangeQuantized(_params.activation, output(), &activation_min, &activation_max);

  auto fn = [output_multiplier, output_shift, activation_min, activation_max](int16_t input1_val,
                                                                              int16_t input2_val) {
    int32_t output = static_cast<int32_t>(input1_val) * static_cast<int32_t>(input2_val);
    output = tflite::MultiplyByQuantizedMultiplier(output, output_multiplier, output_shift);
    output = std::max(output, activation_min);
    output = std::min(output, activation_max);
    return static_cast<int16_t>(output);
  };

  BinaryOpBroadcastSlow(getTensorShape(input1()), getTensorData<int16_t>(input1()),
                        getTensorShape(input2()), getTensorData<int16_t>(input2()),
                        getTensorShape(output()), getTensorData<int16_t>(output()), fn);
}

} // namespace kernels
} // namespace luci_interpreter
