$files = Get-ChildItem -Recurse -File -Include *.C,*.H,*.TXT,*.BAT,*.ASM,*.PAS,*.PRN,*.HTM,*.X,*.Z,*.ZZ1,*.ZZ2

foreach ($f in $files) {
    $bytes = [System.IO.File]::ReadAllBytes($f.FullName)
    if ($bytes -notcontains 0) {
        $text = [System.Text.Encoding]::Default.GetString($bytes)
        $text = $text -replace "`r`n", "`n"
        $text = $text -replace "`r", "`n"
        $text = $text -replace "`n", "`r`n"
        [System.IO.File]::WriteAllText($f.FullName, $text, [System.Text.Encoding]::ASCII)
    }
}

git add .
git commit -m "Normalize legacy CR line endings"
git push