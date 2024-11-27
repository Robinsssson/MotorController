add_rules("mode.debug", "mode.release")
set_project("stm32_project")
set_version("1.0.0")

-- 设置工具链（使用 arm-none-eabi-gcc）
set_toolchains("arm-none-eabi")

-- 配置工具链（全局工具链设置）
toolchain("arm-none-eabi")
    set_kind("standalone")
    set_toolset("cc", "arm-none-eabi-gcc")
    set_toolset("ld", "arm-none-eabi-gcc")
    set_toolset("ar", "arm-none-eabi-ar")
    set_toolset("as", "arm-none-eabi-gcc")
toolchain_end()

-- 目标 3rd_cjson
target("3rd_cjson")
    set_kind("static")
    add_files("3rdparty/cjson/*.c")

-- 目标 3rd_pid
target("3rd_pid")
    set_kind("static")
    add_files("3rdparty/pid/*.c")

-- 目标 motor_controller
target("motor_controller")
    set_kind("binary")

    add_cflags(
        "-g", "-Wall", "--specs=nano.specs", "-march=armv7-m", "-O0",
        "-mlittle-endian", "-mthumb", "-mcpu=cortex-m3", "-mfloat-abi=soft"
    )
    add_defines("STM32F10X_HD", "USE_STDPERIPH_DRIVER")
    
    -- 添加链接选项
    add_ldflags(
        "-Wl,--gc-sections", "-Tstartup/stm32_flash.ld", "-u", "_printf_float",
        {force = true} -- `force` 确保 LDFLAGS 参数优先级最高
    )
    
    -- 添加启动文件
    add_files("startup/startup_stm32f10x_md.s") -- 启动文件
    
    -- 添加源文件
    add_files("src/**.c") -- 包括 src 目录下的所有 C 文件
    add_files("STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/STM32F10x_StdPeriph_Driver/src/*.c")
    -- 链接库
    add_links("m", "c") -- 数学库、标准库
    add_sysincludedirs("/opt/gcc-arm-none-eabi-10.3-2021.10/arm-none-eabi/include")
    -- 包含目录
    add_includedirs(
        "include","3rdparty", 
        "STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/CMSIS/CM3/CoreSupport/", 
        "STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/",
        "STM32F10x_StdPeriph_Lib_V3.6.0/Libraries/STM32F10x_StdPeriph_Driver/inc/",
        {public = true}
    )

    -- 添加依赖项
    add_deps("3rd_pid", "3rd_cjson")

task("flash")
    on_run(function ()
        print("hello xmake!")
        print("start flash")
    end)
    set_menu(
        {
            usage = "xmake flash",
            description = "flash to the MCU",
        }
    )