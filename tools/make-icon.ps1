param([string]$Out = "$PSScriptRoot\..\res\keyswitcher.ico")

Add-Type -AssemblyName System.Drawing

$sizes = 16, 20, 24, 32, 40, 48, 64
$images = foreach ($s in $sizes) {
    $bmp = New-Object System.Drawing.Bitmap -ArgumentList $s, $s
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.SmoothingMode = 'AntiAlias'
    $g.TextRenderingHint = 'AntiAliasGridFit'

    [int]$r = [math]::Max(2, [int]($s / 3))
    $path = New-Object System.Drawing.Drawing2D.GraphicsPath
    $path.AddArc(0, 0, $r, $r, 180, 90)
    $path.AddArc($s - $r - 1, 0, $r, $r, 270, 90)
    $path.AddArc($s - $r - 1, $s - $r - 1, $r, $r, 0, 90)
    $path.AddArc(0, $s - $r - 1, $r, $r, 90, 90)
    $path.CloseFigure()
    $g.FillPath((New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(0x2B, 0x5C, 0xD8))), $path)

    $font = New-Object System.Drawing.Font -ArgumentList 'Segoe UI Semibold', ([single]($s * 7 / 8)), ([System.Drawing.FontStyle]::Regular), ([System.Drawing.GraphicsUnit]::Pixel)
    $format = New-Object System.Drawing.StringFormat
    $format.Alignment = 'Center'
    $format.LineAlignment = 'Center'
    $g.DrawString([string][char]0x0531, $font, [System.Drawing.Brushes]::White, (New-Object System.Drawing.RectangleF -ArgumentList 0, 0, $s, $s), $format)
    $g.Dispose()

    $ms = New-Object System.IO.MemoryStream
    $bmp.Save($ms, [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
    , @($s, $ms.ToArray())
}

$out = [System.IO.Path]::GetFullPath($Out)
[System.IO.Directory]::CreateDirectory([System.IO.Path]::GetDirectoryName($out)) | Out-Null
$fs = [System.IO.File]::Create($out)
$w = New-Object System.IO.BinaryWriter $fs
$w.Write([uint16]0); $w.Write([uint16]1); $w.Write([uint16]$images.Count)
[int]$offset = 6 + 16 * $images.Count
foreach ($img in $images) {
    $s = $img[0]; $data = $img[1]
    $dim = if ($s -ge 256) { 0 } else { $s }
    $w.Write([byte]$dim); $w.Write([byte]$dim); $w.Write([byte]0); $w.Write([byte]0)
    $w.Write([uint16]1); $w.Write([uint16]32)
    $w.Write([uint32]$data.Length); $w.Write([uint32]$offset)
    $offset += $data.Length
}
foreach ($img in $images) { $w.Write([byte[]]$img[1]) }
$w.Close()
"Wrote $out"
