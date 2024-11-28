add_rules("mode.debug", "mode.release")
set_project("stm32_project")
set_version("1.0.0")
set_plat("cross")
set_arch("cortex-m3")

includes("arm-none-eabi-gcc.lua")
use_toolchain("D:/software/GNU Arm Embedded Toolchain/10 2021.10/")
add_rules("arm-gcc")
-- 目标 cjson
target("cjson")
    set_kind("static")
    add_files("3rdparty/cjson/*.c")
-- 目标 pid
target("pid")
    set_kind("static")
    add_files("3rdparty/pid/*.c")
-- 目标 motor_controller
target("motor_controller")
    set_kind("binary")
    add_defines("STM32F10X_HD", "USE_STDPERIPH_DRIVER")
    -- 添加链接选项
    add_ldflags(
        "-Tstartup/stm32_flash.ld", "-u", "_printf_float", {force = true} -- `force` 确保 LDFLAGS 参数优先级最高
    )
    -- 添加启动文件
    add_files("startup/startup_stm32f10x_md.s") -- 启动文件
    -- 添加源文件
    add_files("src/**.c") -- 包括 src 目录下的所有 C 文件
    add_files("STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/STM32F10x_StdPeriph_Driver/src/*.c")
    add_includedirs(
        "include","3rdparty", 
        "STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/CMSIS/CM3/CoreSupport/", 
        "STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/",
        "STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/STM32F10x_StdPeriph_Driver/inc/",
        {public = true}
    )
    -- 添加依赖项
    add_deps("cjson", "pid")
    add_links("m", "c", "nosys")

