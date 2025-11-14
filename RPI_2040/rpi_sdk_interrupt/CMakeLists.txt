# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(PWM_MEASURE C CXX ASM)


# If you want debug output from USB (pass -DPICO_STDIO_USB=1) this ensures you don't lose any debug output while USB is set up
if (NOT DEFINED PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS)
    set(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS 3000)
endif()


# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if(TARGET tinyusb_device)

	# rest of your project
	add_executable(main
		main.cpp
	)


	# Add pico_stdlib library which aggregates commonly used features
	target_link_libraries(main pico_stdlib hardware_pwm pico_cyw43_arch_none)

	# enable usb output, disable uart output
	pico_enable_stdio_usb(main 1)
	pico_enable_stdio_uart(main 0)

	# create map/bin/hex/uf2 file in addition to ELF.
	pico_add_extra_outputs(main)

elseif(PICO_ON_DEVICE)
    message("Skipping because TinyUSB submodule is not initialized in the SDK")
endif()
