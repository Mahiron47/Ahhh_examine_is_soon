@echo off
if not exist build mkdir build

REM Компилируем все исходники модуля laba3, чтобы определить все символы для линковки
g++ src/laba3/*.cpp -std=c++17 -O2 -Wall -Wextra -o build\main.exe

if errorlevel 1 (
	echo Сборка завершилась с ошибкой.
	exit /b 1
)

cd build

REM Запуск (в Windows достаточно main.exe или main)
main.exe