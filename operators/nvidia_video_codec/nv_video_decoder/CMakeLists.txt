# SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.24)
project(nv_video_decoder LANGUAGES CXX)

include(nvidia_video_codec)

find_package(holoscan 3.0 REQUIRED CONFIG
             PATHS "/opt/nvidia/holoscan" "/workspace/holoscan-sdk/install")

# Decoder Operator
add_library(nv_video_decoder SHARED
  nv_video_decoder.cpp
  ${NVC_SDK_PATH}/Logger.cpp
  ${NVC_SDK_PATH}/NvDecoder/NvDecoder.cpp
)
add_library(holoscan::ops::nv_video_decoder ALIAS nv_video_decoder)

target_link_libraries(nv_video_decoder PUBLIC
  holoscan::core
  cuda
  avcodec
  avformat
  avutil
  ${NVCUVID_LIBRARY}
  ${NVENC_LIBRARY}
)
target_include_directories(nv_video_decoder PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  ${NVC_SDK_PATH}/
  ${NVC_SDK_PATH}/NvDecoder
)

add_subdirectory(python)
