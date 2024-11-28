-- base on git@github.com:xmake-examples/stm32-example-project.git project
local cflags = {"-g", "-Wall", "--specs=nano.specs", "-march=armv7-m", "-O0", "-mlittle-endian", "-mthumb",
                "-mcpu=cortex-m3", "-mfloat-abi=soft"}

local ldflags = {"-Wl,--gc-sections"}
local path

function use_toolchain(sdk_path)
    toolchain("arm-none-eabi")
    set_kind("cross")
    set_sdkdir(sdk_path)
    path = sdk_path
    if is_plat("windows") then
        set_toolset("cc", "arm-none-eabi-gcc.exe")
        set_toolset("ld", "arm-none-eabi-gcc.exe")
        set_toolset("ar", "arm-none-eabi-ar.exe")
        set_toolset("as", "arm-none-eabi-gcc.exe")
    else
        set_toolset("cc", "arm-none-eabi-gcc")
        set_toolset("ld", "arm-none-eabi-gcc")
        set_toolset("ar", "arm-none-eabi-ar")
        set_toolset("as", "arm-none-eabi-gcc")
    end
    toolchain_end()
    set_toolchains("arm-none-eabi")
end

rule("arm-gcc")
on_load(function(target)
    -- force add ld flags, ldflags {force = true}
    target:set("policy", "check.auto_ignore_flags", false)
    target:add("cxflags", cflags)
    target:add("asflags", cflags)
    target:add("sysincludedirs", path .. "arm-none-eabi/include")
    -- use gcc to link
    target:add("ldflags", cflags)
    target:add("ldflags", ldflags)
end)

after_build(function(target)
    if target:get("kind") ~= "binary" then
        return
    end
    print("after_build: generate hex files")
    local out = target:targetfile() or ""
    local gen_fi = "build/" .. target:name()
    print(string.format("%s => %s", out, gen_fi))
    if is_plat("windows") then
        os.exec("arm-none-eabi-objcopy.exe -Obinary " .. out .. " " .. gen_fi .. ".bin")
        os.execv("arm-none-eabi-objdump.exe", {"-S", out}, {
            stdout = gen_fi .. ".asm"
        })
        os.exec("arm-none-eabi-objcopy.exe -O ihex " .. out .. " " .. gen_fi .. ".hex")
    else
        os.exec("arm-none-eabi-objcopy -Obinary " .. out .. " " .. gen_fi .. ".bin")
        os.execv("arm-none-eabi-objdump", {"-S", out}, {
            stdout = gen_fi .. ".asm"
        })
        os.exec("arm-none-eabi-objcopy -O ihex " .. out .. " " .. gen_fi .. ".hex")
    end
    os.mv(out, gen_fi .. ".elf")
end)
after_clean(function(target)
    local gen_fi = "build/" .. target:name()
    os.rm(gen_fi .. ".*")
end)
rule_end()

task("flash")
on_run(function()
    print("Flashing the firmware to STM32...")
    if is_plat("windows") then
        os.exec(
            "openocd.exe -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c 'program ".. "build/" .. target:name() .. ".elf verify reset exit'")
    else
        os.exec(
            "openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c 'program ".. "build/" .. target:name() .. ".elf verify reset exit'")
    end

end)
set_menu({
    usage = "xmake flash",
    description = "Flash to the MCU"
})
