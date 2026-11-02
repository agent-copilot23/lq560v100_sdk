@echo off
chcp 65001 >nul
echo ===== 扫描当前目录及子目录大于100MB的文件 =====
echo.
for /r %%f in (*) do (
    for /f "tokens=3" %%s in ('dir "%%f" ^| findstr "%%~nxf"') do (
        set "size=%%s"
        setlocal enabledelayedexpansion
        set "size=!size:,=!"
        if !size! GTR 104857600 (
            echo 【超标】 %%f 大小：!size! 字节
        )
        endlocal
    )
)
echo.
echo 扫描结束，按任意键关闭
pause >nul