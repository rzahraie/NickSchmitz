Get-ChildItem -Recurse -File | ForEach-Object {
    $bytes = [System.IO.File]::ReadAllBytes($_.FullName)
    if ($bytes -notcontains 0) {
        $text = [System.Text.Encoding]::Default.GetString($bytes)
        $text = $text -replace "`r(?!`n)", "`r`n"
        [System.IO.File]::WriteAllText($_.FullName, $text)
    }
}

git add .
git commit -m "Normalize text file line endings"
git push