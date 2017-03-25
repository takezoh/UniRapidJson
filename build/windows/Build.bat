set MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
set PROJROOT=%~dp0\..\..

%MSBUILD% /p:Configuration=Release /p:Platform=x64 /t:Clean;Build %~dp0\rapidjson\rapidjson.sln
%MSBUILD% /p:Configuration=Release /p:Platform=x86 /t:Clean;Build %~dp0\rapidjson\rapidjson.sln

mkdir %PROJROOT%\dist\unity\Windows.x64 2> NUL
mkdir %PROJROOT%\dist\unity\Windows.x86 2> NUL
del %PROJROOT%\dist\unity\Windows.x64\rapidjson.dll 2> NUL
del %PROJROOT%\dist\unity\Windows.x86\rapidjson.dll 2> NUL

copy /B %~dp0\rapidjson\x64\Release\rapidjson.dll %PROJROOT%\dist\unity\Windows.x64\rapidjson.dll
copy /B %~dp0\rapidjson\Release\rapidjson.dll %PROJROOT%\dist\unity\Windows.x86\rapidjson.dll
