$binDir = "../bin64"
if (Test-Path "../bin") {
    $binDir = "../bin"
}
if (Test-Path $binDir) {
    Set-Location $binDir
    Remove-Item "Qt?Svg.dll"
    Set-Location "./plugins"
    Remove-Item "./iconengines" -Recurse
    Set-Location "./imageformats"
    Remove-Item "./*.dll" -Exclude "qico.dll"
}
