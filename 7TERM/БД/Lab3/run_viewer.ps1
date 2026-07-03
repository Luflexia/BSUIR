param(
  [switch]$List,
  [string]$Table,
  [switch]$All,
  [string]$OutDir
)

$ErrorActionPreference = 'Stop'
$here = $PSScriptRoot

try { [Console]::OutputEncoding = [System.Text.Encoding]::UTF8 } catch {}
chcp 65001 > $null

# Загружаем конфиг
$configPath = Join-Path $here 'config.json'
if (!(Test-Path $configPath)) {
    throw "Не найден файл конфигурации: $configPath"
}
$cfg = Get-Content $configPath -Raw | ConvertFrom-Json

function Resolve([string]$p) {
    [System.IO.Path]::GetFullPath((Join-Path $here $p))
}

$jdkBin = Resolve $cfg.jdk_bin
$jeJar  = Resolve $cfg.je_jar
$envDir = Resolve $cfg.berkeley_env

$env:PATH = "$jdkBin;$env:PATH"
if ($cfg.java_tool_options) { $env:JAVA_TOOL_OPTIONS = $cfg.java_tool_options }

Write-Host "Компиляция DumpBerkeley.java..." -ForegroundColor Cyan
& "$jdkBin/javac.exe" -encoding UTF-8 -cp "$jeJar" (Join-Path $here 'DumpBerkeley.java')

function RunJava([string[]]$args) {
  & "$jdkBin/java.exe" @args
}

if ($List) {
  RunJava -args @('-cp', "$here;$jeJar", 'DumpBerkeley', '--env', $envDir, '--list')
  exit 0
}

if ($All) {
  $tables = RunJava -args @('-cp', "$here;$jeJar", 'DumpBerkeley', '--env', $envDir, '--list')
  if (-not $OutDir) { $OutDir = $here }
  foreach ($t in $tables) {
    RunJava -args @('-cp', "$here;$jeJar", 'DumpBerkeley', '--env', $envDir, $t) |
      Out-File -FilePath (Join-Path $OutDir ("view_{0}.txt" -f $t)) -Encoding utf8
  }
  Write-Host ("Сохранено: {0} таблиц в '{1}'" -f $tables.Count, (Resolve-Path $OutDir)) -ForegroundColor Green
  exit 0
}

if ($Table) {
  if ($OutDir) {
    RunJava -args @('-cp', "$here;$jeJar", 'DumpBerkeley', '--env', $envDir, $Table) |
      Out-File -FilePath (Join-Path $OutDir ("view_{0}.txt" -f $Table)) -Encoding utf8
    Write-Host ("Сохранено в файл: {0}" -f (Join-Path $OutDir ("view_{0}.txt" -f $Table))) -ForegroundColor Green
  } else {
    RunJava -args @('-cp', "$here;$jeJar", 'DumpBerkeley', '--env', $envDir, $Table)
  }
  exit 0
}

# По умолчанию просто список
RunJava -args @('-cp', "$here;$jeJar", 'DumpBerkeley', '--env', $envDir, '--list')