cmake_minimum_required(VERSION 3.9)
project(mpi)

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

#add_custom_target(mpi_intro main.c)
add_executable(mpi_intro main.c)
add_executable(mpi_broadcast broadcast.c)
add_executable(mpi_reduce reduce.c)
add_executable(mpi_reduce2all reduce2all.c)
add_executable(mpi_gather gather.c)
add_executable(mpi_gatherv gather_v.c)
add_executable(mpi_scatterv scatter_v.c)
add_executable(mpi_scan scan.c)
add_executable(mpi_complexprod userop.c)
add_executable(mpi_ping_pong ping_pong.c)
add_executable(mpi_avg non_blocking_averaging.c)
add_executable(mpi_io io.c)
add_executable(mpi_computation_on_matrix computation_on_matrix.c)
add_executable(mpi_pi_approx pi_approx.c)



target_link_libraries(mpi_intro ${MPI_C_LIBRARIES})
target_link_libraries(mpi_broadcast ${MPI_C_LIBRARIES})
target_link_libraries(mpi_reduce ${MPI_C_LIBRARIES})
target_link_libraries(mpi_reduce2all ${MPI_C_LIBRARIES})
target_link_libraries(mpi_gather ${MPI_C_LIBRARIES})
target_link_libraries(mpi_gatherv ${MPI_C_LIBRARIES})
target_link_libraries(mpi_scatterv ${MPI_C_LIBRARIES})
target_link_libraries(mpi_scan ${MPI_C_LIBRARIES})
target_link_libraries(mpi_complexprod ${MPI_C_LIBRARIES})
target_link_libraries(mpi_ping_pong ${MPI_C_LIBRARIES})
target_link_libraries(mpi_avg ${MPI_C_LIBRARIES})
target_link_libraries(mpi_io ${MPI_C_LIBRARIES})
target_link_libraries(mpi_computation_on_matrix ${MPI_C_LIBRARIES})
target_link_libraries(mpi_pi_approx ${MPI_C_LIBRARIES})



