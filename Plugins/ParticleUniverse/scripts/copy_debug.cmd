copy /Y ..\..\..\bin\Debug\ParticleUniverse_d.dll ..\..\..\sdk\bin\Debug
copy /Y ..\..\..\bin\Debug\ParticleUniverse_d.pdb ..\..\..\sdk\bin\Debug
copy /Y ..\..\..\lib\Debug\ParticleUniverse_d.lib ..\..\..\sdk\lib\Debug\opt
xcopy ..\media\core\*.* ..\..\..\sdk\media\ParticleUniverse\core\ /Y /S
xcopy ..\include ..\..\..\sdk\include\ParticleUniverse\ /Y /S
