param([ValidateSet('transfer','boot','app','all')][string]$Kind='all', [string]$AppProject='')
$ErrorActionPreference='Stop'
if($AppProject -and $Kind -ne 'app'){throw '-AppProject requires -Kind app.'}
$compiler='C:\Program Files (x86)\Renesas Electronics\CS+\CC\CubeSuite+.exe'
$kinds=if($Kind -eq 'all'){@('boot','app')}else{@($Kind)}
foreach($item in $kinds) {
    $folder=switch($item) {
        transfer { Join-Path $PSScriptRoot '..\RL78_F24_CAN_FD_Transfer' }
        boot { $PSScriptRoot }
        app { if($AppProject){$AppProject}else{Join-Path $PSScriptRoot '..\RL78_F24_Boot_loader_CAN_FD_app'} }
    }
    $folder=(Resolve-Path -LiteralPath $folder).Path
    & (Join-Path $PSScriptRoot 'tools\prepare_projects.ps1') -Kind $item -Project $folder
    $projects=@(Get-ChildItem -LiteralPath $folder -Filter '*.mtpj')
    if($projects.Count -ne 1){throw 'Expected one MTPJ.'}
    $logs=Join-Path $PSScriptRoot 'tmp\build'
    New-Item -ItemType Directory -Path $logs -Force | Out-Null
    $log=Join-Path $logs ($item+'_rebuild.log')
    $process=Start-Process -FilePath $compiler -ArgumentList ('/br "'+$projects[0].FullName+'"') -RedirectStandardOutput $log -RedirectStandardError ($log+'.stderr') -WindowStyle Hidden -PassThru
    $process.WaitForExit()
    $text=Get-Content -LiteralPath $log -Raw
    $text -split '\r?\n' | Select-Object -Last 14 | Write-Host
    if($text -notmatch 'Build ended\(Error:0, Warning:\d+\)' -or $text -match 'Failed:[1-9]'){throw "Build failed: $log"}
    & (Join-Path $PSScriptRoot 'tools\package_images.ps1') -Kind $item -Project $folder
    Write-Host "PASS: $item clean rebuild. Hardware NOT RUN."
}
