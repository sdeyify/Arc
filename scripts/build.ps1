Clear-Host

$files = Get-ChildItem -Path:. -Recurse -File
$cmd = "cls && cl /Zc:__cplusplus /permissive- /std:c++latest /EHsc"
$run = $false
$afterCmd = if ($run) { "&& cls && main.exe" } else { "" }
$allowOnly = @()
$notIncludes = "_______";
$winbuild = $false

if ($winbuild) {
    $cmd += " "
    $cmd += "/clr"
}

foreach ($file in $files) {
    $allowed = $false

    if ($allowOnly.Length -ne 0) {
        foreach ($path in $allowOnly) {
            if ($file.FullName.Contains($path)) {
                $allowed = $true
            }
        }
    }

    if ($notIncludes.Length -ne 0) {
        $allowed = !$file.FullName.Contains($notIncludes)
    }

    if (!$allowed) {
        continue
    }

    if ($file.Extension -eq ".cpp") {
        $cmd += " "
        $cmd += $file.FullName
    }
}

$cmd += " $afterCmd"

Write-Output "Copied $cmd to clipboard."
Set-Clipboard $cmd