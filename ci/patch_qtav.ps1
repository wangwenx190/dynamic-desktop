$content = Get-Content "$env:QTDIR\include\QtAV\AVPlayer.h" -Raw
$content = $content.Replace("std::numeric_limits<qint64>::max()", "(std::numeric_limits<qint64>::max)()")
Set-Content "$env:QTDIR\include\QtAV\AVPlayer.h" $content
