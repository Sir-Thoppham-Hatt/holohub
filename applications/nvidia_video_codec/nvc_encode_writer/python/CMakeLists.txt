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

cmake_minimum_required(VERSION 3.20)
project(nvidia_video_codec LANGUAGES CXX)

find_package(holoscan 3.0 REQUIRED CONFIG
             PATHS "/opt/nvidia/holoscan" "/workspace/holoscan-sdk/install")

# Enable the operators
add_library(nvidia_video_codec INTERFACE)
target_link_libraries(nvidia_video_codec INTERFACE
    holoscan::core
    holoscan::ops::format_converter
    holoscan::ops::nv_video_encoder
    holoscan::ops::tensor_to_file
    holoscan::ops::video_stream_replayer
)