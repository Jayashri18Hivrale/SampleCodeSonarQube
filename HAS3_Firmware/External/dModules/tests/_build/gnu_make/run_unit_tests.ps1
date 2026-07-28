param(
    [switch]$Clean  # Add this parameter to enable cleaning
)

# Get the script's directory
$scriptPath = $PSScriptRoot

# Navigate up FOUR levels to reach the parent directory (C:\GitLab\netx)
$workspacePath = (Get-Item (Join-Path $scriptPath "..\..\..")).FullName

# Path inside the container where the workspace is mounted
$containerWorkspace = "/workspace"

# Path to the test directory RELATIVE TO THE REPOSITORY ROOT ($workspacePath)
$testDirectory = "tests/_build/gnu_make"

# Validate paths before running Docker
if (-Not (Test-Path (Join-Path $workspacePath $testDirectory))) {
  Write-Error "Test directory not found: $((Join-Path $workspacePath $testDirectory))"
  exit 1
}

# Determine the Makefile target (build or clean)
$makeTarget = if ($Clean) { "clean" } else { "" }

# Run the command in the container
docker run --rm `
  -v "${workspacePath}:${containerWorkspace}" `
  --workdir "${containerWorkspace}" `
  -i cpputest-ci `
  make -C "$testDirectory" $makeTarget  # Pass the target to make