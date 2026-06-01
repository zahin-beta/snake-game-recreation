param(
    [string]$AssetsDir = (Join-Path $PSScriptRoot "..\assets"),
    [string]$OutputFile = (Join-Path $PSScriptRoot "assets.c")
)

$files = Get-ChildItem -Path $AssetsDir -File | Where-Object { $_.Extension -notin @('.a') }

$content = @"
#include "assets.h"

"@

foreach ($file in $files) {
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    $varName = ($file.BaseName -replace '[^a-zA-Z0-9]', '_').ToUpper() + "_" + $file.Extension.Substring(1).ToUpper()

    $content += "const unsigned char ${varName}_DATA[] = {`n"

    $line = ""
    for ($i = 0; $i -lt $bytes.Length; $i++) {
        $line += "0x{0:X2}, " -f $bytes[$i]
        if (($i + 1) % 12 -eq 0) {
            $content += "    $line`n"
            $line = ""
        }
    }
    if ($line -ne "") {
        $content += "    $line`n"
    }

    $content += "};`n"
    $content += "const unsigned int ${varName}_SIZE = sizeof(${varName}_DATA);`n`n"
}

Set-Content -Path $OutputFile -Value $content -NoNewline
Write-Host "Generated $OutputFile ($($files.Count) assets)"
