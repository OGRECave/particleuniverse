copy /Y ..\..\..\bin\Release\ParticleUniverse.dll ..\..\..\sdk\bin\Release
copy /Y ..\..\..\lib\Release\ParticleUniverse.lib ..\..\..\sdk\lib\Release\opt
xcopy ..\media\core\*.* ..\..\..\sdk\media\ParticleUniverse\core\ /Y /S
xcopy ..\include ..\..\..\sdk\include\ParticleUniverse\ /Y /S
