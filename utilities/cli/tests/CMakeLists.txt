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

include(CTest)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Add functional tests for holohub script
add_test(
    NAME test_holohub_list
    COMMAND ${CMAKE_SOURCE_DIR}/holohub list
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(test_holohub_list PROPERTIES
    PASS_REGULAR_EXPRESSION "APPLICATIONS.*BENCHMARKS.*PACKAGES.*OPERATORS.*WORKFLOWS"
)

add_test(
    NAME test_holohub_autocompletion_list
    COMMAND ${CMAKE_SOURCE_DIR}/holohub autocompletion_list
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(test_holohub_autocompletion_list PROPERTIES
    PASS_REGULAR_EXPRESSION "ultrasound_segmentation.*build.*run.*list"
)


add_test(
    NAME test_holohub_lint
    COMMAND ${CMAKE_SOURCE_DIR}/holohub lint --dryrun
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(test_holohub_lint PROPERTIES
    PASS_REGULAR_EXPRESSION "ruff check --ignore.*isort -c.*black --check.*cpplint.*codespell.*cmakelint"
)
add_test(
    NAME test_holohub_lint_fix
    COMMAND ${CMAKE_SOURCE_DIR}/holohub lint --fix --dryrun
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(test_holohub_lint_fix PROPERTIES
    PASS_REGULAR_EXPRESSION "ruff check --fix.*isort.*black.*codespell.*cpplint"  # TODO: cmakelint missing
)

add_test(
    NAME test_holohub_setup
    COMMAND ${CMAKE_SOURCE_DIR}/holohub setup --dryrun
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(test_holohub_setup PROPERTIES
    PASS_REGULAR_EXPRESSION "(already installed|apt install)"
    FAIL_REGULAR_EXPRESSION "returned non-zero exit status;failed;Traceback;Error"
)

find_package(CUDAToolkit QUIET)
if(CUDAToolkit_FOUND)
    get_test_property(test_holohub_setup PASS_REGULAR_EXPRESSION pass_expression)
    set_tests_properties(test_holohub_setup PROPERTIES
        PASS_REGULAR_EXPRESSION "${pass_expression}.*(cudnn|nvinfer|nvonnxparsers)"
        FAIL_REGULAR_EXPRESSION "returned non-zero exit status;failed;Traceback;Error checking available versions"
    )
endif()

add_test(
    NAME test_holohub_clear_cache
    COMMAND ${CMAKE_SOURCE_DIR}/holohub clear-cache --dryrun
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_clear_cache PROPERTY
    PASS_REGULAR_EXPRESSION "Would remove:.*${CMAKE_SOURCE_DIR}/build"
)

add_test(
    NAME test_holohub_build_container_dryrun
    COMMAND ${CMAKE_SOURCE_DIR}/holohub build-container --dryrun --no-cache
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_build_container_dryrun PROPERTY
    PASS_REGULAR_EXPRESSION "docker build"
)

add_test(
    NAME test_holohub_build_container_build_args
    COMMAND ${CMAKE_SOURCE_DIR}/holohub build-container --dryrun --build-args "--build-arg TEST=value"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_build_container_build_args PROPERTY
    PASS_REGULAR_EXPRESSION "docker build"
    PASS_REGULAR_EXPRESSION "--build-arg TEST=value"
)

add_test(
    NAME test_holohub_run_container_docker_opts
    COMMAND ${CMAKE_SOURCE_DIR}/holohub run-container --dryrun --docker-opts "--memory 4g --entrypoint=bash"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_run_container_docker_opts PROPERTY
    PASS_REGULAR_EXPRESSION "docker run"
    PASS_REGULAR_EXPRESSION "--memory 4g"
)

add_test(
    NAME test_holohub_run_container_add_volume
    COMMAND ${CMAKE_SOURCE_DIR}/holohub run-container --dryrun --add-volume "/tmp:/tmp"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_run_container_add_volume PROPERTY
    PASS_REGULAR_EXPRESSION "docker run"
    PASS_REGULAR_EXPRESSION "-v /tmp:/tmp"
)

add_test(
    NAME test_holohub_build_dryrun
    COMMAND ${CMAKE_SOURCE_DIR}/holohub build endoscopy_tool_tracking --dryrun
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_build_dryrun PROPERTY
    PASS_REGULAR_EXPRESSION "build endoscopy_tool_tracking.*--local"
)

add_test(
    NAME test_holohub_run_dryrun
    COMMAND ${CMAKE_SOURCE_DIR}/holohub run endoscopy_tool_tracking --dryrun
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_run_dryrun PROPERTY
    PASS_REGULAR_EXPRESSION "run endoscopy_tool_tracking.*--local"
)

add_test(
    NAME test_holohub_run_forward_args
    COMMAND ${CMAKE_SOURCE_DIR}/holohub run endoscopy_tool_tracking --dryrun --run-args=--flag
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_run_forward_args PROPERTY
    PASS_REGULAR_EXPRESSION "run endoscopy_tool_tracking.*--run-args=--flag"
)

add_test(
    NAME test_holohub_run_pythonpath
    COMMAND ${CMAKE_SOURCE_DIR}/holohub run-container --dryrun
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_run_pythonpath PROPERTY
    PASS_REGULAR_EXPRESSION "Inspect docker image PYTHONPATH: docker inspect --format"
)

add_test(
    NAME test_holohub_run_container_dryrun
    COMMAND ${CMAKE_SOURCE_DIR}/holohub run-container --dryrun -- echo hello
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_holohub_run_container_dryrun PROPERTY
    PASS_REGULAR_EXPRESSION "docker run"
    PASS_REGULAR_EXPRESSION "holohub.*echo hello"
)

# Add test for container module
add_test(
    NAME test_container
    COMMAND ${Python3_EXECUTABLE} -m unittest discover -s ${CMAKE_CURRENT_SOURCE_DIR} -p "test_container.py"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Add test for CLI module
add_test(
    NAME test_cli
    COMMAND ${Python3_EXECUTABLE} -m unittest discover -s ${CMAKE_CURRENT_SOURCE_DIR} -p "test_cli.py"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_test(
    NAME test_cli_env_info
    COMMAND ${CMAKE_SOURCE_DIR}/holohub env-info
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_cli_env_info PROPERTY
    PASS_REGULAR_EXPRESSION "HoloHub Environment Variables"
    PASS_REGULAR_EXPRESSION "Holoscan Environment Variables"
    PASS_REGULAR_EXPRESSION "Other Relevant Environment Variables"
)

# test ambiguous dash-prefixed arguments
add_test(
    NAME test_cli_ambiguous_dash_prefixed_arguments
    COMMAND ${CMAKE_SOURCE_DIR}/holohub run-container --dryrun --run-args --local
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TEST test_cli_ambiguous_dash_prefixed_arguments PROPERTY
    PASS_REGULAR_EXPRESSION "ambiguous dash-prefixed arguments"
)
