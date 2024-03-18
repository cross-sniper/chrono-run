-- Build command
if not arg or not table.concat(arg, " "):find("--skip-raylib") then
    os.execute("cd raylib; cmake -S . -B build -DUSE_WAYLAND=ON; cd build; make")
end

local buildCommand = "g++ src/main.cpp -DDEBUG_BUILD -o main -Os -O3 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -pthread -I/usr/include/AL -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -L src/imgui -I src/imgui -limgui -lraylib -DUSE_WAYLAND=ON -Wfatal-errors -Wall -L raylib/build/raylib -I raylib/build/raylib -lraylib"
-- Execute build command
print(buildCommand)
local success = os.execute(buildCommand)

-- Check if the build was successful
if success then
    print("Build successful")
else
    print("Build failed")
end
