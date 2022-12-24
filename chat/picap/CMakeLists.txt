cmake_minimum_required(VERSION 3.7)
if(POLICY CMP0079)
	cmake_policy(SET CMP0079 NEW)
endif()

set(CLIENT_SOURCES
	main.cpp
	parse_cl.cpp
	parse_cl.h
)

add_executable(datachannel-picap ${CLIENT_SOURCES})


set_target_properties(datachannel-picap PROPERTIES
	CXX_STANDARD 17
	OUTPUT_NAME picap)

set_target_properties(datachannel-picap PROPERTIES
	XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER com.github.paullouisageneau.libdatachannel.examples.client)

find_package(Threads REQUIRED)
target_link_libraries(datachannel-picap LibDataChannel::LibDataChannel Threads::Threads nlohmann_json)

if(MSVC)
	add_custom_command(TARGET datachannel-picap POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"$<TARGET_FILE_DIR:datachannel>/datachannel.dll"
		$<TARGET_FILE_DIR:datachannel-picap>
	)
endif()
