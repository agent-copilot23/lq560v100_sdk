# 扫描当前目录所有大于100MB的文件
$limit = 100 * 1024 * 1024
Write-Host "===== 正在扫描文件夹中大于100MB的文件 =====" -ForegroundColor Cyan
$bigFiles = Get-ChildItem -Recurse -File | Where-Object { $_.Length -gt $limit } | Sort-Object Length -Descending

if ($bigFiles.Count -eq 0)
{
    Write-Host "✅ 扫描完成，没有找到大于100MB的文件！" -ForegroundColor Green
}
else
{
    Write-Host "❌ 找到以下超标文件：" -ForegroundColor Red
    foreach ($f in $bigFiles)
    {
        $sizeMB = [math]::Round($f.Length / 1MB, 2)
        Write-Host ("{0,10} MB | {1}" -f $sizeMB, $f.FullName)
    }
}
Write-Host "`n按回车关闭窗口..."
[Console]::ReadKey() | Out-Null