set MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
set PROJROOT=%~dp0\..\..

%MSBUILD% /p:Configuration=Release /p:Platform=ARM /t:Clean;Build %~dp0\rapidjson\rapidjson.sln
%MSBUILD% /p:Configuration=Release /p:Platform=x64 /t:Clean;Build %~dp0\rapidjson\rapidjson.sln
%MSBUILD% /p:Configuration=Release /p:Platform=x86 /t:Clean;Build %~dp0\rapidjson\rapidjson.sln

mkdir %PROJROOT%\dist\unity\WinRT.ARM 2> NUL
mkdir %PROJROOT%\dist\unity\WinRT.x64 2> NUL
mkdir %PROJROOT%\dist\unity\WinRT.x86 2> NUL

del %PROJROOT%\dist\unity\WinRT.ARM\rapidjson.dll 2> NUL
del %PROJROOT%\dist\unity\WinRT.x64\rapidjson.dll 2> NUL
del %PROJROOT%\dist\unity\WinRT.x86\rapidjson.dll 2> NUL

copy /B %~dp0\rapidjson\ARM\Release\rapidjson\rapidjson.dll %PROJROOT%\dist\unity\WinRT.ARM\rapidjson.dll
copy /B %~dp0\rapidjson\x64\Release\rapidjson\rapidjson.dll %PROJROOT%\dist\unity\WinRT.x64\rapidjson.dll
copy /B %~dp0\rapidjson\Release\rapidjson\rapidjson.dll %PROJROOT%\dist\unity\WinRT.x86\rapidjson.dll
