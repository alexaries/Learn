@echo off 
echo ���ڿ����ļ�
set /p TARGETPATH= < ClientPath.ini
 
xcopy /e Generated\ActionScript\*.as %TARGETPATH%

echo.&pause