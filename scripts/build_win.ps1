$msBuildExe = 'C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\msbuild.exe'
& "$($msBuildExe)" C:\Users\Asda\Documents\programming\feud\build\src\feud.vcxproj
# & "$($msBuildExe)" C:\Users\Asda\Documents\programming\feud\build\src\feud.vcxproj /p:Configuration=Release

$destinationRoot = "C:\Users\Asda\Documents\programming\feud\build\src\Debug"

$sourceRoot = "C:\Users\Asda\Documents\programming\feud\ext\SFML\bin\*"
Copy-Item -Path $sourceRoot -Filter "*-d-2.dll" -Destination $destinationRoot

$sourceRoot = "C:\Users\Asda\Documents\programming\feud\ext\assimp\bin\Debug\*"
Copy-Item -Path $sourceRoot -Filter "*.dll" -Destination $destinationRoot

$sourceRoot = "C:\Users\Asda\Documents\programming\feud\ext\freetype\Debug\*"
Copy-Item -Path $sourceRoot -Filter "*.dll" -Destination $destinationRoot

# $destinationRoot = "C:\Users\Asda\Documents\programming\feud\build\src\Release"
# Copy-Item -Path $sourceRoot -Filter "*.dll" -Destination $destinationRoot