if(NOT DEFINED ROUTECRAFT_EXECUTABLE OR NOT EXISTS "${ROUTECRAFT_EXECUTABLE}")
    message(FATAL_ERROR "ROUTECRAFT_EXECUTABLE phải trỏ tới executable routecraft đã build")
endif()
if(NOT DEFINED ROUTECRAFT_FIXTURE_DIR OR NOT IS_DIRECTORY "${ROUTECRAFT_FIXTURE_DIR}")
    message(FATAL_ERROR "ROUTECRAFT_FIXTURE_DIR phải trỏ tới thư mục fixture")
endif()

function(require_contains value expected context)
    string(FIND "${value}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "${context}: đầu ra phải chứa '${expected}', nhưng nhận được '${value}'")
    endif()
endfunction()

function(run_valid_json fixture)
    execute_process(
        COMMAND "${ROUTECRAFT_EXECUTABLE}" solve --input "${ROUTECRAFT_FIXTURE_DIR}/${fixture}" --method nn --format json
        RESULT_VARIABLE result
        OUTPUT_VARIABLE stdout
        ERROR_VARIABLE stderr
    )
    if(NOT result EQUAL 0 OR NOT stderr STREQUAL "")
        message(FATAL_ERROR "${fixture}: JSON phải thành công, exit=${result}, stderr='${stderr}'")
    endif()
    require_contains("${stdout}" "\"feasible\": true" "${fixture} JSON")
endfunction()

function(require_axis_distance first_x first_y second_x second_y result_variable)
    math(EXPR delta_x "${first_x} - ${second_x}")
    math(EXPR delta_y "${first_y} - ${second_y}")
    if(delta_x LESS 0)
        math(EXPR delta_x "-${delta_x}")
    endif()
    if(delta_y LESS 0)
        math(EXPR delta_y "-${delta_y}")
    endif()
    if(NOT delta_x EQUAL 0 AND NOT delta_y EQUAL 0)
        message(FATAL_ERROR "Fixture toy phải giữ các cạnh được kiểm tra song song một trục")
    endif()
    math(EXPR ${result_variable} "${delta_x} + ${delta_y}")
    set(${result_variable} "${${result_variable}}" PARENT_SCOPE)
endfunction()

function(independent_route_metrics source_ids result_load result_distance)
    set(load 0)
    set(distance 0)
    list(LENGTH source_ids count)
    math(EXPR last "${count} - 1")
    foreach(index RANGE 0 ${last})
        list(GET source_ids ${index} source_id)
        math(EXPR load "${load} + ${demand_${source_id}}")
        if(index GREATER 0)
            math(EXPR previous_index "${index} - 1")
            list(GET source_ids ${previous_index} previous_id)
            require_axis_distance(
                "${x_${previous_id}}" "${y_${previous_id}}"
                "${x_${source_id}}" "${y_${source_id}}" edge_distance)
            math(EXPR distance "${distance} + ${edge_distance}")
        endif()
    endforeach()
    set(${result_load} "${load}" PARENT_SCOPE)
    set(${result_distance} "${distance}" PARENT_SCOPE)
endfunction()

# Chạy đầy đủ fixture hợp lệ qua CLI; từng fixture đại diện một biên Sprint 1.
foreach(fixture IN ITEMS
    sprint1-toy.vrp
    sprint1-one-customer.vrp
    sprint1-full-capacity.vrp
    sprint1-noncontiguous-ids.vrp
    sprint1-depot-not-first.vrp)
    run_valid_json("${fixture}")
endforeach()

execute_process(
    COMMAND "${ROUTECRAFT_EXECUTABLE}" solve --input "${ROUTECRAFT_FIXTURE_DIR}/sprint1-toy.vrp" --method nn --format json
    RESULT_VARIABLE toy_json_result
    OUTPUT_VARIABLE toy_json_stdout
    ERROR_VARIABLE toy_json_stderr
)
execute_process(
    COMMAND "${ROUTECRAFT_EXECUTABLE}" solve --input "${ROUTECRAFT_FIXTURE_DIR}/sprint1-toy.vrp" --method nn --format txt
    RESULT_VARIABLE toy_txt_result
    OUTPUT_VARIABLE toy_txt_stdout
    ERROR_VARIABLE toy_txt_stderr
)
if(NOT toy_json_result EQUAL 0 OR NOT toy_json_stderr STREQUAL "" OR
   NOT toy_txt_result EQUAL 0 OR NOT toy_txt_stderr STREQUAL "")
    message(FATAL_ERROR "CLI toy phải xuất cả JSON và TXT mà không có chẩn đoán lỗi")
endif()

# Số liệu dưới đây được tính độc lập từ tọa độ và demand fixture, không lấy từ solver/exporter.
set(x_100 0)
set(y_100 0)
set(demand_100 0)
set(x_8 3)
set(y_8 0)
set(demand_8 4)
set(x_41 5)
set(y_41 0)
set(demand_41 6)
set(x_17 0)
set(y_17 4)
set(demand_17 3)
set(x_90 0)
set(y_90 7)
set(demand_90 7)
set(x_29 -2)
set(y_29 0)
set(demand_29 5)
set(x_75 -5)
set(y_75 0)
set(demand_75 5)

independent_route_metrics("100;29;75;100" load_1 distance_1)
independent_route_metrics("100;8;41;100" load_2 distance_2)
independent_route_metrics("100;17;90;100" load_3 distance_3)
math(EXPR total_distance "${distance_1} + ${distance_2} + ${distance_3}")
if(NOT load_1 EQUAL 10 OR NOT load_2 EQUAL 10 OR NOT load_3 EQUAL 10 OR NOT total_distance EQUAL 34)
    message(FATAL_ERROR "Phép tính độc lập của toy cho kết quả không như mong đợi")
endif()

require_contains("${toy_json_stdout}" "\"routes\": [[0, 5, 6, 0], [0, 1, 2, 0], [0, 3, 4, 0]]" "toy JSON routes")
require_contains("${toy_json_stdout}" "\"route_loads\": [${load_1}, ${load_2}, ${load_3}]" "toy JSON loads")
require_contains("${toy_json_stdout}" "\"route_distances\": [${distance_1}, ${distance_2}, ${distance_3}]" "toy JSON distances")
require_contains("${toy_json_stdout}" "\"total_distance\": ${total_distance}" "toy JSON total")
require_contains("${toy_txt_stdout}" "Route 1: 100 -> 29 -> 75 -> 100 | Load: ${load_1} | Distance: ${distance_1}" "toy TXT route 1")
require_contains("${toy_txt_stdout}" "Route 2: 100 -> 8 -> 41 -> 100 | Load: ${load_2} | Distance: ${distance_2}" "toy TXT route 2")
require_contains("${toy_txt_stdout}" "Route 3: 100 -> 17 -> 90 -> 100 | Load: ${load_3} | Distance: ${distance_3}" "toy TXT route 3")
require_contains("${toy_txt_stdout}" "Total distance: ${total_distance}" "toy TXT total")

execute_process(
    COMMAND "${ROUTECRAFT_EXECUTABLE}" solve --input "${ROUTECRAFT_FIXTURE_DIR}/sprint1-demand-exceeds-capacity.vrp" --method nn --format json
    RESULT_VARIABLE invalid_result
    OUTPUT_VARIABLE invalid_stdout
    ERROR_VARIABLE invalid_stderr
)
if(NOT invalid_result EQUAL 1 OR NOT invalid_stdout STREQUAL "")
    message(FATAL_ERROR "Demand vượt tải phải bị CLI từ chối với exit code 1 và không có stdout")
endif()
require_contains("${invalid_stderr}" "CAPACITY" "demand vượt tải")
