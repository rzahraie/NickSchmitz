Add-Content .gitignore "`n*.zip`n*.ZIP"

git rm -r --cached -- "*.zip"
git rm -r --cached -- "*.ZIP"

git add .gitignore
git commit -m "Remove ZIP files and ignore archives"
git push