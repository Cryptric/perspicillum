add_executable(quick_plot quick_plot.cpp)
target_link_libraries(quick_plot perspicillum)

add_executable(figure_api figure_api.cpp)
target_link_libraries(figure_api perspicillum)

add_executable(data_types data_types.cpp)
target_link_libraries(data_types perspicillum)

add_executable(live_example live_example.cpp)
target_link_libraries(live_example perspicillum)

add_executable(heatmap heatmap.cpp)
target_link_libraries(heatmap perspicillum)

add_executable(heatmap_large heatmap_large.cpp)
target_link_libraries(heatmap_large perspicillum)
