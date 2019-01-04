echo "first we need the java directory"
Add-Type -AssemblyName System.Windows.Forms
$JavaJre = New-Object System.Windows.Forms.FolderBrowserDialog -Property @{
        SelectedPath =  '\Program Files (x86)'
        ShowNewFolderButton = $false
        Description = "Please locate the '\java\jre1.8_*' Directory "
        
    }
[void]$JavaJre.ShowDialog()
$jrebin = ";"+$JavaJre.SelectedPath+"\bin"
$jreBinS = ";"+$JavaJre.SelectedPath+"\bin\client"
$jreBinP = ";"+$JavaJre.SelectedPath+"\bin\plugin2"
$addedPaths = $jrebin+$jreBinS+$jreBinP

[Environment]::SetEnvironmentVariable("Path",$env.Path+$addedPaths,[System.EnvironmentVariableTarget]::Machine)