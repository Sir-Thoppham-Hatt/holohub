/*
 * SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * CUDA driver API error check helper
 */
#define CudaCheck(FUNC)                                                                     \
  {                                                                                         \
    const CUresult result = FUNC;                                                           \
    if (result != CUDA_SUCCESS) {                                                           \
      const char *error_name = "";                                                          \
      cuGetErrorName(result, &error_name);                                                  \
      const char *error_string = "";                                                        \
      cuGetErrorString(result, &error_string);                                              \
      std::stringstream buf;                                                                \
      buf << "[" << __FILE__ << ":" << __LINE__ << "] CUDA driver error " << result << " (" \
          << error_name << "): " << error_string;                                           \
      throw std::runtime_error(buf.str().c_str());                                          \
    }                                                                                       \
  }
#define CUDA_TRY(stmt)                                                                       \
  {                                                                                          \
    cudaError_t cuda_status = stmt;                                                          \
    if (cudaSuccess != cuda_status) {                                                        \
      HOLOSCAN_LOG_ERROR("CUDA runtime call {} in line {} of file {} failed with '{}' ({})", \
                         #stmt,                                                              \
                         __LINE__,                                                           \
                         __FILE__,                                                           \
                         cudaGetErrorString(cuda_status),                                    \
                         int(cuda_status));                                                  \
      throw std::runtime_error("CUDA runtime call failed");                                  \
    }                                                                                        \
  }

#endif /* UTILS_H */
